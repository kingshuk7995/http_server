#include "Socket.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

constexpr int PORT = 8080;

const std::string http = R"(
<html>
<body style="background-color:black;color:white;">
  <h1>Hello world</h1>
</body>
</html>
)";

const char* response_header = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: %zu\r\n"
    "Connection: close\r\n"
    "\r\n";

int main() {
    ServerSocket s;
    if (!s.create()) {
        std::cerr << "Socket creation failed.\n";
        return 1;
    }
    std::cout << "Socket created successfully.\n";

    s.bind_socket(PORT);
    while (s.listen_socket()) {
        int client_socket = s.accept_connection();
        if (client_socket < 0) {
            std::cerr << "Failed to accept connection\n";
            continue;
        }

        std::cout << "Client connected. Sending message...\n";

        char header[256];
        std::snprintf(header, sizeof(header), response_header, http.size());

        std::string full_response = std::string(header) + http;

        s.send_message(client_socket, full_response.c_str());
    }
    
    std::cout << "Server closed\n";
    return 0;
}

