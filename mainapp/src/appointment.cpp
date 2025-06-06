#include "appointment.h"

Appointment::Appointment()
{
    operation = Operation();
    date = Date();
    patientName = "";
}

Appointment::Appointment(const Operation& operation, const Date& date, const std::string& patientName)
{
    this->operation = operation;
    this->date = date;
    this->patientName = patientName;
}

Operation Appointment::getOperation() const
{
    return operation;
}

Date Appointment::getDate() const
{
    return date;
}

std::string Appointment::getPatientName() const
{
    return patientName;
}

void Appointment::setOperation(const Operation& operation)
{
    this->operation = operation;
}

void Appointment::setDate(const Date& date)
{
    this->date = date;
}

void Appointment::setPatientName(const std::string& name)
{
    this->patientName = name;
}
