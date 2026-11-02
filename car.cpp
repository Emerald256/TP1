#include "car.h"
#include <iostream>

const char Car::CODE = 'C';

Car::Car() {
    brand = "Unknown";
    model = "Unknown";
    plate_number = "Unknown";
    std::cout << "Constructor for Car without params called" << std::endl;
}

Car::Car(std::string i_brand, std::string i_model, std::string i_plate_number) {
    brand = i_brand;
    model = i_model;
    plate_number = i_plate_number;
    std::cout << "Constructor for Car (" << plate_number << ") with params called" << std::endl;
}

Car::Car(const Car &c) : Base(c) {
    brand = c.brand;
    model = c.model;
    plate_number = c.plate_number;
    std::cout << "Copy constructor for Car (" << plate_number << ") called" << std::endl;
}

Car::~Car() {
    std::cout << "Destructor for Car (" << plate_number << ") called" << std::endl;
}

void Car::set_brand(std::string new_brand) {
    brand = new_brand;
}

void Car::set_model(std::string new_model) {
    model = new_model;
}

void Car::set_plate_number(std::string new_plate_number) {
    plate_number = new_plate_number;
}

std::string Car::get_brand() const {
    return brand;
}

std::string Car::get_model() const {
    return model;
}

std::string Car::get_plate_number() const {
    return plate_number;
}

char Car::type_code() const {
    return CODE;
}

Base* Car::clone() const {
    return new Car(*this);
}
