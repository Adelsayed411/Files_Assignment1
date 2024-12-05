#ifndef UNTITLED25_FILEMANAGER_H
#define UNTITLED25_FILEMANAGER_H

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "Doctor.h"
#include "Appointment.h"
#include "IndexManager.h"
using namespace std;

// File management functions
void openFiles();
void closeFiles();
void writeDoctorRecord(const Doctor& doctor);
Doctor readDoctorRecord(streampos position);
void writeAppointmentRecord(const Appointment& appointment);
Appointment readAppointmentRecord(streampos position);

// Search functions
Doctor searchDoctorByID(const string& doctorID);
Appointment searchAppointmentByID(const string& appointmentID);
vector<streampos> searchDoctorByName(const string& doctorName);
vector<streampos> searchAppointmentsByDoctorID(const string& doctorID);

// AVAIL LIST management functions
void addToAvailList(streampos position, size_t recordSize);
bool getAvailPositionAndSize(size_t recordSize, streampos& position);

// Delete functions
void deleteDoctorRecord(const string& doctorID);
void deleteAppointmentRecord(const string& appointmentID);

#endif // UNTITLED25_FILEMANAGER_H
