#include "FileManager.h"
#include <iostream>
#include <cstring>

using namespace std;

fstream doctorsFile, appointmentsFile;

void openFiles() {
    doctorsFile.open("doctors.dat", ios::in | ios::out | ios::binary);
    if (!doctorsFile) {
        doctorsFile.open("doctors.dat", ios::out);
        doctorsFile.close();
        doctorsFile.open("doctors.dat", ios::in | ios::out | ios::binary);
    }

    appointmentsFile.open("appointments.dat", ios::in | ios::out | ios::binary);
    if (!appointmentsFile) {
        appointmentsFile.open("appointments.dat", ios::out);
        appointmentsFile.close();
        appointmentsFile.open("appointments.dat", ios::in | ios::out | ios::binary);
    }
}

void closeFiles() {
    if (doctorsFile.is_open()) doctorsFile.close();
    if (appointmentsFile.is_open()) appointmentsFile.close();
}

void writeDoctorRecord(const Doctor& doctor) {
    std::string record = string(doctor.doctorID) + "|" +
                         string(doctor.doctorName) + "|" +
                         string(doctor.address) + "\n";
    doctorsFile.seekp(0, ios::end);
    doctorsFile.write(record.c_str(), record.size());
}

Doctor readDoctorRecord(streampos position) {
    Doctor doctor;
    string record;
    doctorsFile.seekg(position);
    getline(doctorsFile, record);

    size_t pos1 = record.find('|');
    size_t pos2 = record.find('|', pos1 + 1);

    strncpy(doctor.doctorID, record.substr(0, pos1).c_str(), 15);
    strncpy(doctor.doctorName, record.substr(pos1 + 1, pos2 - pos1 - 1).c_str(), 30);
    strncpy(doctor.address, record.substr(pos2 + 1).c_str(), 30);

    return doctor;
}

void writeAppointmentRecord(const Appointment& appointment) {
    string record = string(appointment.appointmentID) + "|" +
                         string(appointment.appointmentDate) + "|" +
                         string(appointment.doctorID) + "\n";
    appointmentsFile.seekp(0, ios::end);
    appointmentsFile.write(record.c_str(), record.size());
}

Appointment readAppointmentRecord(streampos position) {
    Appointment appointment;
    string record;
    appointmentsFile.seekg(position);
    getline(appointmentsFile, record);

    size_t pos1 = record.find('|');
    size_t pos2 = record.find('|', pos1 + 1);

    strncpy(appointment.appointmentID, record.substr(0, pos1).c_str(), 15);
    strncpy(appointment.appointmentDate, record.substr(pos1 + 1, pos2 - pos1 - 1).c_str(), 30);
    strncpy(appointment.doctorID, record.substr(pos2 + 1).c_str(), 15);

    return appointment;
}
