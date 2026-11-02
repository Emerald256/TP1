#pragma once
#include <string>
#include "base.h"

class Worker : public Base {
private:
    std::string full_name;
    std::string job_title;
    double salary;
    std::string address;
    std::string phone_number;
public:
    static const char CODE;                 // метка вида в файле

    Worker();
    Worker(std::string full_name, std::string job_title, double salary, std::string address, std::string phone_number);
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

    char type_code() const override;
    Base* clone() const override;
};
