cmake_minimum_required(VERSION 3.16)
project(HuffmanForge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(huffman_core
    src/frequency_analyzer.cpp
    src/huffman_tree.cpp
)
target_include_directories(huffman_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
if(MSVC)
    target_compile_options(huffman_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(huffman_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(huffforge src/main.cpp)
target_link_libraries(huffforge PRIVATE huffman_core)

include(CTest)
if(BUILD_TESTING)
    add_executable(huffman_tests tests/huffman_tests.cpp)
    target_link_libraries(huffman_tests PRIVATE huffman_core)
    add_test(NAME huffman_tests COMMAND huffman_tests)
endif()
