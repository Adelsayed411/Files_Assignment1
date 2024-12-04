#include "FileManager.h"
#include "IndexManager.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>

// Global variables for file streams
std::fstream doctorsFile, appointmentsFile;

// Function to open files
void openFiles() {
    doctorsFile.open("doctors.dat", std::ios::in | std::ios::out | std::ios::binary);
    if (!doctorsFile) {
        doctorsFile.open("doctors.dat", std::ios::out);
        doctorsFile.close();
        doctorsFile.open("doctors.dat", std::ios::in | std::ios::out | std::ios::binary);
    }

    appointmentsFile.open("appointments.dat", std::ios::in | std::ios::out | std::ios::binary);
    if (!appointmentsFile) {
        appointmentsFile.open("appointments.dat", std::ios::out);
        appointmentsFile.close();
        appointmentsFile.open("appointments.dat", std::ios::in | std::ios::out | std::ios::binary);
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
    doctorsFile.seekp(0, std::ios::end);
    std::streampos position = doctorsFile.tellp();

    std::string record = std::string(doctor.doctorID) + "|" +
                         std::string(doctor.doctorName) + "|" +
                         std::string(doctor.address) + "\n";

    doctorsFile.write(record.c_str(), record.size());
    IndexManager::addDoctorToIndexFile(doctor, position);
}

// Function to write an appointment record
void writeAppointmentRecord(const Appointment& appointment) {
    appointmentsFile.seekp(0, std::ios::end);
    std::streampos position = appointmentsFile.tellp();

    std::string record = std::string(appointment.appointmentID) + "|" +
                         std::string(appointment.appointmentDate) + "|" +
                         std::string(appointment.doctorID) + "\n";

    appointmentsFile.write(record.c_str(), record.size());
    IndexManager::addAppointmentToIndexFile(appointment, position);
}

// Function to read a doctor record from a specific position
Doctor readDoctorRecord(std::streampos position) {
    Doctor doctor;
    std::string record;
    doctorsFile.seekg(position);
    std::getline(doctorsFile, record);

    size_t pos1 = record.find('|');
    size_t pos2 = record.find('|', pos1 + 1);

    std::strncpy(doctor.doctorID, record.substr(0, pos1).c_str(), 15);
    std::strncpy(doctor.doctorName, record.substr(pos1 + 1, pos2 - pos1 - 1).c_str(), 30);
    std::strncpy(doctor.address, record.substr(pos2 + 1).c_str(), 30);

    return doctor;
}

// Function to read an appointment record from a specific position
Appointment readAppointmentRecord(std::streampos position) {
    Appointment appointment;
    std::string record;
    appointmentsFile.seekg(position);
    std::getline(appointmentsFile, record);

    size_t pos1 = record.find('|');
    size_t pos2 = record.find('|', pos1 + 1);

    std::strncpy(appointment.appointmentID, record.substr(0, pos1).c_str(), 15);
    std::strncpy(appointment.appointmentDate, record.substr(pos1 + 1, pos2 - pos1 - 1).c_str(), 30);
    std::strncpy(appointment.doctorID, record.substr(pos2 + 1).c_str(), 15);

    return appointment;
}

// Function to search for a doctor by ID using the primary index
Doctor searchDoctorByID(const std::string& doctorID) {
    const auto& doctorPrimaryIndex = IndexManager::getDoctorPrimaryIndex();
    auto it = doctorPrimaryIndex.find(doctorID);
    if (it != doctorPrimaryIndex.end()) {
        return readDoctorRecord(it->second);
    } else {
        throw std::runtime_error("Doctor ID not found");
    }
}

// Function to search for an appointment by ID using the primary index
Appointment searchAppointmentByID(const std::string& appointmentID) {
    const auto& appointmentPrimaryIndex = IndexManager::getAppointmentPrimaryIndex();
    auto it = appointmentPrimaryIndex.find(appointmentID);
    if (it != appointmentPrimaryIndex.end()) {
        return readAppointmentRecord(it->second);
    } else {
        throw std::runtime_error("Appointment ID not found");
    }
}

// Function to search for doctors by name using the secondary index
std::vector<std::streampos> searchDoctorByName(const std::string& doctorName) {
    std::vector<std::streampos> positions;
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
        throw std::runtime_error("Doctor name not found");
    }

    return positions;
}

// Function to search for appointments by doctor ID using the secondary index
std::vector<std::streampos> searchAppointmentsByDoctorID(const std::string& doctorID) {
    std::vector<std::streampos> positions;
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
        throw std::runtime_error("Doctor ID not found in appointments");
    }

    return positions;
}
