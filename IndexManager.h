#ifndef UNTITLED25_INDEXMANAGER_H
#define UNTITLED25_INDEXMANAGER_H

#include <fstream>
#include <map>
#include <list>
#include <string>
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

    // Accessors for primary and secondary indexes
    static const std::map<std::string, std::streampos>& getDoctorPrimaryIndex();
    static const std::map<std::string, std::streampos>& getAppointmentPrimaryIndex();
    static const std::map<std::string, std::list<std::string>>& getDoctorNameSecondaryIndex();
    static const std::map<std::string, std::list<std::string>>& getDoctorIDSecondaryIndex();

private:
    static std::ofstream doctorPrimaryIndexFile;
    static std::ofstream appointmentPrimaryIndexFile;
    static std::ofstream doctorNameSecondaryIndexFile;
    static std::ofstream appointmentDoctorIDSecondaryIndexFile;

    static std::map<std::string, std::streampos> doctorPrimaryIndex;
    static std::map<std::string, std::streampos> appointmentPrimaryIndex;
    static std::map<std::string, std::list<std::string>> doctorNameSecondaryIndex;
    static std::map<std::string, std::list<std::string>> doctorIDSecondaryIndex;

    static void writePrimaryIndexFile();
    static void writeSecondaryIndexFiles();
    static void readPrimaryIndexFile();
    static void readSecondaryIndexFiles();
};

#endif //UNTITLED25_INDEXMANAGER_H
