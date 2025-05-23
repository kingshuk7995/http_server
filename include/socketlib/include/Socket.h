#ifndef SOCKET_H
#define SOCKET_H

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
    bool listen_socket(int backlog = 5);
    int accept_connection();
    bool send_message(int clientSock, const std::string& msg);
    void close_socket();

private:
    int m_sockfd;
};

class ClientSocket {
public:
    ClientSocket();
    ~ClientSocket();

    void request(const char* hostname, int port, const std::string& message);
    int get_fd() const { return m_sockfd; }

private:
    int m_sockfd;
};

#endif // SOCKET_H
