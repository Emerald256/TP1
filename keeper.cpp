#include "keeper.h"
#include <stdexcept>
#include "furniture.h"
#include "worker.h"
#include "car.h"
#include <iostream>

Keeper::Keeper() {
    items = NULL;
    count = 0;
    std::cout << "Constructor for Keeper without params called" << std::endl;
}

Keeper::Keeper(const Keeper &k) {
    items = NULL;
    count = 0;
    for (int i = 0; i < k.count; i++) {
        add(k.items[i]->clone());
    }
    std::cout << "Copy constructor for Keeper called" << std::endl;
}

Keeper& Keeper::operator=(const Keeper &k) {
    if (this == &k) {
        return *this;
    }
    Keeper tmp(k);
    swap_with(tmp);
    std::cout << "Assignment operator for Keeper called" << std::endl;
    return *this;
}

Keeper::~Keeper() {
    clear();
    std::cout << "Destructor for Keeper called" << std::endl;
}

void Keeper::swap_with(Keeper &other) {
    Base** tmp_items = items;
    items = other.items;
    other.items = tmp_items;

    int tmp_count = count;
    count = other.count;
    other.count = tmp_count;
}

void Keeper::clear() {
    for (int i = 0; i < count; i++) {
        delete items[i];
    }
    delete[] items;
    items = NULL;
    count = 0;
}

Base* Keeper::create_object(char code) {
    if (code == Furniture::CODE) {
        return new Furniture();
    }
    if (code == Worker::CODE) {
        return new Worker();
    }
    if (code == Car::CODE) {
        return new Car();
    }
    throw std::invalid_argument(std::string("Unknown object kind: ") + code);
}

void Keeper::add(Base* b) {
    Base** new_arr = new Base*[count + 1];
    for (int i = 0; i < count; i++) {
        new_arr[i] = items[i];
    }
    new_arr[count] = b;
    delete[] items;
    items = new_arr;
    count++;
}

void Keeper::remove_at(int index) {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Object index out of range");
    }
    delete items[index];
    if (count == 1) {
        delete[] items;
        items = NULL;
    }
    else {
        Base** new_arr = new Base*[count - 1];
        int j = 0;
        for (int i = 0; i < count; i++) {
            if (i != index) {
                new_arr[j] = items[i];
                j++;
            }
        }
        delete[] items;
        items = new_arr;
    }
    count--;
}

Base* Keeper::get(int index) const {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Object index out of range");
    }
    return items[index];
}

int Keeper::get_count() const {
    return count;
}

Keeper& Keeper::operator+=(Base* b) {
    add(b);
    return *this;
}
