#include "date.h"

Date::Date()
{
    year = 0;
    month = 0;
    day = 0;
    hour = 0;
    minute = 0;
}

Date::Date(int year, int month, int day, int hour, int minute)
{
    this->year = year;
    this->month = month;
    this->day = day;
    this->hour = hour;
    this->minute = minute;
}

int Date::getYear() const
{
    return year;
}

int Date::getMonth() const
{
    return month;
}

int Date::getDay() const
{
    return day;
}

int Date::getHour() const
{
    return hour;
}

int Date::getMinute() const
{
    return minute;
}

void Date::setYear(int year)
{
    this->year = year;
}

void Date::setMonth(int month)
{
    this->month = month;
}

void Date::setDay(int day)
{
    this->day = day;
}

void Date::setHour(int hour)
{
    this->hour = hour;
}

void Date::setMinute(int minute)
{
    this->minute = minute;
}
