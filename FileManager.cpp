#include "FileManager.h"
#include "DoctorsPrimaryIndex.h"
#include <iostream>
#include <cstring>

using namespace std;

fstream doctorsFile, appointmentsFile;
DoctorsPrimaryIndex doctorsIndex("doctors_index.dat");

void openFiles() {
    doctorsFile.open("doctors.dat", ios::in | ios::out | ios::binary);
    if (!doctorsFile) {
        doctorsFile.open("doctors.dat", ios::out);
        doctorsFile.close();
        doctorsFile.open("doctors.dat", ios::in | ios::out | ios::binary);
    }

    appointmentsFile.open("appointments.dat", ios::in | ios::out | ios::binary);
    if (!appointmentsFile) {
        appointmentsFile.open("appointments.dat", ios::out);
        appointmentsFile.close();
        appointmentsFile.open("appointments.dat", ios::in | ios::out | ios::binary);
    }

    // Load the doctors index
    doctorsIndex.loadIndex();
}

void closeFiles() {
    if (doctorsFile.is_open()) doctorsFile.close();
    if (appointmentsFile.is_open()) appointmentsFile.close();

    // Save the doctors index
    doctorsIndex.saveIndex();
}

void writeDoctorRecord(const Doctor& doctor) {
    // Check if the doctor already exists in the index
    if (doctorsIndex.searchByDoctorID(doctor.doctorID) != -1) {
        cout << "Error: Doctor with ID " << doctor.doctorID << " already exists.\n";
        return;
    }

    // Prepare the record to write
    string record = string(doctor.doctorID) + "|" +
                    string(doctor.doctorName) + "|" +
                    string(doctor.address) + "\n";

    // Write to the end of the doctors file
    doctorsFile.seekp(0, ios::end);
    streampos position = doctorsFile.tellp(); // Get the current position
    doctorsFile.write(record.c_str(), record.size());

    // Add the doctor to the primary index
    doctorsIndex.addIndexEntry(doctor.doctorID, position);
}

Doctor readDoctorRecord(const string& doctorID) {
    Doctor doctor;

    // Search for the doctor in the primary index
    streampos position = doctorsIndex.searchByDoctorID(doctorID);
    if (position == -1) {
        cout << "Error: Doctor with ID " << doctorID << " not found.\n";
        return doctor;
    }

    // Read the record from the file
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
    streampos position = doctorsIndex.searchByDoctorID(doctorID);
    if (position == -1) {
        cout << "Error: Doctor with ID " << doctorID << " not found.\n";
        return;
    }

    // Mark the record as deleted
    doctorsFile.seekp(position);
    doctorsFile.write("*", 1); // Write a marker (*)

    // Remove from the primary index
    doctorsIndex.removeIndexEntry(doctorID);

    cout << "Doctor with ID " << doctorID << " has been deleted.\n";
}
