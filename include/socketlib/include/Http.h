#ifndef HTTP_H
#define HTTP_H

#include <string>
#include <map>

class Http {
public:
    Http(const std::string& raw);
    ~Http() = default;

    // Serialization
    std::string c_stringify() const;

    // Request getters
    bool is_request() const;
    const std::string& get_method() const;
    const std::string& get_url() const;

    // Common
    const std::string& get_header(const std::string& name) const;
    const std::string& get_body() const;

    // Response setters
    void set_status(const std::string& code, const std::string& message);
    void set_header(const std::string& name, const std::string& value);
    void set_body(const std::string& body);

private:
    bool m_is_request;
    std::string m_http_version;
    std::string m_method, m_url;
    std::string m_status_code, m_status_message;
    std::map<std::string, std::string> m_headers;
    std::string m_body;
};

#endif // HTTP_H
