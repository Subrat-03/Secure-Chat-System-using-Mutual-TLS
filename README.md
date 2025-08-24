


# Secure Chat System with Mutual TLS (mTLS)

A secure, terminal-based chat application developed in **C++** using **mutual TLS (mTLS)** with OpenSSL.  
This project demonstrates a production-grade security mechanism where both client and server authenticate each other using digital certificates, ensuring **confidentiality, authenticity, and integrity** of all communications.

---

## Features
- Mutual TLS authentication: both server and client verify certificates.  
- TLS 1.3 encrypted communication ensuring secure message exchange.  
- Multi-client support with concurrent connections via multithreading.  
- Modular C++ codebase designed for scalability and portability.  
- Tested on Linux systems with planned extension for Windows compatibility.  

---

## System Architecture

### High-Level Workflow
```

\[ Client 1 ] --
\[ Client 2 ] ---- mTLS ----> \[ Secure Chat Server (C++) ]
\[ Client N ] --/

````

### Architecture Diagram

![Architecture Diagram](./ChatGPT%20Image%20Jul%2024%2C%202025%2C%2004_11_16%20PM.png)
 

```markdown
![Architecture Diagram](docs/architecture.png)
````

---

## Technology Stack

* **Language**: C++17
* **Security**: OpenSSL (TLS 1.3, X.509 certificates)
* **Concurrency**: POSIX Threads (Linux)
* **Build Tool**: CMake

---

## Project Structure

```
secure-chat-mtls/
│── src/
│   ├── server.cpp        # Server implementation
│   ├── client.cpp        # Client implementation
│   ├── tls_utils.cpp     # TLS and certificate utilities
│── include/              # Header files
│── certs/                # Certificates (generated locally)
│── scripts/
│   └── generate-certs.sh # Helper script for certificate generation
│── docs/
│   ├── architecture.png  # System diagram (placeholder)
│   └── demo.png          # Terminal session screenshot
│── CMakeLists.txt
│── README.md
```

---

## Prerequisites

* C++17 or later
* OpenSSL (>= 1.1.1)
* CMake build system
* Linux/Unix-like environment (initial testing platform)

---

## Installation

1. **Clone the repository**

```bash
git clone https://github.com/Anshuman773/Secure-Chat-System-using-mTLS.git
cd secure-chat-mtls
```

2. **Generate certificates**
   Run the helper script to create CA, server, and client certificates:

```bash
./scripts/generate-certs.sh
```

This generates:

* `ca.crt` – Certificate Authority
* `server.crt`, `server.key` – Server certificate and private key
* `client.crt`, `client.key` – Client certificate and private key

3. **Build the project**

```bash
mkdir build && cd build
cmake ..
make
```

---

### Demo Video

A short demonstration video of the system in action is included in the repository:

[![Watch the video](thumb.png)](sasken.mp4)


### Start the Server

```bash
./server ../certs/server.crt ../certs/server.key ../certs/ca.crt
```

### Start the Client

```bash
./client ../certs/client.crt ../certs/client.key ../certs/ca.crt
```

---

## Example Session (Terminal Demo)

Below is a demonstration of a secure session between a client and the server:

```markdown
![Terminal Demo](docs/demo.png)<img width="1520" height="279" alt="Screenshot from 2025-07-13 20-19-30" src="https://github.com/user-attachments/assets/2085ee40-0bba-4d00-905c-fdbe265f93c1" />

```

---

## Security Considerations

* Both server and client must present valid certificates signed by the same Certificate Authority (CA).
* TLS 1.3 is enforced to guarantee strong encryption.
* Private keys (`.key` files) should be stored securely and excluded from version control.
* Only CA-signed clients are permitted to connect, preventing unauthorized access.

---
