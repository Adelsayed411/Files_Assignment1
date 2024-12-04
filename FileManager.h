#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std ;
// Structure for Doctor data
struct Doctor {
    int doctorID;
    std::string name;
    std::string specialty;
    // Add other necessary fields as needed

    // Serialization function to write data to file
    void writeToFile(std::fstream& file) const {
        file.write((char*)&doctorID, sizeof(doctorID));
        size_t nameSize = name.size();
        file.write((char*)&nameSize, sizeof(nameSize));
        file.write(name.c_str(), nameSize);

        size_t specialtySize = specialty.size();
        file.write((char*)&specialtySize, sizeof(specialtySize));
        file.write(specialty.c_str(), specialtySize);
    }

    // Deserialization function to read data from file
    void readFromFile(std::fstream& file) {
        file.read((char*)&doctorID, sizeof(doctorID));
        size_t nameSize;
        file.read((char*)&nameSize, sizeof(nameSize));
        name.resize(nameSize);
        file.read(&name[0], nameSize);

        size_t specialtySize;
        file.read((char*)&specialtySize, sizeof(specialtySize));
        specialty.resize(specialtySize);
        file.read(&specialty[0], specialtySize);
    }
};

// Structure for Appointment data
struct Appointment {
    int appointmentID;
    int doctorID;
    std::string patientName;
    std::string dateTime;
    // Add other necessary fields as needed

    // Serialization function to write data to file
    void writeToFile(std::fstream& file) const {
        file.write((char*)&appointmentID, sizeof(appointmentID));
        file.write((char*)&doctorID, sizeof(doctorID));

        size_t patientNameSize = patientName.size();
        file.write((char*)&patientNameSize, sizeof(patientNameSize));
        file.write(patientName.c_str(), patientNameSize);

        size_t dateTimeSize = dateTime.size();
        file.write((char*)&dateTimeSize, sizeof(dateTimeSize));
        file.write(dateTime.c_str(), dateTimeSize);
    }

    // Deserialization function to read data from file
    void readFromFile(std::fstream& file) {
        file.read((char*)&appointmentID, sizeof(appointmentID));
        file.read((char*)&doctorID, sizeof(doctorID));

        size_t patientNameSize;
        file.read((char*)&patientNameSize, sizeof(patientNameSize));
        patientName.resize(patientNameSize);
        file.read(&patientName[0], patientNameSize);

        size_t dateTimeSize;
        file.read((char*)&dateTimeSize, sizeof(dateTimeSize));
        dateTime.resize(dateTimeSize);
        file.read(&dateTime[0], dateTimeSize);
    }
};

// Class to manage file operations for doctors and appointments
class FileManager {
public:
    static void writeDoctor(const Doctor& doctor, const std::string& filename);
    static void readDoctor(Doctor& doctor, const std::string& filename);
    static void writeAppointment(const Appointment& appointment, const std::string& filename);
    static void readAppointment(Appointment& appointment, const std::string& filename);
    void createIndex(const std::string& filename);

};

#endif // FILEMANAGER_H
