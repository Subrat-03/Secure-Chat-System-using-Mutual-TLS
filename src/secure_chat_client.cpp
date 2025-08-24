#include "tls_wrapper.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <thread>

#define PORT 5555
#define BUFFER_SIZE 4096

void receive_messages(SSL* ssl) {
    char buffer[BUFFER_SIZE];
    while (true) {
        int bytes = SSL_read(ssl, buffer, sizeof(buffer));
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        std::cout << "🔐 [Encrypted Reply]: " << buffer << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::string certName = "client";
    if (argc > 1) {
        certName = argv[1];
    }

    TLSWrapper::init_openssl();
    SSL_CTX* ctx = TLSWrapper::create_context(false);
    TLSWrapper::configure_context(ctx, false, certName);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock, (sockaddr*)&server_addr, sizeof(server_addr));

    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);

    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(sock);
        return 1;
    }

    std::cout << "✅ Connected to server with authenticated certificate.\n";
    std::thread(receive_messages, ssl).detach();

    std::string input;
    std::cout << "Format: <ReceiverCN>:<Message>\n";
    while (true) {
        std::getline(std::cin, input);
        if (input == "exit") break;
        SSL_write(ssl, input.c_str(), input.length());
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sock);
    SSL_CTX_free(ctx);
    TLSWrapper::cleanup_openssl();
    return 0;
}
