#include <iostream>
#include "FileManager.h"
#include <vector>

int main() {
    // Open files for reading and writing
    openFiles();

    // Create and add multiple doctors
    Doctor doc1 = {"D001", "Dr. Smith", "123 Main St"};
    Doctor doc2 = {"D002", "Dr. Brown", "456 Elm St"};
    Doctor doc3 = {"D003", "Dr. Johnson", "789 Pine St"};
    Doctor doc4 = {"D004", "Dr. Lee", "101 Maple St"};
    Doctor doc5 = {"D005", "Dr. White", "202 Oak St"};

    writeDoctorRecord(doc1);
    writeDoctorRecord(doc2);
    writeDoctorRecord(doc3);
    writeDoctorRecord(doc4);
    writeDoctorRecord(doc5);

    // Create and add multiple appointments
    Appointment app1 = {"A001", "2024-12-10", "D001"};
    Appointment app2 = {"A002", "2024-12-15", "D002"};
    Appointment app3 = {"A003", "2024-12-20", "D003"};
    Appointment app4 = {"A004", "2024-12-25", "D001"};
    Appointment app5 = {"A005", "2024-12-30", "D004"};
    Appointment app6 = {"A006", "2025-01-05", "D005"};

    writeAppointmentRecord(app1);
    writeAppointmentRecord(app2);
    writeAppointmentRecord(app3);
    writeAppointmentRecord(app4);
    writeAppointmentRecord(app5);
    writeAppointmentRecord(app6);

    // Test search for a doctor by ID
    std::cout << "\nSearching for doctors by ID:\n";
    for (const auto& doctorID : {"D001", "D002", "D003", "D004", "D005", "D006"}) {
        try {
            Doctor foundDoc = searchDoctorByID(doctorID);
            std::cout << "Doctor ID: " << foundDoc.doctorID << "\n"
                      << "Name: " << foundDoc.doctorName << "\n"
                      << "Address: " << foundDoc.address << "\n";
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << " for Doctor ID " << doctorID << "\n";
        }
    }

    // Test search for an appointment by ID
    std::cout << "\nSearching for appointments by ID:\n";
    for (const auto& appointmentID : {"A001", "A002", "A003", "A004", "A005", "A006", "A007"}) {
        try {
            Appointment foundApp = searchAppointmentByID(appointmentID);
            std::cout << "Appointment ID: " << foundApp.appointmentID << "\n"
                      << "Date: " << foundApp.appointmentDate << "\n"
                      << "Doctor ID: " << foundApp.doctorID << "\n";
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << " for Appointment ID " << appointmentID << "\n";
        }
    }

    // Test search for doctors by name
    std::cout << "\nSearching for doctors by name:\n";
    for (const auto& doctorName : {"Dr. Smith", "Dr. Brown", "Dr. Johnson", "Dr. Lee", "Dr. White", "Dr. Black"}) {
        try {
            std::vector<std::streampos> positions = searchDoctorByName(doctorName);
            if (!positions.empty()) {
                std::cout << "Doctors with name " << doctorName << ":\n";
                for (const auto& pos : positions) {
                    Doctor doc = readDoctorRecord(pos);
                    std::cout << "Doctor ID: " << doc.doctorID << "\n"
                              << "Name: " << doc.doctorName << "\n"
                              << "Address: " << doc.address << "\n";
                }
            } else {
                std::cout << "No doctors found with name " << doctorName << "\n";
            }
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << " for Doctor Name " << doctorName << "\n";
        }
    }

    // Test search for appointments by doctor ID
    std::cout << "\nSearching for appointments by doctor ID:\n";
    for (const auto& doctorID : {"D001", "D002", "D003", "D004", "D005", "D006"}) {
        try {
            std::vector<std::streampos> positions = searchAppointmentsByDoctorID(doctorID);
            if (!positions.empty()) {
                std::cout << "Appointments for Doctor ID " << doctorID << ":\n";
                for (const auto& pos : positions) {
                    Appointment app = readAppointmentRecord(pos);
                    std::cout << "Appointment ID: " << app.appointmentID << "\n"
                              << "Date: " << app.appointmentDate << "\n"
                              << "Doctor ID: " << app.doctorID << "\n";
                }
            } else {
                std::cout << "No appointments found for Doctor ID " << doctorID << "\n";
            }
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << " for Doctor ID " << doctorID << "\n";
        }
    }

    // Close the files after operations
    closeFiles();
    return 0;
}
