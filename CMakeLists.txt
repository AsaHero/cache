cmake_minimum_required(VERSION 3.20)
project(cache)

set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(Crow REQUIRED)
find_package(fmt REQUIRED)

add_subdirectory(src)
