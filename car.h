#pragma once
#include <string>
#include "base.h"

class Car : public Base {
private:
    std::string brand;
    std::string model;
    std::string plate_number;
public:
    static const char CODE;                 // метка вида в файле

    Car();
    Car(std::string brand, std::string model, std::string plate_number);
    Car(const Car &c);
    ~Car();

    void set_brand(std::string new_brand);
    void set_model(std::string new_model);
    void set_plate_number(std::string new_plate_number);

    std::string get_brand() const;
    std::string get_model() const;
    std::string get_plate_number() const;

    char type_code() const override;
    Base* clone() const override;
};
