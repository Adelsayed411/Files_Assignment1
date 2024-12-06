#include <iostream>
#include "FileManager.h"
#include <vector>
#include <stdexcept>

int main() {
    // Open files for reading and writing
    openFiles();

    // Create and add new doctor records
    Doctor doctors[] = {
            {"D101", "Dr. jana", "123 Elm St"},
            {"D102", "Dr. Baker", "456 Oak St"},
            {"D103", "Dr. Carter", "789 Pine St"},
            {"D104", "Dr. Davis", "101 Maple St"},
            {"D105", "Dr. alia", "202 Cedar St"}
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

    // Test deleting multiple doctor and appointment records
    cout << "\nDeleting multiple doctor and appointment records:\n";
    try {
        deleteDoctorRecord("D102");
        cout << "Doctor D101 deleted successfully.\n";
        deleteDoctorRecord("D105");
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

    // Add new doctors and appointments after deletion to ensure best-fit strategy
    cout << "\nAdding records after deletions:\n";
    try {
        Doctor newDoctor1 = {"D106", "Dr. malak", "303 Oakryy St"};
        addDoctorRecord(newDoctor1);
        cout << "Doctor " << newDoctor1.doctorID << " added successfully after deletion.\n";

        Doctor newDoctor2 = {"D107", "Dr. tabark", "707 Cedar St"};
        addDoctorRecord(newDoctor2);
        cout << "Doctor " << newDoctor2.doctorID << " added successfully after deletion.\n";

        Appointment newAppointment1 = {"A107", "2025-01-15", "D102"};
        addAppointmentRecord(newAppointment1);
        cout << "Appointment " << newAppointment1.appointmentID << " added successfully after deletion.\n";

        Appointment newAppointment2 = {"A1088", "2025-01-20", "D105"};
        addAppointmentRecord(newAppointment2);
        cout << "Appointment " << newAppointment2.appointmentID << " added successfully after deletion.\n";
    } catch (const runtime_error& e) {
        cout << "Error adding new record: " << e.what() << "\n";
    }

    // Test updating a doctor's record
    cout << "\nUpdating doctor records:\n";
    try {
        updateDoctorRecord("D103", "Dr. farhaa", "456 onmr St");
        cout << "Doctor D102 updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating doctor D102: " << e.what() << "\n";
    }

    try {
        updateDoctorRecord("D104", "Dr. Foster", "303 cvrt St");
        cout << "Doctor D106 updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating doctor D106: " << e.what() << "\n";
    }

    // Test updating an appointment's record
    cout << "\nUpdating appointment records:\n";
    try {
        updateAppointmentRecord("A104", "2025-01-01");
        cout << "Appointment A104 updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating appointment A104: " << e.what() << "\n";
    }

    try {
        updateAppointmentRecord("A108", "2025-02-01");
        cout << "Appointment A108 updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating appointment A108: " << e.what() << "\n";
    }

    // Verify the content of avail lists
    cout << "\nVerifying the avail list content:\n";
    ifstream availListFile("doctorAvailList.txt");
    if (availListFile.is_open()) {
        string line;
        while (getline(availListFile, line)) {
            cout << "Doctor Avail List Entry: " << line << "\n";
        }
        availListFile.close();
    } else {
        cout << "Unable to open doctor avail list file for verification.\n";
    }

    availListFile.open("appointmentAvailList.txt");
    if (availListFile.is_open()) {
        string line;
        while (getline(availListFile, line)) {
            cout << "Appointment Avail List Entry: " << line << "\n";
        }
        availListFile.close();
    } else {
        cout << "Unable to open appointment avail list file for verification.\n";
    }

    // Close the files after operations
    closeFiles();
    return 0;
}
