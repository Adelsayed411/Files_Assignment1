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
        try {
            addDoctorRecord(doc);
            cout << "Doctor " << doc.doctorID << " added successfully.\n";
        } catch (const runtime_error& e) {
            cout << "Error adding doctor " << doc.doctorID << ": " << e.what() << "\n";
        }
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
        try {
            addAppointmentRecord(app);
            cout << "Appointment " << app.appointmentID << " added successfully.\n";
        } catch (const runtime_error& e) {
            cout << "Error adding appointment " << app.appointmentID << ": " << e.what() << "\n";
        }
    }

    // Test searching for doctors by ID
    cout << "\nSearching for doctors by ID:\n";
    for (const auto& doctorID : {"D101", "D102", "D103", "D104", "D105", "D106"}) {
        try {
            Doctor foundDoc = searchDoctorByID(doctorID);
            cout << "Doctor ID: " << foundDoc.doctorID << "\n"
                      << "Name: " << foundDoc.doctorName << "\n"
                      << "Address: " << foundDoc.address << "\n";
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << " for Doctor ID " << doctorID << "\n";
        }
    }

    // Test searching for appointments by ID
    cout << "\nSearching for appointments by ID:\n";
    for (const auto& appointmentID : {"A101", "A102", "A103", "A104", "A105", "A106", "A107"}) {
        try {
            Appointment foundApp = searchAppointmentByID(appointmentID);
            cout << "Appointment ID: " << foundApp.appointmentID << "\n"
                      << "Date: " << foundApp.appointmentDate << "\n"
                      << "Doctor ID: " << foundApp.doctorID << "\n";
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << " for Appointment ID " << appointmentID << "\n";
        }
    }

    // Test searching for doctors by name
    cout << "\nSearching for doctors by name:\n";
    for (const auto& doctorName : {"Dr. Adams", "Dr. Baker", "Dr. Carter", "Dr. Davis", "Dr. Evans", "Dr. Foster"}) {
        try {
            vector<streampos> positions = searchDoctorByName(doctorName);
            if (!positions.empty()) {
                cout << "Doctors with name " << doctorName << ":\n";
                for (const auto& pos : positions) {
                    Doctor doc = readDoctorRecord(pos);
                    cout << "Doctor ID: " << doc.doctorID << "\n"
                              << "Name: " << doc.doctorName << "\n"
                              << "Address: " << doc.address << "\n";
                }
            } else {
                cout << "No doctors found with name " << doctorName << "\n";
            }
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << " for Doctor Name " << doctorName << "\n";
        }
    }

    // Test searching for appointments by doctor ID
    cout << "\nSearching for appointments by doctor ID:\n";
    for (const auto& doctorID : {"D101", "D102", "D103", "D104", "D105", "D106"}) {
        try {
            vector<streampos> positions = searchAppointmentsByDoctorID(doctorID);
            if (!positions.empty()) {
                cout << "Appointments for Doctor ID " << doctorID << ":\n";
                for (const auto& pos : positions) {
                    Appointment app = readAppointmentRecord(pos);
                    cout << "Appointment ID: " << app.appointmentID << "\n"
                              << "Date: " << app.appointmentDate << "\n"
                              << "Doctor ID: " << app.doctorID << "\n";
                }
            } else {
                cout << "No appointments found for Doctor ID " << doctorID << "\n";
            }
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << " for Doctor ID " << doctorID << "\n";
        }
    }

    // Test deleting multiple doctor and appointment records
    cout << "\nDeleting multiple doctor and appointment records:\n";
    try {
        deleteDoctorRecord("D101");
        cout << "Doctor D101 deleted successfully.\n";
        deleteDoctorRecord("D103");
        cout << "Doctor D103 deleted successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error deleting doctor: " << e.what() << "\n";
    }

    try {
        deleteAppointmentRecord("A102");
        cout << "Appointment A102 deleted successfully.\n";
        deleteAppointmentRecord("A105");
        cout << "Appointment A105 deleted successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error deleting appointment: " << e.what() << "\n";
    }

    Doctor newDoctor = {"D106", "Dr. Foste", "303 Bir St"};
    try {
        addDoctorRecord(newDoctor);
        cout << "\nDoctor " << newDoctor.doctorID << " added successfully after deletion.\n";
    } catch (const runtime_error& e) {
        cout << "Error adding doctor " << newDoctor.doctorID << ": " << e.what() << "\n";
    }

    try {

        deleteDoctorRecord("D104");
        cout << "Doctor D103 deleted successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error deleting doctor: " << e.what() << "\n";
    }

    // Close the files after operations
    closeFiles();
    return 0;
}
