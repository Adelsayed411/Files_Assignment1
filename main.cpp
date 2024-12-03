#include <iostream>
#include "FileManager.h"

int main() {
    openFiles();


    Doctor doc = {"D002", "Dr. Smith", "123 Main St"};
    writeDoctorRecord(doc);


    Appointment app = {"A001", "2024-12-10", "D001"};
    writeAppointmentRecord(app);


    Doctor readDoc = readDoctorRecord(0);
    std::cout << "Doctor ID: " << readDoc.doctorID << "\n"
              << "Name: " << readDoc.doctorName << "\n"
              << "Address: " << readDoc.address << "\n";


    closeFiles();
    return 0;
}
