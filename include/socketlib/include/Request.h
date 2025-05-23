#ifndef REQUEST_H
#define REQUEST_H

#include "Http.h"
#include <string>

class Request {
public:
    static Http fetch(const std::string& host, int port, const std::string& path);
};

#endif // REQUEST_H
