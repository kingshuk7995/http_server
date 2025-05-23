#include "LinkParser.h"
#include <iostream>

LinkParser::LinkParser() {
    root = std::make_shared<Node>();
}

std::vector<std::string> LinkParser::tokenize(const std::string& path) const {
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string seg;
    while (std::getline(ss, seg, '/')) {
        if (!seg.empty()) parts.push_back(seg);
    }
    return parts;
}

void LinkParser::add_route(const std::string& path, const std::string& method, Handler h) {
    auto tokens = tokenize(path);
    auto curr = root;
    for (auto& tok : tokens) {
        bool dyn = !tok.empty() && tok[0] == ':';
        std::string key = dyn ? ":" : tok;
        if (!curr->children.count(key))
            curr->children[key] = std::make_shared<Node>(dyn ? LinkType::DYNAMIC : LinkType::STATIC);
        curr = curr->children[key];
    }
    curr->methods[method] = h;
}

void LinkParser::concat_links(const std::string& base, std::shared_ptr<Node> subroot) {
    auto tokens = tokenize(base);
    auto curr = root;
    for (auto& tok : tokens) {
        std::string key = (!tok.empty() && tok[0] == ':') ? ":" : tok;
        if (!curr->children.count(key))
            curr->children[key] = std::make_shared<Node>();
        curr = curr->children[key];
    }
    for (auto& [k, child] : subroot->children) {
        curr->children[k] = child;
    }
}

void LinkParser::call(const std::string& path, const std::string& method) const {
    auto tokens = tokenize(path);
    auto curr = root;
    for (auto& tok : tokens) {
        if (curr->children.count(tok)) {
            curr = curr->children.at(tok);
        } else if (curr->children.count(":")) {
            curr = curr->children.at(":");
        } else {
            std::cerr << "404 Not Found: " << path << "\n";
            return;
        }
    }
    if (curr->methods.count(method)) {
        curr->methods.at(method)();
    } else {
        std::cerr << "405 Method Not Allowed: " << method << " at " << path << "\n";
    }
}

std::shared_ptr<LinkParser::Node> LinkParser::get_root() const {
    return root;
}
