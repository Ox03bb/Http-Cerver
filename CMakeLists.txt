cmake_minimum_required(VERSION 3.10)
project(HttpCerver C)

set(CMAKE_C_STANDARD 99)

# Source files
set(SOURCES
    src/server.c
    src/http/parser.c
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