cmake_minimum_required(VERSION 3.17)
project(COMP4981_A3 C)

set(CMAKE_C_STANDARD 11)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_executable(server server.c)
target_link_libraries(server dc)

add_executable(client client.c)
target_link_libraries(client dc)