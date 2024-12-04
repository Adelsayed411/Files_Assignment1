cmake_minimum_required(VERSION 3.27)
project(Files_Assignment1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source files for the project
set(SOURCE_FILES
    main.cpp
    FileManager.cpp
    IndexManager.cpp
)

# Add the header files for clarity (not strictly required by CMake, but good practice)
set(HEADER_FILES
    Appointment.h
    Doctor.h
    FileManager.h
    IndexManager.h
)

# Create the executable
add_executable(Files_Assignment1 ${SOURCE_FILES} ${HEADER_FILES})

# Include directories for headers (if needed)
include_directories(${CMAKE_SOURCE_DIR})

# Optional: Add warnings for better code quality checks
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(Files_Assignment1 PRIVATE -Wall -Wextra -pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(Files_Assignment1 PRIVATE /W4)
endif
