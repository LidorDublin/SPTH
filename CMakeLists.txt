cmake_minimum_required(VERSION 3.12)

project(SPTH)

# Notify about everything, Set standard to C++17, include curlpp & curl
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++17 -lcurlpp -lcurl")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(shortest-path-to-hitler main.cpp web_utils/web_utils.cpp web_utils/web_utils.h cache_utils/cache_utils.h cache_utils/cache_utils.cpp wikiPage/wikiPage.cpp wikiPage/wikiPage.h)

# Prevent JSON tests from running
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

target_link_libraries(shortest-path-to-hitler PRIVATE curlpp nlohmann_json::nlohmann_json)
