cmake_minimum_required(VERSION 3.17)
project(Vector)

set(CMAKE_CXX_STANDARD 14)
set(VECTOR_SRC Vector.cpp Vector.h)

add_library(Vector SHARED ${VECTOR_SRC})