#include "FileManager.h"
#include "IndexManager.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <vector>
using namespace std;

// Global variables for file streams
fstream doctorsFile, appointmentsFile;

// Function to open files
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

    // Open and load index files using IndexManager
    IndexManager::openIndexFiles();
}

// Function to close files
void closeFiles() {
    if (doctorsFile.is_open()) doctorsFile.close();
    if (appointmentsFile.is_open()) appointmentsFile.close();

    // Close index files
    IndexManager::closeIndexFiles();
}

// Function to write a doctor record
void writeDoctorRecord(const Doctor& doctor) {
    doctorsFile.seekp(0, ios::end);
    streampos position = doctorsFile.tellp();

    string record = string(doctor.doctorID) + "|" +
                    string(doctor.doctorName) + "|" +
                    string(doctor.address) + "\n";

    doctorsFile.write(record.c_str(), record.size());
    IndexManager::addDoctorToIndexFile(doctor, position);
}

// Function to write an appointment record
void writeAppointmentRecord(const Appointment& appointment) {
    appointmentsFile.seekp(0, ios::end);
    streampos position = appointmentsFile.tellp();

    string record = string(appointment.appointmentID) + "|" +
                    string(appointment.appointmentDate) + "|" +
                    string(appointment.doctorID) + "\n";

    appointmentsFile.write(record.c_str(), record.size());
    IndexManager::addAppointmentToIndexFile(appointment, position);
}

// Function to read a doctor record from a specific position
Doctor readDoctorRecord(streampos position) {
    Doctor doctor;
    string record;
    doctorsFile.seekg(position);
    getline(doctorsFile, record);

    size_t pos1 = record.find('|');
    size_t pos2 = record.find('|', pos1 + 1);

    strncpy(doctor.doctorID, record.substr(0, pos1).c_str(), 15);
    strncpy(doctor.doctorName, record.substr(pos1 + 1, pos2 - pos1 - 1).c_str(), 30);
    strncpy(doctor.address, record.substr(pos2 + 1).c_str(), 30);

    return doctor;
}

// Function to read an appointment record from a specific position
Appointment readAppointmentRecord(streampos position) {
    Appointment appointment;
    string record;
    appointmentsFile.seekg(position);
    getline(appointmentsFile, record);

    size_t pos1 = record.find('|');
    size_t pos2 = record.find('|', pos1 + 1);

    strncpy(appointment.appointmentID, record.substr(0, pos1).c_str(), 15);
    strncpy(appointment.appointmentDate, record.substr(pos1 + 1, pos2 - pos1 - 1).c_str(), 30);
    strncpy(appointment.doctorID, record.substr(pos2 + 1).c_str(), 15);

    return appointment;
}

// Function to search for a doctor by ID using the primary index
Doctor searchDoctorByID(const string& doctorID) {
    const auto& doctorPrimaryIndex = IndexManager::getDoctorPrimaryIndex();
    auto it = doctorPrimaryIndex.find(doctorID);
    if (it != doctorPrimaryIndex.end()) {
        return readDoctorRecord(it->second);
    } else {
        throw runtime_error("Doctor ID not found");
    }
}

// Function to search for an appointment by ID using the primary index
Appointment searchAppointmentByID(const string& appointmentID) {
    const auto& appointmentPrimaryIndex = IndexManager::getAppointmentPrimaryIndex();
    auto it = appointmentPrimaryIndex.find(appointmentID);
    if (it != appointmentPrimaryIndex.end()) {
        return readAppointmentRecord(it->second);
    } else {
        throw runtime_error("Appointment ID not found");
    }
}

// Function to search for doctors by name using the secondary index
vector<streampos> searchDoctorByName(const string& doctorName) {
    vector<streampos> positions;
    const auto& doctorNameSecondaryIndex = IndexManager::getDoctorNameSecondaryIndex();

    auto it = doctorNameSecondaryIndex.find(doctorName);
    if (it != doctorNameSecondaryIndex.end()) {
        for (const auto& doctorID : it->second) {
            auto posIt = IndexManager::getDoctorPrimaryIndex().find(doctorID);
            if (posIt != IndexManager::getDoctorPrimaryIndex().end()) {
                positions.push_back(posIt->second);
            }
        }
    } else {
        throw runtime_error("Doctor name not found");
    }

    return positions;
}

// Function to search for appointments by doctor ID using the secondary index
vector<streampos> searchAppointmentsByDoctorID(const string& doctorID) {
    vector<streampos> positions;
    const auto& doctorIDSecondaryIndex = IndexManager::getDoctorIDSecondaryIndex();

    auto it = doctorIDSecondaryIndex.find(doctorID);
    if (it != doctorIDSecondaryIndex.end()) {
        for (const auto& appointmentID : it->second) {
            auto posIt = IndexManager::getAppointmentPrimaryIndex().find(appointmentID);
            if (posIt != IndexManager::getAppointmentPrimaryIndex().end()) {
                positions.push_back(posIt->second);
            }
        }
    } else {
        throw runtime_error("Doctor ID not found in appointments");
    }

    return positions;
}

#include <stdexcept>

// Flag to track if the first deletion has occurred
bool firstDoctorDeletion = true;
bool firstAppointmentDeletion = true;

// Function to delete a doctor record and update the index and avail list
void deleteDoctorRecord(const std::string& doctorID) {
    const auto& doctorPrimaryIndex = IndexManager::getDoctorPrimaryIndex();
    auto it = doctorPrimaryIndex.find(doctorID);
    if (it != doctorPrimaryIndex.end()) {
        std::streampos position = it->second;

        // Calculate the size of the record
        doctorsFile.seekg(position);
        std::string record;
        std::getline(doctorsFile, record);
        size_t size = record.size() + 1;  // Include newline character

        // Mark the record as deleted in the data file
        doctorsFile.seekp(position);
        if (firstDoctorDeletion) {
            doctorsFile << "*-1\n";  // Marking the first deleted record
            firstDoctorDeletion = false;
        } else {
            doctorsFile << "*" << position << "\n";  // Mark subsequent deletions with offset
        }

        // Remove from indexes and add to avail list
        IndexManager::removeDoctorFromIndexFile(doctorID);
        IndexManager::addToAvailList("doctor", position, size);

        std::cout << "Doctor record with ID " << doctorID << " marked as deleted.\n";
    } else {
        std::cout << "Doctor ID " << doctorID << " not found.\n";
    }
}

// Function to delete an appointment record and update the index and avail list
void deleteAppointmentRecord(const std::string& appointmentID) {
    const auto& appointmentPrimaryIndex = IndexManager::getAppointmentPrimaryIndex();
    auto it = appointmentPrimaryIndex.find(appointmentID);
    if (it != appointmentPrimaryIndex.end()) {
        std::streampos position = it->second;

        // Calculate the size of the record
        appointmentsFile.seekg(position);
        std::string record;
        std::getline(appointmentsFile, record);
        size_t size = record.size() + 1;  // Include newline character

        // Mark the record as deleted in the data file
        appointmentsFile.seekp(position);
        if (firstAppointmentDeletion) {
            appointmentsFile << "*-1\n";  // Marking the first deleted record
            firstAppointmentDeletion = false;
        } else {
            appointmentsFile << "*" << position << "\n";  // Mark subsequent deletions with offset
        }

        // Remove from indexes and add to avail list
        IndexManager::removeAppointmentFromIndexFile(appointmentID);
        IndexManager::addToAvailList("appointment", position, size);

        std::cout << "Appointment record with ID " << appointmentID << " marked as deleted.\n";
    } else {
        std::cout << "Appointment ID " << appointmentID << " not found.\n";
    }
}


#include <algorithm>


// Function to add a doctor record with best-fit placement strategy
void addDoctorRecord(const Doctor& doctor) {
    // Check for duplicate primary key
    const auto& doctorPrimaryIndex = IndexManager::getDoctorPrimaryIndex();
    if (doctorPrimaryIndex.find(doctor.doctorID) != doctorPrimaryIndex.end()) {
        throw std::runtime_error("Duplicate Doctor ID is not allowed: " + std::string(doctor.doctorID));
    }

    // Serialize the record into a string
    std::ostringstream recordStream;
    recordStream << doctor.doctorID << "|" << doctor.doctorName << "|" << doctor.address << "\n";
    std::string record = recordStream.str();
    size_t recordSize = record.size();

    // Sort the avail list by size (ascending order)
    auto availList = IndexManager::getAvailList("doctor");
    std::sort(availList.begin(), availList.end(), [](const auto& a, const auto& b) {
        return a.second < b.second; // Sort by size
    });

    std::streampos position;
    bool recordPlaced = false;

    // Find the best-fit slot
    for (auto it = availList.begin(); it != availList.end(); ++it) {
        if (it->second >= recordSize) {
            // Suitable slot found
            position = it->first;

            // Write the record to the file
            doctorsFile.seekp(position);
            doctorsFile.write(record.c_str(), recordSize);

            // Update the avail list and remove the used slot
            size_t remainingSize = it->second - recordSize;
            availList.erase(it);
            if (remainingSize > 0) {
                availList.push_back({position + static_cast<std::streamoff>(recordSize), remainingSize});
            }
            recordPlaced = true;
            break;
        }
    }

    // If no suitable slot is found, append at the end of the file
    if (!recordPlaced) {
        doctorsFile.seekp(0, std::ios::end);
        position = doctorsFile.tellp();
        doctorsFile.write(record.c_str(), recordSize);
    }

    // Update the index
    IndexManager::addDoctorToIndexFile(doctor, position);

    // Mark the last deleted record as *-1 if it remains
    if (availList.size() == 1) {
        auto lastDeleted = availList.front();
        doctorsFile.seekp(lastDeleted.first);
        doctorsFile << "*-1\n";
    }

    // Write back the updated avail list
    IndexManager::writeAvailListFiles();
}

// Function to add an appointment record with best-fit placement strategy
void addAppointmentRecord(const Appointment& appointment) {
    // Check for duplicate primary key
    const auto& appointmentPrimaryIndex = IndexManager::getAppointmentPrimaryIndex();
    if (appointmentPrimaryIndex.find(appointment.appointmentID) != appointmentPrimaryIndex.end()) {
        throw std::runtime_error("Duplicate Appointment ID is not allowed: " + std::string(appointment.appointmentID));
    }

    // Serialize the record into a string
    std::ostringstream recordStream;
    recordStream << appointment.appointmentID << "|" << appointment.appointmentDate << "|" << appointment.doctorID << "\n";
    std::string record = recordStream.str();
    size_t recordSize = record.size();

    // Sort the avail list by size (ascending order)
    auto availList = IndexManager::getAvailList("appointment");
    std::sort(availList.begin(), availList.end(), [](const auto& a, const auto& b) {
        return a.second < b.second; // Sort by size
    });

    std::streampos position;
    bool recordPlaced = false;

    // Find the best-fit slot
    for (auto it = availList.begin(); it != availList.end(); ++it) {
        if (it->second >= recordSize) {
            // Suitable slot found
            position = it->first;

            // Write the record to the file
            appointmentsFile.seekp(position);
            appointmentsFile.write(record.c_str(), recordSize);

            // Update the avail list and remove the used slot
            size_t remainingSize = it->second - recordSize;
            availList.erase(it);
            if (remainingSize > 0) {
                availList.push_back({position + static_cast<std::streamoff>(recordSize), remainingSize});
            }
            recordPlaced = true;
            break;
        }
    }

    // If no suitable slot is found, append at the end of the file
    if (!recordPlaced) {
        appointmentsFile.seekp(0, std::ios::end);
        position = appointmentsFile.tellp();
        appointmentsFile.write(record.c_str(), recordSize);
    }

    // Update the index
    IndexManager::addAppointmentToIndexFile(appointment, position);

    // Mark the last deleted record as *-1 if it remains
    if (availList.size() == 1) {
        auto lastDeleted = availList.front();
        appointmentsFile.seekp(lastDeleted.first);
        appointmentsFile << "*-1\n";
    }

    // Write back the updated avail list
    IndexManager::writeAvailListFiles();
}







