project(socketlib LANGUAGES CXX)

add_library(socketlib STATIC
    src/Socket.cpp
    src/Http.cpp
    src/LinkParser.cpp
    src/Server.cpp
    src/Request.cpp
    src/Router.cpp
)

target_include_directories(socketlib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Windows-specific
if(WIN32)
    target_compile_definitions(socketlib PUBLIC _WIN32)
    target_link_libraries(socketlib PUBLIC ws2_32)
endif()
