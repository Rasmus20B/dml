cmake_minimum_required(VERSION 3.13)  
project(dml)               
set(CMAKE_CXX_STANDARD 23)            
set(CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add main.cpp file of project root directory as source file

add_subdirectory(src)
add_subdirectory(test)
include(FetchContent)


