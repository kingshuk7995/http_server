#include "Socket.h"
#include <string.h>
#include <iostream>

ServerSocket::ServerSocket() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
}

ServerSocket::~ServerSocket() {
    close_socket();
#ifdef _WIN32
    WSACleanup();
#endif
}

bool ServerSocket::create() {
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return m_sockfd >= 0;
}

void ServerSocket::close_socket() {
#ifdef _WIN32
    closesocket(m_sockfd);
#else
    close(m_sockfd);
#endif
}

bool ServerSocket::bind_socket(int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    memset(&(addr.sin_zero), 0, 8);
    return bind(m_sockfd, (struct sockaddr*)&addr, sizeof(addr)) == 0;
}

ClientSocket::ClientSocket() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd < 0) {
        std::cerr << "Failed to create client socket.\n";
    }
}

ClientSocket::~ClientSocket() {
#ifdef _WIN32
    closesocket(m_sockfd);
    WSACleanup();
#else
    close(m_sockfd);
#endif
}

void ClientSocket::request(const char* hostname, int port, const std::string& message) {
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

#ifdef _WIN32
    server_addr.sin_addr.s_addr = inet_addr(hostname);
#else
    if (inet_pton(AF_INET, hostname, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address: " << hostname << std::endl;
        return;
    }
#endif

    if (connect(m_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection to " << hostname << " on port " << port << " failed.\n";
        return;
    }

    send(m_sockfd, message.c_str(), message.length(), 0);

    char buffer[4096];
    int bytesReceived = recv(m_sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }
}
