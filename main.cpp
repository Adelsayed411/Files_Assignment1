#include <iostream>
#include "FileManager.h"  // Ensure this file exists and is in the same directory or in the include path

int main() {
    // Sample code to test if FileManager is included and working
    std::cout << "Welcome to the File Management System!" << std::endl;

    // Example usage of a function from FileManager.h (you'll need to implement or modify these functions in your FileManager.cpp)
    // FileManager fileManager;
    fileManager.createIndex("DoctorsPrimaryIndex.dat");

    std::cout << "Program finished running successfully." << std::endl;
    return 0;
}
