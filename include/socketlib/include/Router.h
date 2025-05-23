#ifndef ROUTER_H
#define ROUTER_H

#include "LinkParser.h"
#include "Http.h"
#include <string>
#include <functional>

class Router {
protected:
    LinkParser m_parser;

public:
    // Register a handler for a specific HTTP method at `path`
    void route(const std::string& path, const std::string& method, std::function<void(const Http& req, Http& res)> handler);

    // Mount another Router at `base` path
    void route(const std::string& path, Router& other);

    // Dispatch an incoming request to the correct handler
    void call(const std::string& path, const std::string& method, const Http& req, Http& res);
};

#endif // ROUTER_H
