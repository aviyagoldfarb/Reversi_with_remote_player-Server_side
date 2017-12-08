cmake_minimum_required(VERSION 3.8)
project(ex4_server)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp Server.cpp Server.h Point.cpp Point.h)
add_executable(ex4_server ${SOURCE_FILES})