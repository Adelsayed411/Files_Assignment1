
#ifndef UNTITLED27_FILEMANAGER_H
#define UNTITLED27_FILEMANAGER_H
#include <fstream>
#include <string>
#include "Doctor.h"
#include "Appointment.h"

void openFiles();
void closeFiles();
void writeDoctorRecord(const Doctor& doctor);
Doctor readDoctorRecord(std::streampos position);
void writeAppointmentRecord(const Appointment& appointment);
Appointment readAppointmentRecord(std::streampos position);


#endif