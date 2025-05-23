#include "Http.h"
#include <sstream>
#include <iostream>

Http::Http(const std::string& raw) {
    std::istringstream stream(raw);
    std::string line;
    std::getline(stream, line);
    if (!line.empty() && line.back() == '\r') line.pop_back();

    std::istringstream first(line);
    std::string p1, p2, p3;
    first >> p1 >> p2 >> p3;

    if (p1.rfind("HTTP/", 0) == 0) {
        // Response
        m_is_request = false;
        m_http_version = p1;
        m_status_code = p2;
        m_status_message = p3;
    } else {
        // Request
        m_is_request = true;
        m_method = p1;
        m_url = p2;
        m_http_version = p3;
    }
    // Read headers
    while (std::getline(stream, line) && line != "\r" && !line.empty()) {
        if (line.back() == '\r') line.pop_back();
        auto sep = line.find(": ");
        if (sep != std::string::npos) {
            auto key = line.substr(0, sep);
            auto val = line.substr(sep + 2);
            m_headers[key] = val;
        }
    }
    // everything else as body
    std::string body;
    while (std::getline(stream, body, '\0')) {
        m_body += body;
    }
}

std::string Http::c_stringify() const {
    std::ostringstream out;
    if (m_is_request) {
        out << m_method << " " << m_url << " " << m_http_version << "\r\n";
    } else {
        out << m_http_version << " " << m_status_code << " " << m_status_message << "\r\n";
    }
    for (auto& [k,v] : m_headers) {
        out << k << ": " << v << "\r\n";
    }
    out << "\r\n" << m_body;
    return out.str();
}

bool Http::is_request() const { return m_is_request; }
const std::string& Http::get_method() const { return m_method; }
const std::string& Http::get_url() const { return m_url; }

const std::string& Http::get_header(const std::string& name) const {
    static std::string empty;
    auto it = m_headers.find(name);
    return it != m_headers.end() ? it->second : empty;
}

const std::string& Http::get_body() const { return m_body; }

void Http::set_status(const std::string& code, const std::string& message) {
    m_is_request = false;
    m_status_code = code;
    m_status_message = message;
    m_http_version = "HTTP/1.1";
}

void Http::set_header(const std::string& name, const std::string& value) {
    m_headers[name] = value;
}

void Http::set_body(const std::string& body) {
    m_body = body;
    m_headers["Content-Length"] = std::to_string(body.size());
}
