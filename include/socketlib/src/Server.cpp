#include "Server.h"
#include "Http.h"
#include <iostream>

Server::Server(int port) {
    if (!m_sock.create() || !m_sock.bind_socket(port)) {
        std::cerr << "Failed to bind server socket on port " << port << "\n";
        exit(1);
    }
    m_running = true;
}

void Server::start() {
    std::cout << "Server listening...\n";
    while (m_running && m_sock.listen_socket()) {
        int client = m_sock.accept_connection();
        if (client < 0) continue;
        std::thread([this, client]() {
            char buf[4096];
            int n = recv(client, buf, sizeof(buf)-1, 0);
            if (n > 0) {
                buf[n] = '\0';
                Http req(std::string(buf, n));
                Http res("");
                res.set_status("200", "OK");

                // Dispatch through our router
                this->call(req.get_url(), req.get_method(), req, res);

                m_sock.send_message(client, res.c_stringify());
            }
#ifdef _WIN32
            closesocket(client);
#else
            close(client);
#endif
        }).detach();
    }
}

void Server::stop() {
    m_running = false;
    m_sock.close_socket();
}
