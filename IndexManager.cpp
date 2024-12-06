#include "IndexManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <utility> 
using namespace std;

// Static member initialization
ofstream IndexManager::doctorPrimaryIndexFile;
ofstream IndexManager::appointmentPrimaryIndexFile;
ofstream IndexManager::doctorNameSecondaryIndexFile;
ofstream IndexManager::appointmentDoctorIDSecondaryIndexFile;

map<string, streampos> IndexManager::doctorPrimaryIndex;
map<string, streampos> IndexManager::appointmentPrimaryIndex;
map<string, list<string>> IndexManager::doctorNameSecondaryIndex;
map<string, list<string>> IndexManager::doctorIDSecondaryIndex;

// Static members for avail lists
vector<pair<streampos, size_t>> IndexManager::doctorAvailList;
vector<pair<streampos, size_t>> IndexManager::appointmentAvailList;

// Function to open index and avail list files
void IndexManager::openIndexFiles() {
    doctorPrimaryIndexFile.open("doctor_primary_index.txt", ios::app);
    appointmentPrimaryIndexFile.open("appointment_primary_index.txt", ios::app);
    doctorNameSecondaryIndexFile.open("doctor_name_secondary_index.txt", ios::app);
    appointmentDoctorIDSecondaryIndexFile.open("appointment_doctor_id_secondary_index.txt", ios::app);

    readPrimaryIndexFile();
    readSecondaryIndexFiles();
    readAvailListFiles(); 
}

// Function to close index and avail list files
void IndexManager::closeIndexFiles() {
    writeIndexFile();
    writeSecondaryIndexFiles();
    writeAvailListFiles(); 

    if (doctorPrimaryIndexFile.is_open()) doctorPrimaryIndexFile.close();
    if (appointmentPrimaryIndexFile.is_open()) appointmentPrimaryIndexFile.close();
    if (doctorNameSecondaryIndexFile.is_open()) doctorNameSecondaryIndexFile.close();
    if (appointmentDoctorIDSecondaryIndexFile.is_open()) appointmentDoctorIDSecondaryIndexFile.close();
}

void IndexManager::readAvailListFiles() {
    ifstream doctorAvailFile("doctorAvailList.txt");
    ifstream appointmentAvailFile("appointmentAvailList.txt");
    long long pos; 
    size_t size;

    while (doctorAvailFile >> ws >> pos >> size) {
        doctorAvailList.push_back({static_cast<streampos>(pos), size});
    }
    doctorAvailFile.close();
    while (appointmentAvailFile >> ws >> pos >> size) {
        appointmentAvailList.push_back({static_cast<streampos>(pos), size});
    }
    appointmentAvailFile.close();
}
void IndexManager::writeAvailListFiles() {
    ofstream doctorAvailFile("doctorAvailList.txt");
    ofstream appointmentAvailFile("appointmentAvailList.txt");

    for (const auto& entry : doctorAvailList) {
        doctorAvailFile << static_cast<long long>(entry.first) << " " << entry.second << "\n";
    }

    for (const auto& entry : appointmentAvailList) {
        appointmentAvailFile << static_cast<long long>(entry.first) << " " << entry.second << "\n";
    }

    doctorAvailFile.close();
    appointmentAvailFile.close();
}

void IndexManager::addToAvailList(const string& recordType, streampos position, size_t size) {
    if (recordType == "doctor") {
        doctorAvailList.push_back({position, size});
    } else if (recordType == "appointment") {
        appointmentAvailList.push_back({position, size});
    }
}

vector<pair<streampos, size_t>> IndexManager::getAvailList(const string& recordType) {
    if (recordType == "doctor") {
        return doctorAvailList;
    } else if (recordType == "appointment") {
        return appointmentAvailList;
    }
    return {};
}

void IndexManager::addDoctorToIndexFile(const Doctor& doctor, streampos position) {
    doctorPrimaryIndex[doctor.doctorID] = position;
    doctorNameSecondaryIndex[doctor.doctorName].push_back(doctor.doctorID);
}

void IndexManager::addAppointmentToIndexFile(const Appointment& appointment, streampos position) {
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
    ifstream doctorPrimaryFile("doctor_primary_index.txt");
    ifstream appointmentPrimaryFile("appointment_primary_index.txt");
    string line;
    char delimiter;

    while (getline(doctorPrimaryFile, line)) {
        istringstream iss(line);
        string doctorID;
        long long pos;
        if (iss >> doctorID >> delimiter >> pos) {
            doctorPrimaryIndex[doctorID] = static_cast<streampos>(pos);
        }
    }

    while (getline(appointmentPrimaryFile, line)) {
        istringstream iss(line);
        string appointmentID;
        long long pos;
        if (iss >> appointmentID >> delimiter >> pos) {
            appointmentPrimaryIndex[appointmentID] = static_cast<streampos>(pos);
        }
    }
}

void IndexManager::readSecondaryIndexFiles() {
    ifstream doctorNameFile("doctor_name_secondary_index.txt");
    ifstream appointmentDoctorIDFile("appointment_doctor_id_secondary_index.txt");
    string line;
    char delimiter;

    // Read doctor name secondary index
    while (getline(doctorNameFile, line)) {
        istringstream iss(line);
        string doctorName;
        string doctorID;
        if (iss >> doctorName >> delimiter >> doctorID) {
            doctorNameSecondaryIndex[doctorName].push_back(doctorID);
        }
    }

    // Read appointment doctor ID secondary index
    while (getline(appointmentDoctorIDFile, line)) {
        istringstream iss(line);
        string doctorID;
        string appointmentID;
        if (iss >> doctorID >> delimiter >> appointmentID) {
            doctorIDSecondaryIndex[doctorID].push_back(appointmentID);
        }
    }
}

// Remove a doctor from the primary and secondary indexes
void IndexManager::removeDoctorFromIndexFile(const string& doctorID) {
    // Remove from primary index
    auto primaryIt = doctorPrimaryIndex.find(doctorID);
    if (primaryIt != doctorPrimaryIndex.end()) {
        doctorPrimaryIndex.erase(primaryIt);
    } else {
        cerr << "Doctor ID not found in primary index: " << doctorID << endl;
    }

    // Remove from secondary index (by name)
    for (auto& [doctorName, doctorIDs] : doctorNameSecondaryIndex) {
        auto it = find(doctorIDs.begin(), doctorIDs.end(), doctorID);
        if (it != doctorIDs.end()) {
            doctorIDs.erase(it);
            if (doctorIDs.empty()) {
                doctorNameSecondaryIndex.erase(doctorName); 
            }
            break;
        }
    }
}

void IndexManager::removeAppointmentFromIndexFile(const string& appointmentID) {
    // Remove from primary index
    auto primaryIt = appointmentPrimaryIndex.find(appointmentID);
    if (primaryIt != appointmentPrimaryIndex.end()) {
        appointmentPrimaryIndex.erase(primaryIt);
    } else {
        cerr << "Appointment ID not found in primary index: " << appointmentID << endl;
    }

    for (auto& [doctorID, appointmentIDs] : doctorIDSecondaryIndex) {
        auto it = find(appointmentIDs.begin(), appointmentIDs.end(), appointmentID);
        if (it != appointmentIDs.end()) {
            appointmentIDs.erase(it);
            if (appointmentIDs.empty()) {
                doctorIDSecondaryIndex.erase(doctorID);
            }
            break;
        }
    }
}

const map<string, streampos>& IndexManager::getDoctorPrimaryIndex() {
    return doctorPrimaryIndex;
}

const map<string, streampos>& IndexManager::getAppointmentPrimaryIndex() {
    return appointmentPrimaryIndex;
}

const map<string, list<string>>& IndexManager::getDoctorNameSecondaryIndex() {
    return doctorNameSecondaryIndex;
}

const map<string, list<string>>& IndexManager::getDoctorIDSecondaryIndex() {
    return doctorIDSecondaryIndex;
}


