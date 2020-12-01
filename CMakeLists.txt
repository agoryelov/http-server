cmake_minimum_required(VERSION 3.17)
project(COMP4981_A3 C)


set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_library(str_map STATIC ./libs/str_map.c)
target_compile_options(str_map PRIVATE -Wpedantic -Wall -Wextra)

add_library(thread_pool STATIC ./http_protocol/thread_pool.c)
target_link_libraries(thread_pool http)
target_compile_options(thread_pool PRIVATE -Wpedantic -Wall -Wextra)

add_library(process_pool STATIC ./http_protocol/process_pool.c)
target_link_libraries(process_pool http)
target_compile_options(process_pool PRIVATE -Wpedantic -Wall -Wextra)

add_library(http STATIC ./http_protocol/http.c)
target_link_libraries(http str_map)
target_compile_options(http PRIVATE -Wpedantic -Wall -Wextra)

add_library(http_config STATIC ./http_protocol/config.c)
target_link_libraries(http_config config)
target_compile_options(http_config PRIVATE -Wpedantic -Wall -Wextra)

add_executable(server server.c)
target_link_libraries(server http http_config str_map pthread thread_pool process_pool rt)
target_compile_options(server PRIVATE -Wpedantic -Wall -Wextra -g --coverage)


add_library(settings_form STATIC ncurses/ncurses_form.c)
target_link_libraries(settings_form form ncurses settings_menu settings_shared)
target_compile_options(settings_form PRIVATE -Wpedantic -Wall -Wextra)

add_library(settings_menu STATIC ncurses/ncurses_menu.c)
target_link_libraries(settings_menu menu ncurses settings_form settings_shared)
target_compile_options(settings_menu PRIVATE -Wpedantic -Wall -Wextra)

add_library(settings_panel STATIC ncurses/ncurses_panel.c)
target_link_libraries(settings_panel panel ncurses settings_shared)
target_compile_options(settings_panel PRIVATE -Wpedantic -Wall -Wextra)

add_library(settings_shared STATIC ncurses/ncurses_shared.c)
target_compile_options(settings_shared PRIVATE -Wpedantic -Wall -Wextra)

add_executable(settings ncurses/ncurses.c)
target_link_libraries(settings form menu panel ncurses config settings_form settings_menu settings_panel settings_shared)
target_compile_options(settings PRIVATE -Wpedantic -Wall -Wextra)
