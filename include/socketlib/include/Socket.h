#ifndef _SOCKETLIB_SOCKET_H
#define _SOCKETLIB_SOCKET_H
#include <string>
#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

class ServerSocket {
public:
    ServerSocket();
    ~ServerSocket();
    bool create();
    bool bind_socket(int port);
    inline bool listen_socket(int backlog = 1) {
        return listen(m_sockfd, backlog) == 0;
    }

    inline int accept_connection() {
        return accept(m_sockfd, nullptr, nullptr);
    }

    inline bool send_message(int clientSock, const std::string& msg){
        return send(clientSock, msg.c_str(), msg.size(), 0) >= 0;
    }

    void close_socket();

private:
    int m_sockfd;
};


class ClientSocket {
public:
    ClientSocket();
    ~ClientSocket();
    void request(const char* hostname, int port, const std::string& message);
private:
    int m_sockfd;
};

#endif
