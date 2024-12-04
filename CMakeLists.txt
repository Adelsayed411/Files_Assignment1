cmake_minimum_required(VERSION 3.24.2)
project(files)

set(CMAKE_CXX_STANDARD 14)

include_directories(Files_Assignment1)

add_executable(files
        Files_Assignment1/Appointment.h
        Files_Assignment1/Doctor.h
        Files_Assignment1/FileManager.cpp
        Files_Assignment1/FileManager.h
        Files_Assignment1/IndexManager.cpp
        Files_Assignment1/IndexManager.h
        Files_Assignment1/main.cpp)