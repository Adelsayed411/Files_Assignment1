cmake_minimum_required(VERSION 3.24)
project(assign1)

set(CMAKE_CXX_STANDARD 17)
include_directories(.)

add_executable(assign1
        Appointment.h
        Doctor.h
        FileManager.cpp
        FileManager.h
        main.cpp IndexManager.h IndexManager.cpp)
