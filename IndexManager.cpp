#include "IndexManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

std::ofstream IndexManager::doctorPrimaryIndexFile;
std::ofstream IndexManager::appointmentPrimaryIndexFile;
std::ofstream IndexManager::doctorNameSecondaryIndexFile;
std::ofstream IndexManager::appointmentDoctorIDSecondaryIndexFile;

std::map<std::string, std::streampos> IndexManager::doctorPrimaryIndex;
std::map<std::string, std::streampos> IndexManager::appointmentPrimaryIndex;
std::map<std::string, std::list<std::string>> IndexManager::doctorNameSecondaryIndex;
std::map<std::string, std::list<std::string>> IndexManager::doctorIDSecondaryIndex;

void IndexManager::openIndexFiles() {
    doctorPrimaryIndexFile.open("doctor_primary_index.txt", std::ios::app);
    appointmentPrimaryIndexFile.open("appointment_primary_index.txt", std::ios::app);
    doctorNameSecondaryIndexFile.open("doctor_name_secondary_index.txt", std::ios::app);
    appointmentDoctorIDSecondaryIndexFile.open("appointment_doctor_id_secondary_index.txt", std::ios::app);

    readPrimaryIndexFile();
    readSecondaryIndexFiles();
}

void IndexManager::closeIndexFiles() {
    writeIndexFile();
    writeSecondaryIndexFiles();

    if (doctorPrimaryIndexFile.is_open()) doctorPrimaryIndexFile.close();
    if (appointmentPrimaryIndexFile.is_open()) appointmentPrimaryIndexFile.close();
    if (doctorNameSecondaryIndexFile.is_open()) doctorNameSecondaryIndexFile.close();
    if (appointmentDoctorIDSecondaryIndexFile.is_open()) appointmentDoctorIDSecondaryIndexFile.close();
}

void IndexManager::addDoctorToIndexFile(const Doctor& doctor, std::streampos position) {
    doctorPrimaryIndex[doctor.doctorID] = position;
    doctorNameSecondaryIndex[doctor.doctorName].push_back(doctor.doctorID);
}

void IndexManager::addAppointmentToIndexFile(const Appointment& appointment, std::streampos position) {
    appointmentPrimaryIndex[appointment.appointmentID] = position;
    doctorIDSecondaryIndex[appointment.doctorID].push_back(appointment.appointmentID);
}

void IndexManager::writeIndexFile() {
    for (const auto& [key, value] : doctorPrimaryIndex) {
        doctorPrimaryIndexFile << key << "|" << value << "\n";
    }
    for (const auto& [key, value] : appointmentPrimaryIndex) {
        appointmentPrimaryIndexFile << key << "|" << value << "\n";
    }
}

void IndexManager::writeSecondaryIndexFiles() {
    for (const auto& [key, valueList] : doctorNameSecondaryIndex) {
        for (const auto& value : valueList) {
            doctorNameSecondaryIndexFile << key << "|" << value << "\n";
        }
    }
    for (const auto& [key, valueList] : doctorIDSecondaryIndex) {
        for (const auto& value : valueList) {
            appointmentDoctorIDSecondaryIndexFile << key << "|" << value << "\n";
        }
    }
}

void IndexManager::readPrimaryIndexFile() {
    std::ifstream doctorPrimaryFile("doctor_primary_index.txt");
    std::ifstream appointmentPrimaryFile("appointment_primary_index.txt");
    std::string line;
    char delimiter;

    while (std::getline(doctorPrimaryFile, line)) {
        std::istringstream iss(line);
        std::string doctorID;
        long long pos;
        if (iss >> doctorID >> delimiter >> pos) {
            doctorPrimaryIndex[doctorID] = static_cast<std::streampos>(pos);
        }
    }

    while (std::getline(appointmentPrimaryFile, line)) {
        std::istringstream iss(line);
        std::string appointmentID;
        long long pos;
        if (iss >> appointmentID >> delimiter >> pos) {
            appointmentPrimaryIndex[appointmentID] = static_cast<std::streampos>(pos);
        }
    }
}

void IndexManager::readSecondaryIndexFiles() {
    std::ifstream doctorNameFile("doctor_name_secondary_index.txt");
    std::ifstream appointmentDoctorIDFile("appointment_doctor_id_secondary_index.txt");
    std::string line;
    char delimiter;

    // Read doctor name secondary index
    while (std::getline(doctorNameFile, line)) {
        std::istringstream iss(line);
        std::string doctorName;
        std::string doctorID;
        if (iss >> doctorName >> delimiter >> doctorID) {
            doctorNameSecondaryIndex[doctorName].push_back(doctorID);
        }
    }

    // Read appointment doctor ID secondary index
    while (std::getline(appointmentDoctorIDFile, line)) {
        std::istringstream iss(line);
        std::string doctorID;
        std::string appointmentID;
        if (iss >> doctorID >> delimiter >> appointmentID) {
            doctorIDSecondaryIndex[doctorID].push_back(appointmentID);
        }
    }
}

// New accessor methods for accessing indexes as const references
const std::map<std::string, std::streampos>& IndexManager::getDoctorPrimaryIndex() {
    return doctorPrimaryIndex;
}

const std::map<std::string, std::streampos>& IndexManager::getAppointmentPrimaryIndex() {
    return appointmentPrimaryIndex;
}

const std::map<std::string, std::list<std::string>>& IndexManager::getDoctorNameSecondaryIndex() {
    return doctorNameSecondaryIndex;
}

const std::map<std::string, std::list<std::string>>& IndexManager::getDoctorIDSecondaryIndex() {
    return doctorIDSecondaryIndex;
}
