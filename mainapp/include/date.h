#ifndef DATE_H
#define DATE_H

class Date {
    private:
        int year;
        int month;
        int day;
        int hour;
        int minute;

    public:
        Date();
        Date(int year, int month, int day, int hour, int minute);
        int getYear() const;
        int getMonth() const;
        int getDay() const;
        int getHour() const;
        int getMinute() const;
        void setYear(int year);
        void setMonth(int month);
        void setDay(int day);
        void setHour(int hour);
        void setMinute(int minute);
};

#endif
