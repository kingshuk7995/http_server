#include "Server.h"
#include <iostream>

int main() {
    // 1. Instantiate your app on port 8080
    Server app(8080);

    // 2. Simple GET handler at "/"
    app.route("/", "GET",
        [](const Http& req, Http& res) {
            res.set_header("Content-Type", "text/plain");
            res.set_body("Welcome to the home page!\n");
        }
    );

    // 3. Another route with dynamic segment, e.g. "/user/:id"
    app.route("/user/:id", "GET",
        [](const Http& req, Http& res) {
            // Note: we haven't parsed params; token ":" matched
            // You could extend Http to parse the ":id" into a param map.
            res.set_header("Content-Type", "application/json");
            res.set_body("{ \"message\": \"User details here\" }");
        }
    );

    // 4. Create a sub-router for "/api"
    Router api;
    api.route("/status", "GET",
        [](const Http& req, Http& res) {
            res.set_header("Content-Type", "application/json");
            res.set_body("{ \"status\": \"OK\", \"uptime\": 12345 }");
        }
    );
    api.route("/echo", "POST",
        [](const Http& req, Http& res) {
            // Echo back whatever body was sent
            res.set_header("Content-Type", "text/plain");
            res.set_body("You posted:\n" + req.get_body());
        }
    );

    // 5. Mount the API router at "/api"
    app.route("/api", api);

    // 6. Start the server (blocking)
    std::cout << "Starting server on http://localhost:8080\n";
    app.start();

    return 0;
}
