#pragma once
#include <string>
#include "base.h"

// Машина: марка, модель, государственный номер
class Car : public Base {
private:
    std::string brand;
    std::string model;
    std::string plate_number;
public:
    static const char CODE;                 // Метка вида в файле
    static const char* const NAME;          // Название вида

    Car();
    Car(std::string new_brand, std::string new_model, std::string new_plate_number);
    Car(const Car &c);
    ~Car();

    void set_brand(std::string new_brand);
    void set_model(std::string new_model);
    void set_plate_number(std::string new_plate_number);

    std::string get_brand() const;
    std::string get_model() const;
    std::string get_plate_number() const;

    char kind_code() const override;
    std::string kind_name() const override;
    int field_count() const override;
    std::string field_name(int index) const override;
    std::string get_field(int index) const override;
    void set_field(int index, std::string value) override;
    bool field_is_numeric(int index) const override;
    Base* clone() const override;
};
