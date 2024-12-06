#include <iostream>
#include "FileManager.h"
#include <string>
#include <vector>
#include <stdexcept>

void displayMenu() {
    cout << "\nWelcome to the Doctor and Appointment Management System\n";
    cout << "========================================\n";
    cout << "1. Add New Doctor\n";
    cout << "2. Add New Appointment\n";
    cout << "3. Update Doctor Name (Doctor ID)\n";
    cout << "4. Update Appointment Date (Appointment ID)\n";
    cout << "5. Delete Appointment (Appointment ID)\n";
    cout << "6. Delete Doctor (Doctor ID)\n";
    cout << "7. Print Doctor Info (Doctor ID)\n";
    cout << "8. Print Appointment Info (Appointment ID)\n";
    cout << "9. Test Queries\n";
    cout << "10. Exit\n";
    cout << "========================================\n";
    cout << "Please select an option: ";
}

void handleAddDoctor() {
    Doctor newDoctor;
    cout << "Enter Doctor ID: ";
    cin >> newDoctor.doctorID;
    cin.ignore();
    cout << "Enter Doctor Name: ";
    cin >> newDoctor.doctorName;
    cin.ignore();
    cout << "Enter Doctor Address: ";
    cin >> newDoctor.address;
    cin.ignore();

    addDoctorRecord(newDoctor);
    cout << "Doctor added successfully.\n";
}

void handleAddAppointment() {
    Appointment newAppointment;
    cout << "Enter Appointment ID: ";
    cin >> newAppointment.appointmentID;
    cin.ignore();
    cout << "Enter Appointment Date: ";
    cin >> newAppointment.appointmentDate;
    cin.ignore();
    cout << "Enter Doctor ID: ";
    cin >> newAppointment.doctorID;
    cin.ignore();

    addAppointmentRecord(newAppointment);
    cout << "Appointment added successfully.\n";
}

void handleUpdateDoctor() {
    string doctorID, newDoctorName, newAddress;
    cout << "Enter Doctor ID: ";
    cin >> doctorID;
    cin.ignore();
    cout << "Enter New Doctor Name: ";
    cin >> newDoctorName;
    cin.ignore();
    cout << "Enter New Address: ";
    cin >> newAddress;
    cin.ignore();

    try {
        updateDoctorRecord(doctorID, newDoctorName, newAddress);
        cout << "Doctor updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating doctor: " << e.what() << "\n";
    }
}

void handleUpdateAppointment() {
    string appointmentID, newDate;
    cout << "Enter Appointment ID: ";
    cin >> appointmentID;
    cin.ignore();
    cout << "Enter New Appointment Date: ";
    cin >> newDate;
    cin.ignore();

    try {
        updateAppointmentRecord(appointmentID, newDate);
        cout << "Appointment updated successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error updating appointment: " << e.what() << "\n";
    }
}

void handleDeleteDoctor() {
    string doctorID;
    cout << "Enter Doctor ID: ";
    cin >> doctorID;
    cin.ignore();

    try {
        deleteDoctorRecord(doctorID);
        cout << "Doctor deleted successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error deleting doctor: " << e.what() << "\n";
    }
}

void handleDeleteAppointment() {
    string appointmentID;
    cout << "Enter Appointment ID: ";
    cin >> appointmentID;
    cin.ignore();

    try {
        deleteAppointmentRecord(appointmentID);
        cout << "Appointment deleted successfully.\n";
    } catch (const runtime_error& e) {
        cout << "Error deleting appointment: " << e.what() << "\n";
    }
}

void handlePrintDoctorInfo() {
    string doctorID;
    cout << "Enter Doctor ID: ";
    cin >> doctorID;
    cin.ignore();

    try {
        Doctor doctor = searchDoctorByID(doctorID);
        cout << "Doctor ID: " << doctor.doctorID << "\n"
                  << "Name: " << doctor.doctorName << "\n"
                  << "Address: " << doctor.address << "\n";
    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void handlePrintAppointmentInfo() {
    string appointmentID;
    cout << "Enter Appointment ID: ";
    cin >> appointmentID;
    cin.ignore();

    try {
        Appointment appointment = searchAppointmentByID(appointmentID);
        cout << "Appointment ID: " << appointment.appointmentID << "\n"
                  << "Date: " << appointment.appointmentDate << "\n"
                  << "Doctor ID: " << appointment.doctorID << "\n";
    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void handleTestQueries() {
    cout << "\nTesting predefined queries:\n";

    // Query to select all from Doctors where Doctor ID='D101'
    string query1 = "SELECT ALL FROM DOCTORS WHERE DOCTOR ID='D101';";
    cout << "\nQuery: " << query1 << "\n";
    processQuery(query1);

    // Query to select all from Appointments where Doctor ID='D101'
    string query2 = "SELECT ALL FROM APPOINTMENTS WHERE DOCTOR ID='D101';";
    cout << "\nQuery: " << query2 << "\n";
    processQuery(query2);

    // Query to select Doctor Name from Doctors where Doctor ID='D102'
    string query3 = "SELECT DOCTOR NAME FROM DOCTORS WHERE DOCTOR ID='D102';";
    cout << "\nQuery: " << query3 << "\n";
    processQuery(query3);
}

int main() {
    openFiles();
    int choice;

    do {
        displayMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: handleAddDoctor(); break;
            case 2: handleAddAppointment(); break;
            case 3: handleUpdateDoctor(); break;
            case 4: handleUpdateAppointment(); break;
            case 5: handleDeleteAppointment(); break;
            case 6: handleDeleteDoctor(); break;
            case 7: handlePrintDoctorInfo(); break;
            case 8: handlePrintAppointmentInfo(); break;
            case 9: handleTestQueries(); break;
            case 10: cout << "Exiting the program.\n"; break;
            default: cout << "Invalid choice. Please try again.\n"; break;
        }
    } while (choice != 10);

    closeFiles();
    return 0;
}