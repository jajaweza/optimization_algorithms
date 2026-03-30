cmake_minimum_required(VERSION 4.0)

project(aco)
add_executable(aco)
set_property(TARGET aco PROPERTY CXX_STANDARD 20)
if(DEBUG)
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_definitions(NDEBUG=0)
    set(CMAKE_BUILD_TYPE Debug)

else()
    add_compile_definitions(NDEBUG=1)
endif()

include_directories(inc)
target_sources(aco PUBLIC src/graph.cpp src/main.cpp)