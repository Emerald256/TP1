#include "worker.h"
#include <iostream>

const char Worker::CODE = 'W';

Worker::Worker() {
    full_name = "Unknown";
    job_title = "Unknown";
    salary = 0;
    address = "Unknown";
    phone_number = "Unknown";
    std::cout << "Constructor for Worker without params called" << std::endl;
}

Worker::Worker(std::string i_full_name, std::string i_job_title, double i_salary,
               std::string i_address, std::string i_phone_number) {
    full_name = i_full_name;
    job_title = i_job_title;
    salary = i_salary;
    address = i_address;
    phone_number = i_phone_number;
    std::cout << "Constructor for Worker (" << full_name << ") with params called" << std::endl;
}

Worker::Worker(const Worker &w) : Base(w) {
    full_name = w.full_name;
    job_title = w.job_title;
    salary = w.salary;
    address = w.address;
    phone_number = w.phone_number;
    std::cout << "Copy constructor for Worker (" << full_name << ") called" << std::endl;
}

Worker::~Worker() {
    std::cout << "Destructor for Worker (" << full_name << ") called" << std::endl;
}

void Worker::set_full_name(std::string new_full_name) {
    full_name = new_full_name;
}

void Worker::set_job_title(std::string new_job_title) {
    job_title = new_job_title;
}

void Worker::set_salary(double new_salary) {
    salary = new_salary;
}

void Worker::set_address(std::string new_address) {
    address = new_address;
}

void Worker::set_phone_number(std::string new_phone_number) {
    phone_number = new_phone_number;
}

std::string Worker::get_full_name() const {
    return full_name;
}

std::string Worker::get_job_title() const {
    return job_title;
}

double Worker::get_salary() const {
    return salary;
}

std::string Worker::get_address() const {
    return address;
}

std::string Worker::get_phone_number() const {
    return phone_number;
}

char Worker::type_code() const {
    return CODE;
}

Base* Worker::clone() const {
    return new Worker(*this);
}

void Worker::print(std::ostream &os) const {
    os << "Worker: " << full_name << ", " << job_title << ", salary " << salary
       << ", " << address << ", " << phone_number;
}
