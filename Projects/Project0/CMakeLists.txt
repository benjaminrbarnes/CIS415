cmake_minimum_required(VERSION 3.1)
project(Project0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    finddupl
    finddupl.c
    finddupl.o
    M.out
    M.txt
    Makefile
    mentry.c
    mentry.h
    mentry.o
    mlist.c
    mlist.h
    mlist.o
    mlistLL.c
    S.out
    S.txt)

add_executable(Project0 ${SOURCE_FILES})