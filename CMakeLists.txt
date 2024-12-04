cmake_minimum_required(VERSION 3.24.1)
project(2sm mo7taram)

set(CMAKE_CXX_STANDARD 17)

add_executable(2sm mo7taram main.cpp
        Doctor.h
        Appointment.h
        FileManager.h
        FileManager.cpp)