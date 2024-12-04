#ifndef INDEXMANAGER_H
#define INDEXMANAGER_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "Doctor.h"
#include "Appointment.h"
using namespace std;

// Primary Index structure
struct PrimaryIndexEntry {
    string key;  // Doctor ID or Appointment ID
    streampos position;  // Position of the record in the data file
};

// Secondary Index structure (linked list entry)
struct SecondaryIndexEntry {
    streampos position;  // Position of the record in the data file
    SecondaryIndexEntry* next;  // Pointer to the next entry in the linked list
};

// Class to manage all types of indexes
class IndexManager {
public:
    // Constructor
    IndexManager();
    
    // Destructor
    ~IndexManager();

    // Functions for primary index management
    void createDoctorsPrimaryIndex(const string& fileName);
    void createAppointmentsPrimaryIndex(const string& fileName);

    // Functions for secondary index management
    void createDoctorsSecondaryIndex(const string& fileName);
    void createAppointmentsSecondaryIndex(const string& fileName);

    // Add entries to the indexes
    void addPrimaryIndexEntry(const string& fileName, const string& key, streampos position);
    void addSecondaryIndexEntry(const string& fileName, const string& key, streampos position);

    // Search for an index entry by key
    streampos searchPrimaryIndex(const string& fileName, const string& key);
    vector<streampos> searchSecondaryIndex(const string& fileName, const string& key);

    // Delete an index entry
    void deleteIndexEntry(const string& fileName, const string& key);
    void saveDoctorsIndex();

private:
    // Internal maps to store primary indexes
    map<string, PrimaryIndexEntry> doctorsPrimaryIndex;
    map<string, PrimaryIndexEntry> appointmentsPrimaryIndex;

    // Internal maps to store secondary indexes (linked lists)
    map<string, SecondaryIndexEntry*> doctorsSecondaryIndex;
    map<string, SecondaryIndexEntry*> appointmentsSecondaryIndex;

    // Helper functions
    void writeIndexToFile(const string& fileName, const map<string, PrimaryIndexEntry>& indexMap);
    void writeSecondaryIndexToFile(const string& fileName, const map<string, SecondaryIndexEntry*>& indexMap);
    void loadIndexFromFile(const string& fileName, map<string, PrimaryIndexEntry>& indexMap);
    void loadSecondaryIndexFromFile(const string& fileName, map<string, SecondaryIndexEntry*>& indexMap);
};

#endif // INDEXMANAGER_H
