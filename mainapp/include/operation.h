#ifndef OPERATION_H
#define OPERATION_H
#include <string>

class Operation {
    private:
        std::string name;
        double price;
        int duration;

    public:
        Operation();
        Operation(const std::string& name, double price, int duration);
        std::string getName() const;
        double getPrice() const;
        int getDuration() const;
        void setName(const std::string& name);
        void setPrice(double price);
        void setDuration(int duration);
};

#endif
