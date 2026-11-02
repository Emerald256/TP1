#include "base.h"
#include <iostream>

Base::Base() {
    std::cout << "Constructor for Base class called" << std::endl;
}

Base::Base(const Base &) {
    std::cout << "Copy constructor for Base class called" << std::endl;
}

Base::~Base() {
    std::cout << "Destructor for Base class called" << std::endl;
}
