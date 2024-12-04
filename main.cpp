#include <iostream>
#include "FileManager.h"  // Ensure this file exists and is in the same directory or in the include path

int main() {
    // Sample code to test if FileManager is included and working
    cout << "Welcome to the File Management System!" << endl;

    // Example usage of a function from FileManager.h (you'll need to implement or modify these functions in your FileManager.cpp)
    FileManager fileManager;
    fileManager.createIndex("DoctorsPrimaryIndex.dat");

    cout << "Program finished running successfully." << endl;
    return 0;
}
