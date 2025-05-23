#include "Server.h"
#include "Http.h"
#include <iostream>

Server::Server(int port) {
    if (!sock.create() || !sock.bind_socket(port)) {
        std::cerr << "Failed to start server on port " << port << "\n";
        exit(1);
    }
    running = true;
}

void Server::start() {
    std::cout << "Server listening...\n";
    while (running && sock.listen_socket()) {
        int client = sock.accept_connection();
        if (client < 0) continue;
        std::thread([this, client]() {
            char buf[4096];
            int n = recv(client, buf, sizeof(buf)-1, 0);
            if (n > 0) {
                buf[n] = '\0';
                Http req(buf);
                this->call(req.get_url(), req.get_method());
                Http res("");
                res.set_status("200", "OK");
                res.set_header("Content-Type", "text/plain");
                res.set_body("Hello World\n");
                sock.send_message(client, res.c_stringify());
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
    running = false;
    sock.close_socket();
}
