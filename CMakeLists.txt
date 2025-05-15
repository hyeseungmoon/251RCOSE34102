cmake_minimum_required(VERSION 3.25)
project(COSE341 C)

set(CMAKE_C_STANDARD 11)

include_directories(include include/DataStructure)

add_subdirectory(src/DataStructure)
add_subdirectory(src)
