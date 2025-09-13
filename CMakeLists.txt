cmake_minimum_required(VERSION 3.10)
project(HttpCerver C)

set(CMAKE_C_STANDARD 99)

# Enable sanitizers in Debug mode
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fsanitize=undefined -g -O1")

# Source files
set(SOURCES
    src/server.c
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

# Link system libraries
target_link_libraries(http_server
    pthread
)