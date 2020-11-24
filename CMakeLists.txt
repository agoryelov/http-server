cmake_minimum_required(VERSION 3.17)
project(COMP4981_A3 C)


set(CMAKE_C_STANDARD 11)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_library(str_map STATIC ./libs/str_map.c)
target_compile_options(str_map PRIVATE -Wpedantic -Wall -Wextra)

add_library(http STATIC ./http_protocol/http.c)
target_link_libraries(http str_map dc)
target_compile_options(http PRIVATE -Wpedantic -Wall -Wextra)

add_library(http_config STATIC ./http_protocol/config.c)
target_link_libraries(http_config config)
target_compile_options(http_config PRIVATE -Wpedantic -Wall -Wextra)

add_executable(server server.c)
target_link_libraries(server dc http http_config str_map pthread)
target_compile_options(server PRIVATE -Wpedantic -Wall -Wextra)

add_executable(client client.c)
target_link_libraries(client dc)
target_compile_options(client PRIVATE -Wpedantic -Wall -Wextra)
