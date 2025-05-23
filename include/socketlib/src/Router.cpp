#include "Router.h"

void Router::route(const std::string& path, const std::string& method, LinkParser::Handler handler) {
    parser.add_route(path, method, handler);
}

void Router::route(const std::string& path, Router& other) {
    parser.concat_links(path, other.get_parser().get_root());
}

void Router::call(const std::string& path, const std::string& method) {
    parser.call(path, method);
}
