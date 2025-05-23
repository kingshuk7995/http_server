#include "LinkParser.h"
#include <iostream>

LinkParser::LinkParser() {
    m_root = std::make_shared<Node>();
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
    auto curr = m_root;
    for (auto& tok : tokens) {
        bool isDyn = !tok.empty() && tok[0] == ':';
        std::string key = isDyn ? ":" : tok;
        if (!curr->children.count(key))
            curr->children[key] = std::make_shared<Node>(
                isDyn ? LinkType::DYNAMIC : LinkType::STATIC
            );
        curr = curr->children[key];
    }
    curr->methods[method] = h;
}

void LinkParser::concat_links(const std::string& base, std::shared_ptr<Node> subm_root) {
    auto tokens = tokenize(base);
    auto curr = m_root;
    for (auto& tok : tokens) {
        std::string key = (!tok.empty() && tok[0] == ':') ? ":" : tok;
        if (!curr->children.count(key))
            curr->children[key] = std::make_shared<Node>();
        curr = curr->children[key];
    }
    for (auto& [k, child] : subm_root->children) {
        curr->children[k] = child;
    }
}

void LinkParser::call(const std::string& path, const std::string& method, const Http& req, Http& res) const {
    auto tokens = tokenize(path);
    auto curr = m_root;
    for (auto& tok : tokens) {
        if (curr->children.count(tok)) {
            curr = curr->children.at(tok);
        } else if (curr->children.count(":")) {
            curr = curr->children.at(":");
        } else {
            res.set_status("404", "Not Found");
            res.set_body("404 Not Found");
            return;
        }
    }

    if (curr->methods.count(method)) {
        curr->methods.at(method)(req, res);
    } else {
        res.set_status("405", "Method Not Allowed");
        res.set_body("405 Method Not Allowed");
    }
}
