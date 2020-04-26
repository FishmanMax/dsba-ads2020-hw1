cmake_minimum_required(VERSION 3.14)
project(Fishman)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /-O3")

add_executable(Fishman main.cpp header.h)