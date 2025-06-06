#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "appointment.h"
#include "operation.h"
#include "date.h"

using namespace std;

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

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

vector<Operation> loadOperations(const string& filename) {
    ifstream in(filename);
    vector<Operation> operations;

    if (!in.is_open()) {
        cerr << RED << "Error opening " << filename << " for reading." << RESET << endl;
        return operations;
    }

    string name;
    double price;
    int duration;
    while (in >> name >> price >> duration) {
        operations.emplace_back(name, price, duration);
    }

    in.close();
    return operations;
}

vector<Appointment> loadAppointments(const string& filename) {
    ifstream in(filename);
    vector<Appointment> appointments;

    if (!in.is_open()) {
        cerr << RED << "Error opening " << filename << " for reading." << RESET << endl;
        return appointments;
    }

    string name, opName;
    double price;
    int dur, y, m, d, h, min;
    while (in >> name >> opName >> price >> dur >> d >> m >> y >> h >> min) {
        Operation op(opName, price, dur);
        Date date(y, m, d, h, min);
        appointments.emplace_back(op, date, name);
    }

    in.close();
    return appointments;
}

void saveAppointments(const vector<Appointment>& appointments, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << RED << "Error opening " << filename << " for writing." << RESET << endl;
        return;
    }

    for (const auto& a : appointments) {
        out << a.getPatientName() << " " << a.getOperation().getName() << " "
            << a.getOperation().getPrice() << " " << a.getOperation().getDuration() << " "
            << a.getDate().getDay() << " " << a.getDate().getMonth() << " " << a.getDate().getYear() << " "
            << a.getDate().getHour() << " " << a.getDate().getMinute() << "\n";
    }

    out.close();
}

bool appointmentsEqual(const Appointment& a1, const Appointment& a2) {
    return a1.getPatientName() == a2.getPatientName() && a1.getOperation().getName() == a2.getOperation().getName() &&
           a1.getOperation().getPrice() == a2.getOperation().getPrice() &&
           a1.getOperation().getDuration() == a2.getOperation().getDuration() &&
           a1.getDate().getYear() == a2.getDate().getYear() &&
           a1.getDate().getMonth() == a2.getDate().getMonth() &&
           a1.getDate().getDay() == a2.getDate().getDay() &&
           a1.getDate().getHour() == a2.getDate().getHour() &&
           a1.getDate().getMinute() == a2.getDate().getMinute();
}

int main(int argc, char* argv[]) {
    string fileName = "appointments.txt";
    vector<Appointment> appointments = loadAppointments(fileName);
    vector<Operation> availableOps = loadOperations("operations.txt");

    if (argc < 2) {
        cerr << RED << "Not enough arguments." << RESET << endl;
        return 1;
    }

    string command = argv[1];

    try {
        if (command == "view_appointments") {
            for (const auto& a : appointments) {
                cout << GREEN << a.getPatientName() << RESET << " - " << a.getOperation().getName()
                     << " (" << a.getOperation().getPrice() << " lei, "
                     << a.getOperation().getDuration() << " min) at "
                     << setfill('0') << setw(2) << a.getDate().getDay() << "/"
                     << setw(2) << a.getDate().getMonth() << "/"
                     << a.getDate().getYear() << " "
                     << setw(2) << a.getDate().getHour() << ":"
                     << setw(2) << a.getDate().getMinute() << endl;
            }
        } else if (command == "add_appointment" && argc == 11) {
            string name = argv[2];
            string opName = argv[3];
            double price = stod(argv[4]);
            int duration = stoi(argv[5]);
            int d = stoi(argv[6]), m = stoi(argv[7]), y = stoi(argv[8]), h = stoi(argv[9]), min = stoi(argv[10]);
            if (!isValidDate(y, m, d, h, min)) {
                cerr << RED << "Invalid date provided." << RESET << endl;
                return 1;
            }
            auto it = find_if(availableOps.begin(), availableOps.end(), [&](const Operation& o) {
                return o.getName() == opName;
            });

            if (it == availableOps.end()) {
                cerr << RED << "Operation not found in operations.txt." << RESET << endl;
                return 1;
            }

            Operation op = *it;
            Date date(y, m, d, h, min);
            Appointment newApp(op, date, name);

            appointments.push_back(newApp);
            saveAppointments(appointments, fileName);
            cout << GREEN << "Appointment added successfully." << RESET << endl;
        } else if (command == "delete_appointment" && argc == 11) {
            string name = argv[2];
            string opName = argv[3];
            double price = stod(argv[4]);
            int duration = stoi(argv[5]);
            int d = stoi(argv[6]), m = stoi(argv[7]), y = stoi(argv[8]), h = stoi(argv[9]), min = stoi(argv[10]);
            if (!isValidDate(y, m, d, h, min)) {
                cerr << RED << "Invalid date provided." << RESET << endl;
                return 1;
            }
            Operation op(opName, price, duration);
            Date date(y, m, d, h, min);
            Appointment toDelete(op, date, name);

            auto it = remove_if(appointments.begin(), appointments.end(), [&](const Appointment& a) {
                return appointmentsEqual(a, toDelete);
            });

            if (it != appointments.end()) {
                appointments.erase(it, appointments.end());
                saveAppointments(appointments, fileName);
                cout << GREEN << "Appointment deleted." << RESET << endl;
            } else {
                cout << RED << "Appointment not found." << RESET << endl;
            }
        } else {
            cerr << RED << "Invalid command or arguments." << RESET << endl;
        }

    } catch (const exception& e) {
        cerr << RED << "Error: " << e.what() << RESET << endl;
        return 1;
    }

    return 0;
}
