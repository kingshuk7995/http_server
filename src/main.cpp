#include "Server.h"
#include <iostream>

int main() {
    Server app(8080);

    // simple route
    app.route("/hello", "GET", []() {
        std::cout << "GET /hello\n";
    });

    // sub-router
    Router user;
    user.route("/profile", "GET", []() {
        std::cout << "GET /user/profile\n";
    });
    user.route("/profile", "POST", []() {
        std::cout << "POST /user/profile\n";
    });

    app.route("/user", user);

    app.start();
    return 0;
}
