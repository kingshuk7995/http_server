#include "Request.h"
#include "Socket.h"
#include <sstream>

Http Request::fetch(const std::string& host, int port, const std::string& path) {
    ClientSocket cs;
    std::ostringstream req;
    req << "GET " << path << " HTTP/1.1\r\n";
    req << "Host: " << host << "\r\n\r\n";
    cs.request(host.c_str(), port, req.str());

    char buf[4096];
    int n = recv(cs.get_fd(), buf, sizeof(buf)-1, 0);
    if (n <= 0) return Http("");
    buf[n] = '\0';
    return Http(buf);
}
