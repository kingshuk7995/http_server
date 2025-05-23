#ifndef SERVER_H
#define SERVER_H

#include "Router.h"
#include "Socket.h"
#include <thread>
#include <atomic>

class Server : public Router {
    ServerSocket m_sock;
    std::atomic<bool> m_running{false};

public:
    explicit Server(int port);
    void start();
    void stop();
};

#endif // SERVER_H
