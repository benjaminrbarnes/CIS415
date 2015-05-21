cmake_minimum_required(VERSION 3.1)
project(CIS415)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Project0/finddupl
    Project0/finddupl.c
    Project0/finddupl.o
    Project0/jeff.out
    Project0/M.out
    Project0/M.txt
    Project0/Makefile
    Project0/MakefilePrechange.txt
    Project0/mentry.c
    Project0/mentry.h
    Project0/mentry.o
    Project0/mlist.c
    Project0/mlist.h
    Project0/mlist.o
    Project0/mlistLL.c
    Project0/S.out
    Project0/S.txt)

add_executable(CIS415 ${SOURCE_FILES})