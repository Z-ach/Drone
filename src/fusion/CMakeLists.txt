cmake_minimum_required(VERSION 3.5)
project(fusion C)

file(GLOB_RECURSE files "*.c")

add_library(fusion SHARED ${files})

target_include_directories(fusion PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(UNIX AND NOT APPLE)
    target_link_libraries(fusion m) # link math library for Linux
endif()
