#include "worker.h"
#include "factory_exception.h"
#include <iostream>
#include <cmath>
#include <new>

const char Worker::CODE = 'W';
const char* const Worker::NAME = "Worker";

Worker::Worker() {
    full_name = "";
    job_title = "";
    // NaN — признак незаполненного числового поля (см. Base::is_complete())
    salary = std::nan("");
    address = "";
    phone_number = "";
     std::cout << "Constructor for Worker without params called (object "
              << static_cast<const void*>(this) << ")" << std::endl;
}

Worker::Worker(std::string new_full_name, std::string new_job_title, double new_salary,
               std::string new_address, std::string new_phone_number) {
    set_full_name(new_full_name);
    set_job_title(new_job_title);
    set_salary(new_salary);
    set_address(new_address);
    set_phone_number(new_phone_number);
   std::cout << "Constructor for Worker (" << full_name << ") with params called (object "
              << static_cast<const void*>(this) << ")" << std::endl;
}

Worker::Worker(const Worker &w) : Base(w) {
    full_name = w.full_name;
    job_title = w.job_title;
    salary = w.salary;
    address = w.address;
    phone_number = w.phone_number;
     std::cout << "Copy constructor for Worker (" << full_name << ") called: source "
              << static_cast<const void*>(&w) << ", new object "
              << static_cast<const void*>(this) << std::endl;
}

Worker::~Worker() {
     std::cout << "Destructor for Worker (" << full_name << ") called (object "
              << static_cast<const void*>(this) << ")" << std::endl;
}

void Worker::set_full_name(std::string new_full_name) {
    check_required(new_full_name, "Worker full name");
    full_name = new_full_name;
}

void Worker::set_job_title(std::string new_job_title) {
    check_required(new_job_title, "Worker job title");
    job_title = new_job_title;
}

void Worker::set_salary(double new_salary) {
    if (new_salary <= 0)
        throw FactoryException("Worker salary must be greater than zero");
    check_min_positive(new_salary, "Worker salary");
    salary = new_salary;
}

void Worker::set_address(std::string new_address) {
    check_required(new_address, "Worker address");
    address = new_address;
}

void Worker::set_phone_number(std::string new_phone_number) {
    check_required(new_phone_number, "Worker phone number");
    phone_number = new_phone_number;
}

std::string Worker::get_full_name() const { return full_name; }
std::string Worker::get_job_title() const { return job_title; }
double Worker::get_salary() const { return salary; }
std::string Worker::get_address() const { return address; }
std::string Worker::get_phone_number() const { return phone_number; }

char Worker::kind_code() const { return CODE; }
std::string Worker::kind_name() const { return NAME; }

int Worker::field_count() const { return 5; }

std::string Worker::field_name(int index) const {
    switch (index) {
        case 0: return "Full name";
        case 1: return "Job title";
        case 2: return "Salary";
        case 3: return "Address";
        case 4: return "Phone number";
    }
    throw FactoryException("Worker field index out of range");
}

std::string Worker::get_field(int index) const {
    switch (index) {
        case 0: return full_name;
        case 1: return job_title;
        case 2: return format_number(salary);
        case 3: return address;
        case 4: return phone_number;
    }
    throw FactoryException("Worker field index out of range");
}

void Worker::set_field(int index, std::string value) {
    switch (index) {
        case 0: set_full_name(value); return;
        case 1: set_job_title(value); return;
        case 2: set_salary(parse_number(value, "Worker salary")); return;
        case 3: set_address(value); return;
        case 4: set_phone_number(value); return;
    }
    throw FactoryException("Worker field index out of range");
}

bool Worker::field_is_numeric(int index) const {
    switch (index) {
        case 2: return true;
        case 0: case 1: case 3: case 4: return false;
    }
    throw FactoryException("Worker field index out of range");
}

Base* Worker::clone() const {
    try { return new Worker(*this); }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to clone Worker");
    }
}
