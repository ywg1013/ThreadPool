cmake_minimum_required(VERSION 3.8)
project(threadpool)

set(CMAKE_CXX_STANDARD 11)

ADD_SUBDIRECTORY(threadlib)

include_directories(threadlib)

aux_source_directory(. SOURCE_FILES)

add_executable(main ${SOURCE_FILES})

target_link_libraries(main threadlib)