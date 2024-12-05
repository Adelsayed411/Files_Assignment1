//
// Created by Access on 12/3/2024.
//

#ifndef UNTITLED25_APPOINTMENT_H
#define UNTITLED25_APPOINTMENT_H
struct Appointment {
    char appointmentID[15]; // Primary Key
    char appointmentDate[30];
    char doctorID[15];      // Secondary Key
};

#endif //UNTITLED25_APPOINTMENT_H
