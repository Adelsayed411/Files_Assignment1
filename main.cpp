#include <iostream>
#include "FileManager.h"
#include <vector>
#include <stdexcept>

int main() {
    // Open files for reading and writing
    openFiles();

    // Create and add new doctor records
    Doctor doctors[] = {
            {"D101", "Dr. Adams", "123 Elm St"},
            {"D102", "Dr. Baker", "456 Oak St"},
            {"D103", "Dr. Carter", "789 Pine St"},
            {"D104", "Dr. Davis", "101 Maple St"},
            {"D105", "Dr. Evans", "202 Cedar St"}
    };
    for (const auto& doc : doctors) {
        writeDoctorRecord(doc);
    }

    // Create and add new appointment records
    Appointment appointments[] = {
            {"A101", "2024-12-10", "D101"},
            {"A102", "2024-12-12", "D102"},
            {"A103", "2024-12-14", "D103"},
            {"A104", "2024-12-16", "D101"},
            {"A105", "2024-12-18", "D104"},
            {"A106", "2024-12-20", "D105"}
    };
    for (const auto& app : appointments) {
        writeAppointmentRecord(app);
    }

    // Test searching for doctors by ID
    std::cout << "\nSearching for doctors by ID:\n";
    for (const auto& doctorID : {"D101", "D102", "D103", "D104", "D105", "D106"}) {
        try {
            Doctor foundDoc = searchDoctorByID(doctorID);
            std::cout << "Doctor ID: " << foundDoc.doctorID << "\n"
                      << "Name: " << foundDoc.doctorName << "\n"
                      << "Address: " << foundDoc.address << "\n";
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << " for Doctor ID " << doctorID << "\n";
        }
    }

    // Test searching for appointments by ID
    std::cout << "\nSearching for appointments by ID:\n";
    for (const auto& appointmentID : {"A101", "A102", "A103", "A104", "A105", "A106", "A107"}) {
        try {
            Appointment foundApp = searchAppointmentByID(appointmentID);
            std::cout << "Appointment ID: " << foundApp.appointmentID << "\n"
                      << "Date: " << foundApp.appointmentDate << "\n"
                      << "Doctor ID: " << foundApp.doctorID << "\n";
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << " for Appointment ID " << appointmentID << "\n";
        }
    }

    // Test searching for doctors by name
    std::cout << "\nSearching for doctors by name:\n";
    for (const auto& doctorName : {"Dr. Adams", "Dr. Baker", "Dr. Carter", "Dr. Davis", "Dr. Evans", "Dr. Foster"}) {
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

    // Test searching for appointments by doctor ID
    std::cout << "\nSearching for appointments by doctor ID:\n";
    for (const auto& doctorID : {"D101", "D102", "D103", "D104", "D105", "D106"}) {
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

    // Test deleting multiple doctor and appointment records
    std::cout << "\nDeleting multiple doctor and appointment records:\n";
    try {
        deleteDoctorRecord("D101");
        std::cout << "Doctor D101 deleted successfully.\n";
        deleteDoctorRecord("D103");
        std::cout << "Doctor D103 deleted successfully.\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Error deleting doctor: " << e.what() << "\n";
    }

    try {
        deleteAppointmentRecord("A102");
        std::cout << "Appointment A102 deleted successfully.\n";
        deleteAppointmentRecord("A105");
        std::cout << "Appointment A105 deleted successfully.\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Error deleting appointment: " << e.what() << "\n";
    }

    // Verify if the deleted records are removed and added to avail list
    std::cout << "\nVerifying the avail list content:\n";
    std::ifstream availListFile("doctorAvailList.txt");
    if (availListFile.is_open()) {
        std::string line;
        while (getline(availListFile, line)) {
            std::cout << "Doctor Avail List Entry: " << line << "\n";
        }
        availListFile.close();
    } else {
        std::cout << "Unable to open doctor avail list file for verification.\n";
    }

    availListFile.open("appointmentAvailList.txt");
    if (availListFile.is_open()) {
        std::string line;
        while (getline(availListFile, line)) {
            std::cout << "Appointment Avail List Entry: " << line << "\n";
        }
        availListFile.close();
    } else {
        std::cout << "Unable to open appointment avail list file for verification.\n";
    }

    // Close the files after operations
    closeFiles();
    return 0;
}
