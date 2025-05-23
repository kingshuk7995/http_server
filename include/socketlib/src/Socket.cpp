#include "Socket.h"
#include <iostream>
#include <cstring>

#ifdef _WIN32
  #include <winsock2.h>
#endif

ServerSocket::ServerSocket() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif
    m_sockfd = -1;
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

bool ServerSocket::bind_socket(int port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    return bind(m_sockfd, (struct sockaddr*)&addr, sizeof(addr)) == 0;
}

bool ServerSocket::listen_socket(int backlog) {
    return listen(m_sockfd, backlog) == 0;
}

int ServerSocket::accept_connection() {
    return accept(m_sockfd, nullptr, nullptr);
}

bool ServerSocket::send_message(int clientSock, const std::string& msg) {
    return send(clientSock, msg.c_str(), msg.size(), 0) >= 0;
}

void ServerSocket::close_socket() {
    if (m_sockfd >= 0) {
#ifdef _WIN32
        closesocket(m_sockfd);
#else
        ::close(m_sockfd);
#endif
        m_sockfd = -1;
    }
}

// ----------------------------------------------------------------

ClientSocket::ClientSocket() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd < 0) {
        std::cerr << "Failed to create client socket\n";
    }
}

ClientSocket::~ClientSocket() {
    if (m_sockfd >= 0) {
#ifdef _WIN32
        closesocket(m_sockfd);
        WSACleanup();
#else
        ::close(m_sockfd);
#endif
    }
}

void ClientSocket::request(const char* hostname, int port, const std::string& message) {
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
#ifdef _WIN32
    server_addr.sin_addr.s_addr = inet_addr(hostname);
#else
    if (inet_pton(AF_INET, hostname, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address: " << hostname << "\n";
        return;
    }
#endif

    if (connect(m_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed to " << hostname << ":" << port << "\n";
        return;
    }

    send(m_sockfd, message.c_str(), message.size(), 0);
}
