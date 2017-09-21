cmake_minimum_required(VERSION 3.8)
project(threadpool)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp ThreadPool.cpp ThreadPool.h)
add_executable(main ${SOURCE_FILES})