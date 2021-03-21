cmake_minimum_required(VERSION 3.17)
project(Number)

set(CMAKE_CXX_STANDARD 14)
set(NUMBER_SRC Number.h Number.cpp)

add_library(Number STATIC ${NUMBER_SRC})
