#ifndef UNTITLED25_INDEXMANAGER_H
#define UNTITLED25_INDEXMANAGER_H

#include <fstream>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <utility> 
#include "Doctor.h"
#include "Appointment.h"

class IndexManager {
public:
    static void openIndexFiles();
    static void closeIndexFiles();
    static void addDoctorToIndexFile(const Doctor& doctor, streampos position);
    static void addAppointmentToIndexFile(const Appointment& appointment, streampos position);
    static void writeIndexFile();
    static void readIndexFile();

    static const map<string, streampos>& getDoctorPrimaryIndex();
    static const map<string, streampos>& getAppointmentPrimaryIndex();
    static const map<string, list<string>>& getDoctorNameSecondaryIndex();
    static const map<string, list<string>>& getDoctorIDSecondaryIndex();

    // New functions to remove records from indexes
    static void removeDoctorFromIndexFile(const string& doctorID);
    static void removeAppointmentFromIndexFile(const string& appointmentID);

    // New functions to manage the avail list
    static void addToAvailList(const string& recordType, streampos position, size_t size);
    static vector<pair<streampos, size_t>> getAvailList(const string& recordType);
    static void writeAvailListFiles();
    static void readAvailListFiles();

    // New member variables for avail lists
    static vector<pair<streampos, size_t>> doctorAvailList;
    static vector<pair<streampos, size_t>> appointmentAvailList;


private:
    static ofstream doctorPrimaryIndexFile;
    static ofstream appointmentPrimaryIndexFile;
    static ofstream doctorNameSecondaryIndexFile;
    static ofstream appointmentDoctorIDSecondaryIndexFile;

    static map<string, streampos> doctorPrimaryIndex;
    static map<string, streampos> appointmentPrimaryIndex;
    static map<string, list<string>> doctorNameSecondaryIndex;
    static map<string, list<string>> doctorIDSecondaryIndex;

    static void writePrimaryIndexFile();
    static void writeSecondaryIndexFiles();
    static void readPrimaryIndexFile();
    static void readSecondaryIndexFiles();
};

#endif // UNTITLED25_INDEXMANAGER_H