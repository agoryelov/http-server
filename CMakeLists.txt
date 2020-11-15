cmake_minimum_required(VERSION 3.17)
project(COMP4981_A3 C)

set(CMAKE_C_STANDARD 11)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_library(http STATIC ./http_protocol/http.c)

add_library(http_config STATIC ./http_protocol/config.c)
target_link_libraries(http_config config)
target_compile_options(http_config PRIVATE -Wpedantic -Wall -Wextra)

add_executable(server server.c)
target_link_libraries(server dc http http_config)

add_executable(client client.c)
target_link_libraries(client dc)