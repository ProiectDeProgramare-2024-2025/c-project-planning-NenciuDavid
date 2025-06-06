#ifndef APPOINTMENT_H
#define APPOINTMENT_H
#include <string>
#include "Operation.h"
#include "Date.h"

class Appointment {
    private:
        Operation operation;
        Date date;
        std::string patientName;

    public:
        Appointment();
        Appointment(const Operation& operation, const Date& date, const std::string& patientName);
        Operation getOperation() const;
        Date getDate() const;
        std::string getPatientName() const;
        void setOperation(const Operation& operation);
        void setDate(const Date& date);
        void setPatientName(const std::string& name);
};

#endif
