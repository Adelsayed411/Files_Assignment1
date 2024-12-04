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
    std::string key;  // Doctor ID or Appointment ID
    std::streampos position;  // Position of the record in the data file
};

// Secondary Index structure (linked list entry)
struct SecondaryIndexEntry {
    std::streampos position;  // Position of the record in the data file
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
    void createDoctorsPrimaryIndex(const std::string& fileName);
    void createAppointmentsPrimaryIndex(const std::string& fileName);

    // Functions for secondary index management
    void createDoctorsSecondaryIndex(const std::string& fileName);
    void createAppointmentsSecondaryIndex(const std::string& fileName);

    // Add entries to the indexes
    void addPrimaryIndexEntry(const std::string& fileName, const std::string& key, std::streampos position);
    void addSecondaryIndexEntry(const std::string& fileName, const std::string& key, std::streampos position);

    // Search for an index entry by key
    std::streampos searchPrimaryIndex(const std::string& fileName, const std::string& key);
    std::vector<std::streampos> searchSecondaryIndex(const std::string& fileName, const std::string& key);

    // Delete an index entry
    void deleteIndexEntry(const std::string& fileName, const std::string& key);

private:
    // Internal maps to store primary indexes
    std::map<std::string, PrimaryIndexEntry> doctorsPrimaryIndex;
    std::map<std::string, PrimaryIndexEntry> appointmentsPrimaryIndex;

    // Internal maps to store secondary indexes (linked lists)
    std::map<std::string, SecondaryIndexEntry*> doctorsSecondaryIndex;
    std::map<std::string, SecondaryIndexEntry*> appointmentsSecondaryIndex;

    // Helper functions
    void writeIndexToFile(const std::string& fileName, const std::map<std::string, PrimaryIndexEntry>& indexMap);
    void writeSecondaryIndexToFile(const std::string& fileName, const std::map<std::string, SecondaryIndexEntry*>& indexMap);
    void loadIndexFromFile(const std::string& fileName, std::map<std::string, PrimaryIndexEntry>& indexMap);
    void loadSecondaryIndexFromFile(const std::string& fileName, std::map<std::string, SecondaryIndexEntry*>& indexMap);
};

#endif // INDEXMANAGER_H
