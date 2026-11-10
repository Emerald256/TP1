#include "car.h"
#include "factory_exception.h"
#include <iostream>
#include <new>

const char Car::CODE = 'C';
const char* const Car::NAME = "Car";

Car::Car() {
    brand = "";
    model = "";
    plate_number = "";
    std::cout << "Constructor for Car without params called (object "
              << static_cast<const void*>(this) << ")" << std::endl;
}

Car::Car(std::string new_brand, std::string new_model, std::string new_plate_number) {
    set_brand(new_brand);
    set_model(new_model);
    set_plate_number(new_plate_number);
    std::cout << "Constructor for Car (" << plate_number << ") with params called (object "
              << static_cast<const void*>(this) << ")" << std::endl;
}

Car::Car(const Car &c) : Base(c) {
    brand = c.brand;
    model = c.model;
    plate_number = c.plate_number;
    std::cout << "Copy constructor for Car (" << plate_number << ") called: source "
              << static_cast<const void*>(&c) << ", new object "
              << static_cast<const void*>(this) << std::endl;
}

Car::~Car() {
     std::cout << "Destructor for Car (" << plate_number << ") called (object "
              << static_cast<const void*>(this) << ")" << std::endl;
}

void Car::set_brand(std::string new_brand) {
    check_required(new_brand, "Car brand");
    brand = new_brand;
}

void Car::set_model(std::string new_model) {
    check_required(new_model, "Car model");
    model = new_model;
}

void Car::set_plate_number(std::string new_plate_number) {
    check_required(new_plate_number, "Car plate number");
    plate_number = new_plate_number;
}

std::string Car::get_brand() const { return brand; }
std::string Car::get_model() const { return model; }
std::string Car::get_plate_number() const { return plate_number; }

char Car::kind_code() const { return CODE; }
std::string Car::kind_name() const { return NAME; }

int Car::field_count() const { return 3; }

std::string Car::field_name(int index) const {
    switch (index) {
        case 0: return "Brand";
        case 1: return "Model";
        case 2: return "Plate number";
    }
    throw FactoryException("Car field index out of range");
}

std::string Car::get_field(int index) const {
    switch (index) {
        case 0: return brand;
        case 1: return model;
        case 2: return plate_number;
    }
    throw FactoryException("Car field index out of range");
}

void Car::set_field(int index, std::string value) {
    switch (index) {
        case 0: set_brand(value); return;
        case 1: set_model(value); return;
        case 2: set_plate_number(value); return;
    }
    throw FactoryException("Car field index out of range");
}

bool Car::field_is_numeric(int index) const {
    switch (index) { case 0: case 1: case 2: return false; }
    throw FactoryException("Car field index out of range");
}

Base* Car::clone() const {
    try { return new Car(*this); }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to clone Car");
    }
}
