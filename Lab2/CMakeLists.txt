cmake_minimum_required(VERSION 3.17)
project(Lab2)

set(CMAKE_CXX_STANDARD 14)

set(STATIC_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Number)
set(SHARED_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Vector)

add_subdirectory(Number)
add_subdirectory(Vector)

include_directories(${STATIC_LIB_PATH})
include_directories(${SHARED_LIB_PATH})

add_executable(Main main.cpp)

target_link_libraries(Main PRIVATE Number)
target_link_libraries(Vector PRIVATE Number)
target_link_libraries(Main PRIVATE Vector)
