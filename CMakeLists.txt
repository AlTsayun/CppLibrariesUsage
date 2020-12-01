cmake_minimum_required(VERSION 3.17)
project(lab3)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 17)

add_executable(App main.cpp)


set(LIBRARY_SOURCE_FILES
        memory_replace.cpp)

add_library(memory_replace_static_lib
        STATIC
        ${LIBRARY_SOURCE_FILES})

add_library(memory_replace_dynamic_lib
        SHARED
        ${LIBRARY_SOURCE_FILES})

target_link_libraries(App memory_replace_static_lib)