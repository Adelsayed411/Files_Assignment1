#include "IndexManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

IndexManager::IndexManager() {}

IndexManager::~IndexManager() {}

void IndexManager::createDoctorsPrimaryIndex(const string& fileName) {
    loadIndexFromFile(fileName, doctorsPrimaryIndex);
}

void IndexManager::createAppointmentsPrimaryIndex(const string& fileName) {
    loadIndexFromFile(fileName, appointmentsPrimaryIndex);
}

void IndexManager::createDoctorsSecondaryIndex(const string& fileName) {
    loadSecondaryIndexFromFile(fileName, doctorsSecondaryIndex);
}

void IndexManager::createAppointmentsSecondaryIndex(const string& fileName) {
    loadSecondaryIndexFromFile(fileName, appointmentsSecondaryIndex);
}

void IndexManager::addPrimaryIndexEntry(const string& fileName, const string& key, streampos position) {
    if (fileName == "doctors.dat") {
        doctorsPrimaryIndex[key] = { key, position };
        writeIndexToFile(fileName, doctorsPrimaryIndex);
    } else if (fileName == "appointments.dat") {
        appointmentsPrimaryIndex[key] = { key, position };
        writeIndexToFile(fileName, appointmentsPrimaryIndex);
    }
}

void IndexManager::addSecondaryIndexEntry(const string& fileName, const string& key, streampos position) {
    if (fileName == "doctors.dat") {
        if (doctorsSecondaryIndex.find(key) == doctorsSecondaryIndex.end()) {
            doctorsSecondaryIndex[key] = new SecondaryIndexEntry{ position, nullptr };
        } else {
            SecondaryIndexEntry* current = doctorsSecondaryIndex[key];
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = new SecondaryIndexEntry{ position, nullptr };
        }
        writeSecondaryIndexToFile(fileName, doctorsSecondaryIndex);
    } else if (fileName == "appointments.dat") {
        if (appointmentsSecondaryIndex.find(key) == appointmentsSecondaryIndex.end()) {
            appointmentsSecondaryIndex[key] = new SecondaryIndexEntry{ position, nullptr };
        } else {
            SecondaryIndexEntry* current = appointmentsSecondaryIndex[key];
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = new SecondaryIndexEntry{ position, nullptr };
        }
        writeSecondaryIndexToFile(fileName, appointmentsSecondaryIndex);
    }
}

streampos IndexManager::searchPrimaryIndex(const string& fileName, const string& key) {
    if (fileName == "doctors.dat") {
        if (doctorsPrimaryIndex.find(key) != doctorsPrimaryIndex.end()) {
            return doctorsPrimaryIndex[key].position;
        }
    } else if (fileName == "appointments.dat") {
        if (appointmentsPrimaryIndex.find(key) != appointmentsPrimaryIndex.end()) {
            return appointmentsPrimaryIndex[key].position;
        }
    }
    return -1; // Return -1 if not found
}

vector<streampos> IndexManager::searchSecondaryIndex(const string& fileName, const string& key) {
    vector<streampos> positions;
    if (fileName == "doctors.dat") {
        if (doctorsSecondaryIndex.find(key) != doctorsSecondaryIndex.end()) {
            SecondaryIndexEntry* current = doctorsSecondaryIndex[key];
            while (current != nullptr) {
                positions.push_back(current->position);
                current = current->next;
            }
        }
    } else if (fileName == "appointments.dat") {
        if (appointmentsSecondaryIndex.find(key) != appointmentsSecondaryIndex.end()) {
            SecondaryIndexEntry* current = appointmentsSecondaryIndex[key];
            while (current != nullptr) {
                positions.push_back(current->position);
                current = current->next;
            }
        }
    }
    return positions;
}
void IndexManager::saveDoctorsIndex() {
    // Write the doctors primary index to the file
    writeIndexToFile("doctors_index.dat", doctorsPrimaryIndex);
}
void IndexManager::deleteIndexEntry(const string& fileName, const string& key) {
    if (fileName == "doctors.dat") {
        doctorsPrimaryIndex.erase(key);
        writeIndexToFile(fileName, doctorsPrimaryIndex);
    } else if (fileName == "appointments.dat") {
        appointmentsPrimaryIndex.erase(key);
        writeIndexToFile(fileName, appointmentsPrimaryIndex);
    }
}

void IndexManager::writeIndexToFile(const string& fileName, const map<string, PrimaryIndexEntry>& indexMap) {
    ofstream indexFile(fileName, ios::out | ios::binary);
    for (const auto& entry : indexMap) {
        // Write the key followed by a null terminator
        indexFile.write(entry.first.c_str(), entry.first.size() + 1);
        indexFile.write(reinterpret_cast<const char*>(&entry.second.position), sizeof(entry.second.position));
    }
    indexFile.close();
}


void IndexManager::writeSecondaryIndexToFile(const string& fileName, const map<string, SecondaryIndexEntry*>& indexMap) {
    ofstream indexFile(fileName, ios::out | ios::binary);
    for (const auto& entry : indexMap) {
        indexFile.write(entry.first.c_str(), entry.first.size());
        SecondaryIndexEntry* current = entry.second;
        while (current != nullptr) {
            indexFile.write(reinterpret_cast<const char*>(&current->position), sizeof(current->position));
            current = current->next;
        }
    }
    indexFile.close();
}

void IndexManager::loadIndexFromFile(const string& fileName, map<string, PrimaryIndexEntry>& indexMap) {
    ifstream indexFile(fileName, ios::in | ios::binary);
    if (!indexFile.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        return;
    }

    // Read the entries until EOF
    while (!indexFile.eof()) {
        string key;
        streampos position;

        // Read the length of the key first
        getline(indexFile, key, '\0'); // Read until null terminator

        if (indexFile.read(reinterpret_cast<char*>(&position), sizeof(position))) {
            indexMap[key] = { key, position };
        }
    }
    indexFile.close();
}


void IndexManager::loadSecondaryIndexFromFile(const string& fileName, map<string, SecondaryIndexEntry*>& indexMap) {
    ifstream indexFile(fileName, ios::in | ios::binary);
    string key;
    streampos position;
    while (indexFile >> key) {
        SecondaryIndexEntry* newEntry = new SecondaryIndexEntry;
        indexFile.read(reinterpret_cast<char*>(&position), sizeof(position));
        newEntry->position = position;
        newEntry->next = nullptr;
        if (indexMap.find(key) == indexMap.end()) {
            indexMap[key] = newEntry;
        } else {
            SecondaryIndexEntry* current = indexMap[key];
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newEntry;
        }
    }
    indexFile.close();
}
