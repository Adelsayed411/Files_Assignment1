cmake_minimum_required(VERSION 3.27)
project(untitled27)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled27 main.cpp
        Appointment.h
        Doctor.h
        FileManager.cpp
        FileManager.h)
