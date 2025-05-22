cmake_minimum_required(VERSION 3.25)
project(COSE341 C)

set(CMAKE_C_STANDARD 11)

include_directories(include include/DataStructure include/Component include/Scheduler)

add_subdirectory(src)
