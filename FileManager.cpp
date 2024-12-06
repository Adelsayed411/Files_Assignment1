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

bool firstDoctorDeletion = true;
bool firstAppointmentDeletion = true;

void deleteDoctorRecord(const string& doctorID) {
    const auto& doctorPrimaryIndex = IndexManager::getDoctorPrimaryIndex();
    auto it = doctorPrimaryIndex.find(doctorID);
    if (it != doctorPrimaryIndex.end()) {
        streampos position = it->second;

        doctorsFile.seekg(position);
        string record;
        getline(doctorsFile, record);
        size_t size = record.size() + 1;

        doctorsFile.seekp(position);
        if (firstDoctorDeletion) {
            doctorsFile << "*-1\n";  
            firstDoctorDeletion = false;
        } else {
            doctorsFile << "*" << position << "\n"; 
        }

        IndexManager::removeDoctorFromIndexFile(doctorID);
        IndexManager::addToAvailList("doctor", position, size);

        cout << "Doctor record with ID " << doctorID << " marked as deleted.\n";
    } else {
        cout << "Doctor ID " << doctorID << " not found.\n";
    }
}

void deleteAppointmentRecord(const string& appointmentID) {
    const auto& appointmentPrimaryIndex = IndexManager::getAppointmentPrimaryIndex();
    auto it = appointmentPrimaryIndex.find(appointmentID);
    if (it != appointmentPrimaryIndex.end()) {
        streampos position = it->second;

        appointmentsFile.seekg(position);
        string record;
        getline(appointmentsFile, record);
        size_t size = record.size() + 1;  

        appointmentsFile.seekp(position);
        if (firstAppointmentDeletion) {
            appointmentsFile << "*-1\n";
            firstAppointmentDeletion = false;
        } else {
            appointmentsFile << "*" << position << "\n";  
        }

        IndexManager::removeAppointmentFromIndexFile(appointmentID);
        IndexManager::addToAvailList("appointment", position, size);

        cout << "Appointment record with ID " << appointmentID << " marked as deleted.\n";
    } else {
        cout << "Appointment ID " << appointmentID << " not found.\n";
    }
}


#include <algorithm>

// Function to add a doctor record with best-fit placement strategy
void addDoctorRecord(const Doctor& doctor) {
    const auto& doctorPrimaryIndex = IndexManager::getDoctorPrimaryIndex();
    if (doctorPrimaryIndex.find(doctor.doctorID) != doctorPrimaryIndex.end()) {
        throw runtime_error("Duplicate Doctor ID is not allowed: " + string(doctor.doctorID));
    }

    ostringstream recordStream;
    recordStream << doctor.doctorID << "|" << doctor.doctorName << "|" << doctor.address << "\n";
    string record = recordStream.str();
    size_t recordSize = record.size();

    auto& availList = IndexManager::doctorAvailList; 
    sort(availList.begin(), availList.end(), [](const auto& a, const auto& b) {
        return a.second < b.second; 
    });

    streampos position;
    bool recordPlaced = false;

    for (auto it = availList.begin(); it != availList.end(); ++it) {
        if (it->second >= recordSize) {
      
            position = it->first;
            doctorsFile.seekp(position);
            doctorsFile.write(record.c_str(), recordSize);

            availList.erase(it);
            recordPlaced = true;
            break;
        }
    }

    if (!recordPlaced) {
        doctorsFile.seekp(0, ios::end);
        position = doctorsFile.tellp();
        doctorsFile.write(record.c_str(), recordSize);
    }

    IndexManager::addDoctorToIndexFile(doctor, position);

    if (availList.size() == 1) {
        auto lastDeleted = availList.front();
        doctorsFile.seekp(lastDeleted.first);
        doctorsFile << "*-1\n";
    }

    IndexManager::writeAvailListFiles();
}

// Function to add an appointment record with best-fit placement strategy
void addAppointmentRecord(const Appointment& appointment) {
 
    const auto& appointmentPrimaryIndex = IndexManager::getAppointmentPrimaryIndex();
    if (appointmentPrimaryIndex.find(appointment.appointmentID) != appointmentPrimaryIndex.end()) {
        throw runtime_error("Duplicate Appointment ID is not allowed: " + string(appointment.appointmentID));
    }

    ostringstream recordStream;
    recordStream << appointment.appointmentID << "|" << appointment.appointmentDate << "|" << appointment.doctorID << "\n";
    string record = recordStream.str();
    size_t recordSize = record.size();

    auto& availList = IndexManager::appointmentAvailList; 
    sort(availList.begin(), availList.end(), [](const auto& a, const auto& b) {
        return a.second < b.second; 
    });

    streampos position;
    bool recordPlaced = false;

    for (auto it = availList.begin(); it != availList.end(); ++it) {
        if (it->second >= recordSize) {
           
            position = it->first;

            appointmentsFile.seekp(position);
            appointmentsFile.write(record.c_str(), recordSize);

            availList.erase(it);
            recordPlaced = true;
            break;
        }
    }

    if (!recordPlaced) {
        appointmentsFile.seekp(0, ios::end);
        position = appointmentsFile.tellp();
        appointmentsFile.write(record.c_str(), recordSize);
    }

    IndexManager::addAppointmentToIndexFile(appointment, position);

    if (availList.size() == 1) {
        auto lastDeleted = availList.front();
        appointmentsFile.seekp(lastDeleted.first);
        appointmentsFile << "*-1\n";
    }

    IndexManager::writeAvailListFiles();
}

void updateDoctorRecord(const string& doctorID, const string& newDoctorName, const string& newAddress) {

    const auto& doctorPrimaryIndex = IndexManager::getDoctorPrimaryIndex();
    auto it = doctorPrimaryIndex.find(doctorID);

    if (it == doctorPrimaryIndex.end()) {
        throw runtime_error("Doctor ID not found: " + doctorID);
    }

    streampos position = it->second;

    doctorsFile.seekg(position);
    string record;
    getline(doctorsFile, record);

    size_t pos1 = record.find('|');
    size_t pos2 = record.find('|', pos1 + 1);

    if (pos1 == string::npos || pos2 == string::npos) {
        throw runtime_error("Corrupt record format for Doctor ID: " + doctorID);
    }

    string currentDoctorName = record.substr(pos1 + 1, pos2 - pos1 - 1);
    string currentAddress = record.substr(pos2 + 1);

    string updatedDoctorName = newDoctorName.substr(0, 30); 
    string updatedAddress = newAddress.substr(0, 30);       

    ostringstream updatedRecordStream;
    updatedRecordStream << doctorID << "|" << updatedDoctorName << "|" << updatedAddress << "\n";
    string updatedRecord = updatedRecordStream.str();

    doctorsFile.seekp(position);
    doctorsFile.write(updatedRecord.c_str(), updatedRecord.size());

    if (currentDoctorName != updatedDoctorName) {
        IndexManager::removeDoctorFromIndexFile(doctorID); 
        Doctor updatedDoctor = {0}; 
        strncpy(updatedDoctor.doctorID, doctorID.c_str(), 15);
        strncpy(updatedDoctor.doctorName, updatedDoctorName.c_str(), 30);
        strncpy(updatedDoctor.address, updatedAddress.c_str(), 30);
        IndexManager::addDoctorToIndexFile(updatedDoctor, position); 
    }

    cout << "Doctor record with ID " << doctorID << " updated successfully.\n";
}
void updateAppointmentRecord(const string& appointmentID, const string& newAppointmentDate) {
    const auto& appointmentPrimaryIndex = IndexManager::getAppointmentPrimaryIndex();
    auto it = appointmentPrimaryIndex.find(appointmentID);

    if (it == appointmentPrimaryIndex.end()) {
        throw runtime_error("Appointment ID not found: " + appointmentID);
    }

    streampos position = it->second;

    appointmentsFile.seekg(position);
    string record;
    getline(appointmentsFile, record);

    size_t pos1 = record.find('|');
    size_t pos2 = record.find('|', pos1 + 1);

    if (pos1 == string::npos || pos2 == string::npos) {
        throw runtime_error("Corrupt record format for Appointment ID: " + appointmentID);
    }

    string currentAppointmentDate = record.substr(pos1 + 1, pos2 - pos1 - 1);
    string doctorID = record.substr(pos2 + 1);

    string updatedAppointmentDate = newAppointmentDate.substr(0, 30); 

    ostringstream updatedRecordStream;
    updatedRecordStream << appointmentID << "|" << updatedAppointmentDate << "|" << doctorID << "\n";
    string updatedRecord = updatedRecordStream.str();

    appointmentsFile.seekp(position);
    appointmentsFile.write(updatedRecord.c_str(), updatedRecord.size());

    cout << "Appointment record with ID " << appointmentID << " updated successfully.\n";
}

#include <regex>
// Function to process and execute user queries using regex
void processQuery(const string& query) {
    // Regular expressions for query validation
    regex selectAllFromDoctorsRegex(R"(SELECT\s+ALL\s+FROM\s+DOCTORS\s+WHERE\s+DOCTOR\s+ID\s*=\s*'(.*?)';)", regex::icase);
    regex selectAllFromAppointmentsRegex(R"(SELECT\s+ALL\s+FROM\s+APPOINTMENTS\s+WHERE\s+DOCTOR\s+ID\s*=\s*'(.*?)';)", regex::icase);
    regex selectDoctorNameFromDoctorsRegex(R"(SELECT\s+DOCTOR\s+NAME\s+FROM\s+DOCTORS\s+WHERE\s+DOCTOR\s+ID\s*=\s*'(.*?)';)", regex::icase);

    smatch match;

    // Check for "SELECT ALL FROM DOCTORS WHERE DOCTOR ID='xxx';"
    if (regex_match(query, match, selectAllFromDoctorsRegex)) {
        string doctorID = match[1].str();
        try {
            Doctor doctor = searchDoctorByID(doctorID);
            cout << "Doctor ID: " << doctor.doctorID << "\n"
                      << "Name: " << doctor.doctorName << "\n"
                      << "Address: " << doctor.address << "\n";
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << " for Doctor ID " << doctorID << "\n";
        }
        return;
    }

    // Check for "SELECT ALL FROM APPOINTMENTS WHERE DOCTOR ID='xxx';"
    if (regex_match(query, match, selectAllFromAppointmentsRegex)) {
        string doctorID = match[1].str();
        try {
            vector<streampos> positions = searchAppointmentsByDoctorID(doctorID);
            for (const auto& pos : positions) {
                Appointment appointment = readAppointmentRecord(pos);
                cout << "Appointment ID: " << appointment.appointmentID << "\n"
                          << "Date: " << appointment.appointmentDate << "\n"
                          << "Doctor ID: " << appointment.doctorID << "\n";
            }
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << " for Doctor ID " << doctorID << "\n";
        }
        return;
    }

    // Check for "SELECT DOCTOR NAME FROM DOCTORS WHERE DOCTOR ID='xxx';"
    if (regex_match(query, match, selectDoctorNameFromDoctorsRegex)) {
        string doctorID = match[1].str();
        try {
            Doctor doctor = searchDoctorByID(doctorID);
            cout << "Doctor Name: " << doctor.doctorName << "\n";
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << " for Doctor ID " << doctorID << "\n";
        }
        return;
    }

    cout << "Invalid query format.\n";
}