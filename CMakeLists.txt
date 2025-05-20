cmake_minimum_required(VERSION 3.14)
project(http_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectory
add_subdirectory(include/socketlib)

# Main application
add_executable(App src/main.cpp)

# Link with the socketlib
target_link_libraries(App PRIVATE socketlib)
