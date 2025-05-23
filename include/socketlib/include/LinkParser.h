#ifndef LINKPARSER_H
#define LINKPARSER_H

#include <map>
#include <memory>
#include <string>
#include <functional>
#include <vector>
#include <sstream>

class LinkParser {
public:
    using Handler = std::function<void()>;

    enum class LinkType { STATIC, DYNAMIC };

    struct Node {
        std::map<std::string, std::shared_ptr<Node>> children;
        std::map<std::string, Handler> methods;
        LinkType type = LinkType::STATIC;
        Node(LinkType t = LinkType::STATIC) : type(t){}
    };

    LinkParser();
    void add_route(const std::string& path, const std::string& method, Handler h);
    void concat_links(const std::string& base, std::shared_ptr<Node> subroot);
    void call(const std::string& path, const std::string& method) const;
    std::shared_ptr<Node> get_root() const;

private:
    std::shared_ptr<Node> root;
    std::vector<std::string> tokenize(const std::string& path) const;
};

#endif // LINKPARSER_H
