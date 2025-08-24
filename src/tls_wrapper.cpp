#include "tls_wrapper.hpp"
#include <iostream>

void TLSWrapper::init_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void TLSWrapper::cleanup_openssl() {
    EVP_cleanup();
}

SSL_CTX* TLSWrapper::create_context(bool is_server) {
    const SSL_METHOD* method = is_server ? TLS_server_method() : TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

void TLSWrapper::configure_context(SSL_CTX* ctx, bool is_server, const std::string& certName) {
    std::string cert = is_server ? "certs/server.crt" : "certs/" + certName + ".crt";
    std::string key  = is_server ? "certs/server.key"  : "certs/" + certName + ".key";

    SSL_CTX_use_certificate_file(ctx, cert.c_str(), SSL_FILETYPE_PEM);
    SSL_CTX_use_PrivateKey_file(ctx, key.c_str(), SSL_FILETYPE_PEM);

    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, nullptr);
    SSL_CTX_load_verify_locations(ctx, "certs/ca.crt", nullptr);
}

std::string TLSWrapper::get_cert_subject(SSL* ssl) {
    X509* cert = SSL_get_peer_certificate(ssl);
    if (!cert) return "(No Certificate)";

    char subject[256];
    X509_NAME_oneline(X509_get_subject_name(cert), subject, sizeof(subject));
    X509_free(cert);
    return std::string(subject);
}