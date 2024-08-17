cmake_minimum_required(VERSION 3.16..3.30.2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(AsciiJuke VERSION "0.1"
                  DESCRIPTION "A simple terminal Jukebox"
                  LANGUAGES C)

add_subdirectory(dependencies)

add_executable(ascii_juke_executable src/main.c)

target_link_libraries(ascii_juke_executable PUBLIC arena_allocator)

target_include_directories(ascii_juke_executable PUBLIC "dependencies/arena-allocator")