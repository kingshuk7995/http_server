#include "Http.h"
#include <cstring>
#include <sstream>

Http::Http(const std::string& http) {
    std::istringstream stream(http);
    std::string first_line;
    std::getline(stream, first_line);
    if (!first_line.empty() && first_line.back() == '\r') {
        first_line.pop_back();
    }

    std::istringstream first_line_stream(first_line);
    std::string part1, part2, part3;
    first_line_stream >> part1 >> part2 >> part3;

    if (part1.rfind("HTTP/", 0) == 0) {
        // It's a response line: HTTP/1.1 200 OK
        m_is_request = false;
        m_http_version = part1;
        m_status_code = part2;
        std::getline(first_line_stream, m_status_message);
        if (!m_status_message.empty() && m_status_message.front() == ' ')
            m_status_message.erase(0, 1);
    } else {
        // It's a request line: GET /index.html HTTP/1.1
        m_is_request = true;
        m_method = part1;
        m_url = part2;
        m_http_version = part3;
    }

    // Parse headers
    std::string line;
    while (std::getline(stream, line) && line != "\r") {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        size_t sep = line.find(':');
        if (sep != std::string::npos) {
            std::string key = line.substr(0, sep);
            std::string value = line.substr(sep + 2);
            m_headers[key] = value;
        }
    }

    // Remaining is body
    std::getline(stream, m_body, '\0');
}


std::string Http::c_stringify() const {
    std::string result;

    if (m_is_request) {
        result = m_method + " " + m_url + " " + m_http_version + "\r\n";
    } else {
        result = m_http_version + " " + m_status_code + " " + m_status_message + "\r\n";
    }

    for (const auto& [key, value] : m_headers) {
        result += key + ": " + value + "\r\n";
    }

    result += "\r\n" + m_body;
    return result;
}
