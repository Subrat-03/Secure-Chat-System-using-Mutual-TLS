#!/bin/bash

NUM_CLIENTS=$1

if [[ -z "$NUM_CLIENTS" ]]; then
  echo "Usage: ./generate_clients.sh <number_of_clients>"
  exit 1
fi

mkdir -p certs

# Check if CA files exist
if [[ ! -f certs/ca.crt || ! -f certs/ca.key ]]; then
  echo "❌ Missing certs/ca.crt or certs/ca.key. Make sure the CA is set up."
  exit 1
fi

for ((i = 1; i <= NUM_CLIENTS; i++)); do
  CN="Client$i"
  KEY="certs/client$i.key"
  CSR="certs/client$i.csr"
  CRT="certs/client$i.crt"

  echo "🔐 Generating certificate for $CN..."

  openssl genrsa -out "$KEY" 2048
  openssl req -new -key "$KEY" -out "$CSR" -subj "/CN=$CN"
  openssl x509 -req -in "$CSR" -CA certs/ca.crt -CAkey certs/ca.key -CAcreateserial -out "$CRT" -days 365 -sha256
  rm "$CSR"  # cleanup
done

echo "✅ Generated $NUM_CLIENTS client certificate(s) in certs/"
