cmake_minimum_required(VERSION 3.16)
project(visitor_box)

set(CMAKE_CXX_STANDARD 20)

add_executable(rf_guru rf_guru.cpp)
add_executable(variant_visitor variant_visitor.cpp)
add_executable(animals animals.cpp)

add_subdirectory(pikus)
add_subdirectory(cpp_weekly)