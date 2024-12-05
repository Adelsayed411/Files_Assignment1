cmake_minimum_required(VERSION 3.27)
project(untitled25)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled25 main.cpp
        Doctor.h
        Appointment.h
        FileManager.h
        FileManager.cpp
        IndexManager.cpp
        IndexManager.h)
