#ifndef ROUTER_H
#define ROUTER_H

#include "LinkParser.h"
#include <string>
#include <functional>
#include <memory>

class Router {
protected:
    LinkParser parser;

public:
    void route(const std::string& path, const std::string& method, LinkParser::Handler handler);
    void route(const std::string& path, Router& other);
    void call(const std::string& path, const std::string& method);
    LinkParser& get_parser() {return parser; }
};

#endif // ROUTER_H
