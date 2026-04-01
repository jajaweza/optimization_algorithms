cmake_minimum_required(VERSION 4.0)

project(aco)
add_executable(aco)
set_property(TARGET aco PROPERTY CXX_STANDARD 20)

if(DEBUG)
    message("Compiling with extra debug flags")
    set(CMAKE_CXX_CLANG_TIDY
        clang-tidy;
        -checks=bugprone-*,performance-*,readability-*
    )
    add_compile_options(-Wall -Wextra -Wpedantic -Wunused_ret)
    add_compile_definitions(NDEBUG=0)
    set(CMAKE_BUILD_TYPE Debug)

else()
    add_compile_definitions(NDEBUG=1)
endif()

include_directories(inc)
target_sources(aco PUBLIC src/graph.cpp src/main.cpp src/utils.cpp src/ant_colony.cpp)