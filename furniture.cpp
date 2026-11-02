#include "furniture.h"
#include <iostream>

const char Furniture::CODE = 'F';

Furniture::Furniture() {
    type = "Unknown";
    height = 0;
    width = 0;
    depth = 0;
    color = "Unknown";
    material = "Unknown";
    cost = 0;
    std::cout << "Constructor for Furniture without params called" << std::endl;
}

Furniture::Furniture(std::string i_type, double i_height, double i_width, double i_depth,
                     std::string i_color, std::string i_material, double i_cost) {
    type = i_type;
    height = i_height;
    width = i_width;
    depth = i_depth;
    color = i_color;
    material = i_material;
    cost = i_cost;
    std::cout << "Constructor for Furniture (" << type << ", " << color << ") with params called" << std::endl;
}

Furniture::Furniture(const Furniture &f) : Base(f) {
    type = f.type;
    height = f.height;
    width = f.width;
    depth = f.depth;
    color = f.color;
    material = f.material;
    cost = f.cost;
    std::cout << "Copy constructor for Furniture (" << type << ", " << color << ") called" << std::endl;
}

Furniture::~Furniture() {
    std::cout << "Destructor for Furniture (" << type << ", " << color << ") called" << std::endl;
}

void Furniture::set_type(std::string new_type) {
    type = new_type;
}

void Furniture::set_height(double new_height) {
    height = new_height;
}

void Furniture::set_width(double new_width) {
    width = new_width;
}

void Furniture::set_depth(double new_depth) {
    depth = new_depth;
}

void Furniture::set_color(std::string new_color) {
    color = new_color;
}

void Furniture::set_material(std::string new_material) {
    material = new_material;
}

void Furniture::set_cost(double new_cost) {
    cost = new_cost;
}

std::string Furniture::get_type() const {
    return type;
}

double Furniture::get_height() const {
    return height;
}

double Furniture::get_width() const {
    return width;
}

double Furniture::get_depth() const {
    return depth;
}

std::string Furniture::get_color() const {
    return color;
}

std::string Furniture::get_material() const {
    return material;
}

double Furniture::get_cost() const {
    return cost;
}

char Furniture::type_code() const {
    return CODE;
}

Base* Furniture::clone() const {
    return new Furniture(*this);
}
