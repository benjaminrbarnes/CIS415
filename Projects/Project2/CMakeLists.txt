cmake_minimum_required(VERSION 3.1)
project(Project2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Project2Start64bit/BoundedBuffer.h
    Project2Start64bit/BoundedBuffer.o
    Project2Start64bit/demo
    Project2Start64bit/destination.h
    Project2Start64bit/diagnostics.h
    Project2Start64bit/diagnostics.o
    Project2Start64bit/fakeapplications.o
    Project2Start64bit/freepacketdescriptorstore.h
    Project2Start64bit/freepacketdescriptorstore.o
    Project2Start64bit/freepacketdescriptorstore__full.h
    Project2Start64bit/generic_queue.o
    Project2Start64bit/log
    Project2Start64bit/Makefile
    Project2Start64bit/networkdevice.h
    Project2Start64bit/networkdevice.o
    Project2Start64bit/networkdriver.h
    Project2Start64bit/packetdescriptor.h
    Project2Start64bit/packetdescriptor.o
    Project2Start64bit/packetdescriptorcreator.h
    Project2Start64bit/packetdescriptorcreator.o
    Project2Start64bit/pid.h
    Project2Start64bit/testharness.o)

add_executable(Project2 ${SOURCE_FILES})