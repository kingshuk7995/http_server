#ifndef _HTTP_H_
#define _HTTP_H_

#include <string>
#include <map>

class Http {
public:
    // Construct from raw HTTP message (either request or response)
    Http(const std::string& http);
    ~Http();

    // Serialize back into string
    std::string c_stringify() const;

    // --- Request-only setters/getters ---
    inline void set_method(const std::string& method) { m_method = method; }
    inline const std::string& get_method() const { return m_method; }

    inline void set_url(const std::string& url) { m_url = url; }
    inline const std::string& get_url() const { return m_url; }

    // --- Response-only setters/getters ---
    inline void set_status_code(const std::string& code) { m_status_code = code; }
    inline const std::string& get_status_code() const { return m_status_code; }

    inline void set_status_message(const std::string& message) { m_status_message = message; }
    inline const std::string& get_status_message() const { return m_status_message; }

    // --- Shared ---
    inline void set_http_version(const std::string& version) { m_http_version = version; }
    inline const std::string& get_http_version() const { return m_http_version; }

    inline void set_header(const std::string& name, const std::string& value) {
        m_headers[name] = value;
    }

    inline const std::string& get_header(const std::string& name) const {
        static const std::string empty;
        auto it = m_headers.find(name);
        return (it != m_headers.end()) ? it->second : empty;
    }

    inline const std::map<std::string, std::string>& get_headers() const {
        return m_headers;
    }

    inline void set_body(const std::string& body) { m_body = body; }
    inline const std::string& get_body() const { return m_body; }

    inline bool is_request() const { return m_is_request; }
    inline bool is_response() const { return !m_is_request; }

private:
    bool m_is_request;

    // Shared
    std::string m_http_version;
    std::map<std::string, std::string> m_headers;
    std::string m_body;

    // Request
    std::string m_method;
    std::string m_url;

    // Response
    std::string m_status_code;
    std::string m_status_message;
};

#endif
