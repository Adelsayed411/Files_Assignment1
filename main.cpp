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

    // Test updating a doctor's record
    cout << "\nUpdating doctor records:\n";
    try {
        updateDoctorRecord("D101", "Dr. Allen", "404 Birch St");
        cout << "Doctor D101 updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating doctor D101: " << e.what() << "\n";
    }

    try {
        updateDoctorRecord("D105", "Dr. Evans", "909 Cedar Lane");
        cout << "Doctor D105 updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating doctor D105: " << e.what() << "\n";
    }

    // Test updating an appointment's record
    cout << "\nUpdating appointment records:\n";
    try {
        updateAppointmentRecord("A101", "2025-01-01");
        cout << "Appointment A101 updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating appointment A101: " << e.what() << "\n";
    }

    try {
        updateAppointmentRecord("A106", "2025-02-01");
        cout << "Appointment A106 updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating appointment A106: " << e.what() << "\n";
    }

    // Test searching for updated records
    cout << "\nSearching for updated doctor by ID:\n";
    try {
        Doctor foundDoc = searchDoctorByID("D101");
        cout << "Doctor ID: " << foundDoc.doctorID << "\n"
                  << "Name: " << foundDoc.doctorName << "\n"
                  << "Address: " << foundDoc.address << "\n";
    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << "\n";
    }

    // Search for the updated appointment
    cout << "\nSearching for updated appointment by ID:\n";
    try {
        Appointment foundApp = searchAppointmentByID("A101");
        cout << "Appointment ID: " << foundApp.appointmentID << "\n"
                  << "Date: " << foundApp.appointmentDate << "\n"
                  << "Doctor ID: " << foundApp.doctorID << "\n";
    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << "\n";
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

    // Test updating a doctor's record
    cout << "\nUpdating doctor records:\n";
    try {
        updateDoctorRecord("D101", "Dr. Allen", "404 Birch St");
        cout << "Doctor D101 updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating doctor D101: " << e.what() << "\n";
    }

    try {
        updateDoctorRecord("D105", "Dr. Evans", "909 Cedar Lane");
        cout << "Doctor D105 updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating doctor D105: " << e.what() << "\n";
    }

    // Test updating an appointment's record
    cout << "\nUpdating appointment records:\n";
    try {
        updateAppointmentRecord("A101", "2025-01-01");
        cout << "Appointment A101 updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating appointment A101: " << e.what() << "\n";
    }

    try {
        updateAppointmentRecord("A106", "2025-02-01");
        cout << "Appointment A106 updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating appointment A106: " << e.what() << "\n";
    }

    // Test searching for updated records
    cout << "\nSearching for updated doctor by ID:\n";
    try {
        Doctor foundDoc = searchDoctorByID("D101");
        cout << "Doctor ID: " << foundDoc.doctorID << "\n"
                  << "Name: " << foundDoc.doctorName << "\n"
                  << "Address: " << foundDoc.address << "\n";
    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << "\n";
    }

    // Search for the updated appointment
    cout << "\nSearching for updated appointment by ID:\n";
    try {
        Appointment foundApp = searchAppointmentByID("A101");
        cout << "Appointment ID: " << foundApp.appointmentID << "\n"
                  << "Date: " << foundApp.appointmentDate << "\n"
                  << "Doctor ID: " << foundApp.doctorID << "\n";
    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << "\n";
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
