#include "Router.h"

void Router::route(const std::string& path, const std::string& method, std::function<void(const Http& req, Http& res)> handler) {
    parser.add_route(path, method, handler);
}

void Router::route(const std::string& path, Router& other) {
    parser.concat_links(path, other.parser.get_root());
}

void Router::call(const std::string& path, const std::string& method, const Http& req, Http& res) {
    parser.call(path, method, req, res);
}

