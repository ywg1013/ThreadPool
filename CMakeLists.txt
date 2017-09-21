cmake_minimum_required(VERSION 3.8)
project(hello)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp ThreadPool.cpp ThreadPool.h)
add_executable(hello ${SOURCE_FILES})