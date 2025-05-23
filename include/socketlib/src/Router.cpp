#include "Router.h"

void Router::route(const std::string& path, const std::string& method, std::function<void(const Http& req, Http& res)> handler) {
    m_parser.add_route(path, method, handler);
}

void Router::route(const std::string& path, Router& other) {
    m_parser.concat_links(path, other.m_parser.get_root());
}

void Router::call(const std::string& path, const std::string& method, const Http& req, Http& res) {
    m_parser.call(path, method, req, res);
}

