#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <stdexcept>
#include "date.h"
#include "operation.h"
#include "appointment.h"

#define OPERATIONS_FILE "operations.txt"
#define HISTORY_FILE "appointments.txt"
#define MAX_OFFERS 100
#define MAX_APPOINTMENTS 100
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define RESET "\033[0m"
#define PATIENT_NAME "Ion"

Operation offers[MAX_OFFERS];
int numOffers = 0;
Appointment history[MAX_APPOINTMENTS];
int appointmentCount = 0;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void loadOperations() {
    numOffers = 0;
    try {
        std::ifstream fin(OPERATIONS_FILE);
        if (!fin.is_open()) {
            throw std::ios_base::failure("Could not open operations.txt");
        }

        std::string name;
        double price;
        int duration;

        while (fin >> name >> price >> duration) {
            if (numOffers >= MAX_OFFERS) break;
            Operation op;
            op.setName(name);
            op.setPrice(price);
            op.setDuration(duration);
            offers[numOffers++] = op;
        }

        fin.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Error in loadOperations: " << e.what() << "\n";
    }
}

void printAvailableOperations() {
    loadOperations();

    if (numOffers == 0) {
        std::cout << "No available operations.\n";
        return;
    }

    std::cout << "Available Operations:\n";
    for (int i = 0; i < numOffers; i++) {
        std::cout << i + 1 << ". " << offers[i].getName()
            << " | Price: " << offers[i].getPrice()
            << " | Duration: " << offers[i].getDuration() << " mins\n";
    }
}

void loadAppointments(bool allPatients = false) {
    try {
        std::ifstream fin(HISTORY_FILE);
        if (!fin.is_open()) {
            throw std::ios_base::failure("Could not open appointments.txt");
        }

        appointmentCount = 0;
        int day, month, year, hour, minute, opDuration;
        double opPrice;
        std::string opName, patientName;

        while (fin >> patientName >> opName >> opPrice >> opDuration >> day >> month >> year >> hour >> minute) {
            if (appointmentCount >= MAX_APPOINTMENTS) break;
            if (!allPatients && patientName != PATIENT_NAME) continue;

            Date d;
            d.setDay(day);
            d.setMonth(month);
            d.setYear(year);
            d.setHour(hour);
            d.setMinute(minute);

            Operation op;
            op.setName(opName);
            op.setDuration(opDuration);
            op.setPrice(opPrice);

            Appointment app;
            app.setDate(d);
            app.setOperation(op);
            app.setPatientName(patientName);

            history[appointmentCount++] = app;
        }

        fin.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Error in loadAppointments: " << e.what() << "\n";
    }
}
bool isValidOperationName(const std::string& name) {
    for (int i = 0; i < numOffers; ++i) {
        if (offers[i].getName() == name) {
            return true;
        }
    }
    return false;
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDate(int y, int m, int d, int h, int min) {
    if (m < 1 || m > 12 || d < 1 || h < 0 || h > 23 || min < 0 || min > 59)
        return false;

    int daysInMonth[] = { 31, (isLeapYear(y) ? 29 : 28),
                          31, 30, 31, 30,
                          31, 31, 30, 31, 30, 31 };

    return d <= daysInMonth[m - 1];
}

void saveAppointments() {
    try {
        std::ofstream fout(HISTORY_FILE);
        for (int i = 0; i < appointmentCount; i++) {
            Date d = history[i].getDate();
            fout << history[i].getPatientName() << " " << history[i].getOperation().getName() << " " << history[i].getOperation().getPrice() << " "
                << history[i].getOperation().getDuration()
                << " " << d.getDay() << " "
                << d.getMonth() << " " << d.getYear() << " " << d.getHour() << " " << d.getMinute() << "\n";
        }
        fout.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saveAppointments: " << e.what() << "\n";
    }
}

void viewHistory() {
    loadAppointments();
    if (appointmentCount == 0) {
        std::cout << "No appointments scheduled.\n";
    }
    else {
        std::cout << "Appointment History:\n";
        for (int i = 0; i < appointmentCount; i++) {
            Date d = history[i].getDate();
            std::cout << i + 1 << ". " << history[i].getOperation().getName()
                << " on " << BLUE << d.getDay() << "/" << d.getMonth() << "/" << d.getYear() << RESET
                << " at " << GREEN << d.getHour() << ":" << d.getMinute() << RESET << "\n";
        }
    }
}

bool isDateAvailable(int day, int month, int year, int hour, int minute) {
    loadAppointments(true);
    if (!isValidDate(year, month, day, hour, minute)) {
        std::cerr << "Invalid date provided." << RESET << std::endl;
        return false;
    }
    for (int i = 0; i < appointmentCount; i++) {
        Date d = history[i].getDate();
        if (d.getDay() == day && d.getMonth() == month && d.getYear() == year &&
            d.getHour() == hour && d.getMinute() == minute) {
            return false;
        }
    }
    return true;
}

void addAppointment(const std::vector<std::string>& args) {
    try {
        if (args.size() != 11) {
            throw std::invalid_argument("Invalid number of arguments for add_appointment.");
        }
        if (args[2] != PATIENT_NAME) {
            std::cerr << "Error: You can only make appointments for " << PATIENT_NAME << ".\n";
            return;
        }
        std::string patient_name = args[2];
        std::string operation_name = args[3];
        double price = std::stod(args[4]);
        int duration = std::stoi(args[5]);
        int year = std::stoi(args[6]);
        int month = std::stoi(args[7]);
        int day = std::stoi(args[8]);
        int hour = std::stoi(args[9]);
        int minute = std::stoi(args[10]);

        Operation op;
        op.setName(operation_name);
        op.setPrice(price);
        op.setDuration(duration);

        loadOperations();

        if (!isValidOperationName(operation_name)) {
            std::cout << "Error: Operation name '" << operation_name << "' does not exist.\n";
            return;
        }

        for (int i = 0; i < numOffers; ++i) {
            if (offers[i].getName() == operation_name) {
                op = offers[i];
                break;
            }
        }

        Date date;
        date.setYear(year);
        date.setMonth(month);
        date.setDay(day);
        date.setHour(hour);
        date.setMinute(minute);

        loadAppointments(true);

        if (!isDateAvailable(day, month, year, hour, minute)) {
            std::cout << "This time slot is already booked.\n";
            return;
        }

        Appointment app;
        app.setDate(date);
        app.setOperation(op);
        app.setPatientName(PATIENT_NAME);
        history[appointmentCount++] = app;

        saveAppointments();
        std::cout << "Appointment added successfully.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error in addAppointment: " << e.what() << "\n";
    }
}
bool isDateAvailableCommand(const std::vector<std::string>& args) {
    if (args.size() != 7) {
        std::cerr << "Usage: ./app2.exe check_availability <day> <month> <year> <hour> <minute>\n";
        return false;
    }

    int day = std::stoi(args[2]);
    int month = std::stoi(args[3]);
    int year = std::stoi(args[4]);
    int hour = std::stoi(args[5]);
    int minute = std::stoi(args[6]);
    if (!isValidDate(year, month, day, hour, minute)) {
        std::cerr << "Invalid date provided." << RESET << std::endl;
        return false;
    }
    loadAppointments(true);

    for (int i = 0; i < appointmentCount; i++) {
        Date d = history[i].getDate();
        if (d.getDay() == day && d.getMonth() == month && d.getYear() == year &&
            d.getHour() == hour && d.getMinute() == minute) {
            std::cout << "The date is unavailable!" << std::endl;
            return false;
        }
    }

    std::cout << "The date is available!" << std::endl;
    return true;
}
void deleteAppointment(const std::vector<std::string>& args) {
    try {
        if (args.size() != 11) {
            throw std::invalid_argument("Invalid number of arguments for delete_appointment.");
        }
        if (args[2] != PATIENT_NAME) {
            std::cerr << "Error: You can only delete/modify appointments for " << PATIENT_NAME << ".\n";
            return;
        }
        std::string operation_name = args[3];
        int year = std::stoi(args[6]);
        int month = std::stoi(args[7]);
        int day = std::stoi(args[8]);
        int hour = std::stoi(args[9]);
        int minute = std::stoi(args[10]);

        loadAppointments(true);

        bool found = false;
        for (int i = 0; i < appointmentCount; i++) {
            Date d = history[i].getDate();
            if (d.getDay() == day && d.getMonth() == month && d.getYear() == year &&
                d.getHour() == hour && d.getMinute() == minute &&
                history[i].getOperation().getName() == operation_name && history[i].getPatientName() == PATIENT_NAME) {
                for (int j = i; j < appointmentCount - 1; j++) {
                    history[j] = history[j + 1];
                }
                appointmentCount--;
                found = true;
                break;
            }
        }

        if (found) {
            saveAppointments();
            std::cout << "Appointment deleted successfully.\n";
        }
        else {
            std::cout << "Appointment not found.\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error in deleteAppointment: " << e.what() << "\n";
    }
}
int main(int argc, char* argv[]) {
    try {
        std::vector<std::string> args(argv, argv + argc);
        if (argc < 2) {
            throw std::invalid_argument("Usage:\n"
                "./app_2.exe view_appointments\n"
                "./app_2.exe view_operations\n"
                "./app_2.exe add_appointment <patient_name> <operation_name> <price> <duration_in_minutes> <year> <month> <day> <hour> <minute>\n"
                "./app_2.exe delete_appointment <patient_name> <operation_name> <price> <duration_in_minutes> <year> <month> <day> <hour> <minute>\n"
                "./app2.exe check_availability <day> <month> <year> <hour> <minutes>");
        }

        std::string command = args[1];
        if (command == "view_appointments") {
            viewHistory();
        }
        else if (command == "add_appointment") {
            addAppointment(args);
        }
        else if (command == "delete_appointment") {
            deleteAppointment(args);
        }
        else if (command == "check_availability") {
            isDateAvailableCommand(args);
        }
        else if (command == "view_operations") {
            printAvailableOperations();
        }
        else {
            throw std::invalid_argument("Unknown command.");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Application error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
