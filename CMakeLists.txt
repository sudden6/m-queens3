cmake_minimum_required(VERSION 3.5)

project(m-queens3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(cxxopts)
add_subdirectory(presolver)

add_executable(m-queens3 main.cpp)
