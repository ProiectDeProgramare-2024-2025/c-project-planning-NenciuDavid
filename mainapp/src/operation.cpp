#include "operation.h"

Operation::Operation()
{
    name = "";
    price = 0.0;
    duration = 0;
}

Operation::Operation(const std::string& name, double price, int duration)
{
    this->name = name;
    this->price = price;
    this->duration = duration;
}

std::string Operation::getName() const
{
    return name;
}

double Operation::getPrice() const
{
    return price;
}

int Operation::getDuration() const
{
    return duration;
}

void Operation::setName(const std::string& name)
{
    this->name = name;
}

void Operation::setPrice(double price)
{
    this->price = price;
}

void Operation::setDuration(int duration)
{
    this->duration = duration;
}
