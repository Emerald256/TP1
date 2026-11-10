#pragma once
#include <string>
#include "base.h"

// Работник: ФИО, должность, заработная плата, адрес, телефон
class Worker : public Base {
private:
    std::string full_name;
    std::string job_title;
    double salary;
    std::string address;
    std::string phone_number;
public:
    static const char CODE;                 // Метка вида в файле
    static const char* const NAME;          // Название вида

    Worker();
    Worker(std::string new_full_name, std::string new_job_title, double new_salary, std::string new_address, std::string new_phone_number);
    Worker(const Worker &w);
    ~Worker();

    void set_full_name(std::string new_full_name);
    void set_job_title(std::string new_job_title);
    void set_salary(double new_salary);
    void set_address(std::string new_address);
    void set_phone_number(std::string new_phone_number);

    std::string get_full_name() const;
    std::string get_job_title() const;
    double get_salary() const;
    std::string get_address() const;
    std::string get_phone_number() const;

    char kind_code() const override;
    std::string kind_name() const override;
    int field_count() const override;
    std::string field_name(int index) const override;
    std::string get_field(int index) const override;
    void set_field(int index, std::string value) override;
    bool field_is_numeric(int index) const override;
    Base* clone() const override;
};
