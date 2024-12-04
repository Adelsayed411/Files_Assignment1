#include <iostream>
#include "FileManager.h"  // Ensure this file exists and is in the same directory or in the include path

int main() {
    cout << "Welcome to the File Management System!" << endl;

    FileManager fileManager;
    fileManager.createIndex("DoctorsPrimaryIndex.dat");

    cout << "Program finished running successfully." << endl;
    return 0;
}
