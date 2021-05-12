cmake_minimum_required(VERSION 3.17)
project(Lab5)

set(CMAKE_CXX_STANDARD 14)

find_package (Threads)
add_executable (Lab5 Lab5.cpp)
target_link_libraries (Lab5 ${CMAKE_THREAD_LIBS_INIT})