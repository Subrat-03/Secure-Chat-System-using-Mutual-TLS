#ifndef TLS_WRAPPER_HPP
#define TLS_WRAPPER_HPP

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>

class TLSWrapper {
public:
    static void init_openssl();
    static void cleanup_openssl();
    static SSL_CTX* create_context(bool is_server);
    static void configure_context(SSL_CTX* ctx, bool is_server, const std::string& certName = "client");
    static std::string get_cert_subject(SSL* ssl);
};

#endif