cmake_minimum_required(VERSION 3.1)
project(Project1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    child_handler.c
    p1fxns.c
    p1fxns.h
    setitimer.c
    signal.c)

add_executable(Project1 ${SOURCE_FILES})