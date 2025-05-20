#ifndef _HTML_H_
#define _HTML_H_

enum class HtmlMethods {
    GET,
    HEAD,
    OPTIONS,
    TRACE,
    PUT,
    DELETE,
    POST,
    PATCH,
    CONNECT
};
class Html {
public:
    Html();
    ~Html();
    const char* c_stringify();
public:
    enum class HtmlMethods {
        GET,
        HEAD,
        OPTIONS,
        TRACE,
        PUT,
        DELETE,
        POST,
        PATCH,
        CONNECT
    };
}

#else
#endif
