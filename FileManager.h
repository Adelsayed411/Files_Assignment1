#ifndef UNTITLED25_FILEMANAGER_H
#define UNTITLED25_FILEMANAGER_H

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "Doctor.h"
#include "Appointment.h"
#include "IndexManager.h" // Include IndexManager for search functionality

void openFiles();
void closeFiles();
void writeDoctorRecord(const Doctor& doctor);
Doctor readDoctorRecord(std::streampos position);
void writeAppointmentRecord(const Appointment& appointment);
Appointment readAppointmentRecord(std::streampos position);

Doctor searchDoctorByID(const std::string& doctorID);
Appointment searchAppointmentByID(const std::string& appointmentID);
std::vector<std::streampos> searchDoctorByName(const std::string& doctorName);
std::vector<std::streampos> searchAppointmentsByDoctorID(const std::string& doctorID);

#endif // UNTITLED25_FILEMANAGER_H
