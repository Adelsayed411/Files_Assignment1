#ifndef UNTITLED25_INDEXMANAGER_H
#define UNTITLED25_INDEXMANAGER_H

#include <fstream>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <utility> // For std::pair
#include "Doctor.h"
#include "Appointment.h"

class IndexManager {
public:
    static void openIndexFiles();
    static void closeIndexFiles();
    static void addDoctorToIndexFile(const Doctor& doctor, std::streampos position);
    static void addAppointmentToIndexFile(const Appointment& appointment, std::streampos position);
    static void writeIndexFile();
    static void readIndexFile();

    static const std::map<std::string, std::streampos>& getDoctorPrimaryIndex();
    static const std::map<std::string, std::streampos>& getAppointmentPrimaryIndex();
    static const std::map<std::string, std::list<std::string>>& getDoctorNameSecondaryIndex();
    static const std::map<std::string, std::list<std::string>>& getDoctorIDSecondaryIndex();

    // New functions to remove records from indexes
    static void removeDoctorFromIndexFile(const std::string& doctorID);
    static void removeAppointmentFromIndexFile(const std::string& appointmentID);

    // New functions to manage the avail list
    static void addToAvailList(const std::string& recordType, std::streampos position, size_t size);
    static std::vector<std::pair<std::streampos, size_t>> getAvailList(const std::string& recordType);
    static void writeAvailListFiles();
    static void readAvailListFiles();

// New member variables for avail lists
static std::vector<std::pair<std::streampos, size_t>> doctorAvailList;
private:
    static std::ofstream doctorPrimaryIndexFile;
    static std::ofstream appointmentPrimaryIndexFile;
    static std::ofstream doctorNameSecondaryIndexFile;
    static std::ofstream appointmentDoctorIDSecondaryIndexFile;

    static std::map<std::string, std::streampos> doctorPrimaryIndex;
    static std::map<std::string, std::streampos> appointmentPrimaryIndex;
    static std::map<std::string, std::list<std::string>> doctorNameSecondaryIndex;
    static std::map<std::string, std::list<std::string>> doctorIDSecondaryIndex;

    static std::vector<std::pair<std::streampos, size_t>> appointmentAvailList;

    static void writePrimaryIndexFile();
    static void writeSecondaryIndexFiles();
    static void readPrimaryIndexFile();
    static void readSecondaryIndexFiles();
};

#endif // UNTITLED25_INDEXMANAGER_H
