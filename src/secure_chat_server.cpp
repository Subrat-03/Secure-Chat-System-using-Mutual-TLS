#include "tls_wrapper.hpp"
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <map>
#include <mutex>

#define PORT 5555
#define BUFFER_SIZE 4096

std::map<std::string, SSL*> clients;
std::mutex client_mutex;

void handle_client(SSL* ssl) {
    std::string client_name = TLSWrapper::get_cert_subject(ssl);
    {
        std::lock_guard<std::mutex> lock(client_mutex);
        clients[client_name] = ssl;
    }
    std::cout << "✅ Authenticated: " << client_name << std::endl;
    std::cout << "🔗 Currently connected clients:" << std::endl;
    for (const auto& pair : clients) {
        std::cout << " - " << pair.first << std::endl;
    }

    char buffer[BUFFER_SIZE];
    while (true) {
        int bytes = SSL_read(ssl, buffer, sizeof(buffer));
        if (bytes <= 0) break;
        buffer[bytes] = '\0';

        std::string msg(buffer);
        auto delimiter_pos = msg.find(":");
        if (delimiter_pos != std::string::npos) {
            std::string receiver = msg.substr(0, delimiter_pos);
            std::string actual_msg = msg.substr(delimiter_pos + 1);
            std::lock_guard<std::mutex> lock(client_mutex);
            if (clients.count(receiver)) {
                SSL_write(clients[receiver], actual_msg.c_str(), actual_msg.size());
            } else {
                std::string error = "Receiver not found: " + receiver + "\n";
                SSL_write(ssl, error.c_str(), error.size());
            }
        } else {
            std::string error = "Invalid message format. Use <ReceiverCN>:<Message>\n";
            SSL_write(ssl, error.c_str(), error.size());
        }
    }

    std::lock_guard<std::mutex> lock(client_mutex);
    clients.erase(client_name);
    SSL_shutdown(ssl);
    SSL_free(ssl);
    std::cout << "Disconnected: " << client_name << std::endl;
}

int main() {
    TLSWrapper::init_openssl();
    SSL_CTX* ctx = TLSWrapper::create_context(true);
    TLSWrapper::configure_context(ctx, true);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    std::cout << "🔐 Server listening on port " << PORT << std::endl;

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        SSL* ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_fd);

        std::cout << "Waiting for secure handshake...\n";
        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            close(client_fd);
        } else {
            std::thread(handle_client, ssl).detach();
        }
    }

    close(server_fd);
    SSL_CTX_free(ctx);
    TLSWrapper::cleanup_openssl();
    return 0;
}