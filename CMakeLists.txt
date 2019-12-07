cmake_minimum_required(VERSION 3.10)

project(SPTH)

set(CMAKE_CXX_STANDARD 17)

# Notify about everything, Set standard to C++17, include curlpp & curl
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -Wpedantic -std=c++17 -lcurlpp -lcurl")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(shortest-path-to-hitler main.cpp web_utils/web_utils.cpp web_utils/web_utils.h cache_utils/cache_utils.h cache_utils/cache_utils.cpp wikiPage/wikiPage.h wikiPage/wikiPage.cpp  thread_utils/thread_utils.h exceptions/exceptions.h exceptions/exceptions.cpp config_parser/config_parser.cpp config_parser/config_parser.h)

# Prevent JSON tests from running
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

find_package(Threads REQUIRED)

target_link_libraries(shortest-path-to-hitler PRIVATE curl curlpp nlohmann_json::nlohmann_json ${CMAKE_THREAD_LIBS_INIT})
