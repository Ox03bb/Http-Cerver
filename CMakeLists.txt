cmake_minimum_required(VERSION 3.10)
project(HttpCerver C)

find_package(OpenSSL REQUIRED)

set(CMAKE_C_STANDARD 99)

# Enable sanitizers in Debug mode
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fsanitize=undefined -g -O1")

# Source files
set(SOURCES
    src/server.c
    src/logging.c
    src/proxy.c
    src/secure_con.c
    src/http/parser.c
    src/http/responce.c
    src/http/responce_headers.c
)

# Include directories
include_directories(
    includes
    includes/http
)

add_executable(http_server ${SOURCES})


target_link_libraries(http_server PRIVATE
    pthread
    OpenSSL::SSL
    OpenSSL::Crypto
)