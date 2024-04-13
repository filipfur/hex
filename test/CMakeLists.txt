cmake_minimum_required(VERSION 3.0.0)

project(hex_testing)

include(CTest)
enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(test_hex test_hex.cpp)

target_include_directories(test_hex PRIVATE ../include)

add_test(Test_01, "${CMAKE_CURRENT_SOURCE_DIR}/bin/test_hex" "")