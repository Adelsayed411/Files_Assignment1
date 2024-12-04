#include "FileManager.h"
#include "IndexManager.h"
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

fstream doctorsFile, appointmentsFile;
IndexManager indexManager;  // Use the IndexManager class for index management

void openFiles() {
    // Open the doctors file
    doctorsFile.open("doctors.dat", ios::in | ios::out | ios::binary);
    if (!doctorsFile) {
        doctorsFile.open("doctors.dat", ios::out);
        doctorsFile.close();
        doctorsFile.open("doctors.dat", ios::in | ios::out | ios::binary);
    }

    // Open the appointments file
    appointmentsFile.open("appointments.dat", ios::in | ios::out | ios::binary);
    if (!appointmentsFile) {
        appointmentsFile.open("appointments.dat", ios::out);
        appointmentsFile.close();
        appointmentsFile.open("appointments.dat", ios::in | ios::out | ios::binary);
    }

    // Create indices
    indexManager.createDoctorsPrimaryIndex("doctors_index.dat");
    indexManager.createAppointmentsPrimaryIndex("appointments_index.dat");
}

void closeFiles() {
    // Close doctors and appointments files
    if (doctorsFile.is_open()) doctorsFile.close();
    if (appointmentsFile.is_open()) appointmentsFile.close();

    // Save indices
    indexManager.writeIndexToFile("doctors_index.dat", indexManager.getDoctorsPrimaryIndex());
    indexManager.writeIndexToFile("appointments_index.dat", indexManager.getAppointmentsPrimaryIndex());
}

void writeDoctorRecord(const Doctor& doctor) {
    // Check if doctor already exists in the index
    streampos position = indexManager.searchPrimaryIndex("doctors.dat", doctor.doctorID);
    if (position != -1) {
        cout << "Error: Doctor with ID " << doctor.doctorID << " already exists.\n";
        return;
    }

    // Prepare the doctor record to write
    string record = string(doctor.doctorID) + "|" +
                    string(doctor.doctorName) + "|" +
                    string(doctor.address) + "\n";

    // Write the record to the doctors file
    doctorsFile.seekp(0, ios::end);
    position = doctorsFile.tellp();  // Get the current position of the file pointer
    doctorsFile.write(record.c_str(), record.size());

    // Add the doctor to the primary index
    indexManager.addPrimaryIndexEntry("doctors.dat", doctor.doctorID, position);
}

Doctor readDoctorRecord(const string& doctorID) {
    Doctor doctor;

    // Search for the doctor in the primary index
    streampos position = indexManager.searchPrimaryIndex("doctors.dat", doctorID);
    if (position == -1) {
        cout << "Error: Doctor with ID " << doctorID << " not found.\n";
        return doctor;
    }

    // Read the doctor record from the file
    doctorsFile.seekg(position);
    string record;
    getline(doctorsFile, record);

    // Parse the record
    size_t pos1 = record.find('|');
    size_t pos2 = record.find('|', pos1 + 1);

    strncpy(doctor.doctorID, record.substr(0, pos1).c_str(), 15);
    strncpy(doctor.doctorName, record.substr(pos1 + 1, pos2 - pos1 - 1).c_str(), 30);
    strncpy(doctor.address, record.substr(pos2 + 1).c_str(), 30);

    return doctor;
}

void deleteDoctorRecord(const string& doctorID) {
    // Search for the doctor in the primary index
    streampos position = indexManager.searchPrimaryIndex("doctors.dat", doctorID);
    if (position == -1) {
        cout << "Error: Doctor with ID " << doctorID << " not found.\n";
        return;
    }

    // Mark the record as deleted (for simplicity, just overwrite with *)
    doctorsFile.seekp(position);
    doctorsFile.write("*", 1);  // Write a marker to indicate deletion

    // Remove the doctor from the primary index
    indexManager.deleteIndexEntry("doctors.dat", doctorID);

    cout << "Doctor with ID " << doctorID << " has been deleted.\n";
}

// Add index creation function
void FileManager::createIndex(const std::string& filename) {
    if (filename == "doctors.dat") {
        // Initialize doctors index if it is not already created
        indexManager.createDoctorsPrimaryIndex("doctors_index.dat");
    } else if (filename == "appointments.dat") {
        // Initialize appointments index if it is not already created
        indexManager.createAppointmentsPrimaryIndex("appointments_index.dat");
    }
}
