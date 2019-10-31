cmake_minimum_required(VERSION 3.12)

project(SPTH)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(shortest-path-to-hitler main.cpp)

