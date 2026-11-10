#include "furniture.h"
#include "factory_exception.h"
#include <iostream>
#include <cmath>
#include <new>

const char Furniture::CODE = 'F';
const char* const Furniture::NAME = "Furniture";

Furniture::Furniture() {
    type = "";
   // Числовые поля по умолчанию NaN, чтобы is_complete() отличал незаполненный объект от заполненного
    height = width = depth = std::nan("");
    color = "";
    material = "";
    cost = std::nan("");
    std::cout << "Constructor for Furniture without params called (object "
              << static_cast<const void*>(this) << ")" << std::endl;
}

Furniture::Furniture(std::string new_type, double new_height, double new_width, double new_depth,
                     std::string new_color, std::string new_material, double new_cost) {
    set_type(new_type);
    set_height(new_height);
    set_width(new_width);
    set_depth(new_depth);
    set_color(new_color);
    set_material(new_material);
    set_cost(new_cost);
    std::cout << "Constructor for Furniture (" << type << ", " << color << ") with params called (object "
              << static_cast<const void*>(this) << ")" << std::endl;
}

Furniture::Furniture(const Furniture &f) : Base(f) {
    type = f.type;
    height = f.height;
    width = f.width;
    depth = f.depth;
    color = f.color;
    material = f.material;
    cost = f.cost;
    std::cout << "Copy constructor for Furniture (" << type << ", " << color << ") called: source "
              << static_cast<const void*>(&f) << ", new object "
              << static_cast<const void*>(this) << std::endl;
}

Furniture::~Furniture() {
    std::cout << "Destructor for Furniture (" << type << ", " << color << ") called (object "
              << static_cast<const void*>(this) << ")" << std::endl;
}

void Furniture::set_type(std::string new_type) {
    check_required(new_type, "Furniture type");
    type = new_type;
}

void Furniture::set_height(double new_height) {
    if (new_height <= 0)
        throw FactoryException("Furniture height must be greater than zero");
    check_min_positive(new_height, "Furniture height");
    height = new_height;
}

void Furniture::set_width(double new_width) {
    if (new_width <= 0)
        throw FactoryException("Furniture width must be greater than zero");
    check_min_positive(new_width, "Furniture width");
    width = new_width;
}

void Furniture::set_depth(double new_depth) {
    if (new_depth <= 0)
        throw FactoryException("Furniture depth must be greater than zero");
    check_min_positive(new_depth, "Furniture depth");
    depth = new_depth;
}

void Furniture::set_color(std::string new_color) {
    check_required(new_color, "Furniture color");
    color = new_color;
}

void Furniture::set_material(std::string new_material) {
    check_required(new_material, "Furniture material");
    material = new_material;
}

void Furniture::set_cost(double new_cost) {
    if (new_cost <= 0)
        throw FactoryException("Furniture cost must be greater than zero");
    check_min_positive(new_cost, "Furniture cost");
    cost = new_cost;
}

std::string Furniture::get_type() const { return type; }
double Furniture::get_height() const { return height; }
double Furniture::get_width() const { return width; }
double Furniture::get_depth() const { return depth; }
std::string Furniture::get_color() const { return color; }
std::string Furniture::get_material() const { return material; }
double Furniture::get_cost() const { return cost; }

char Furniture::kind_code() const { return CODE; }
std::string Furniture::kind_name() const { return NAME; }

int Furniture::field_count() const { return 7; }

std::string Furniture::field_name(int index) const {
    switch (index) {
        case 0: return "Type";
        case 1: return "Height";
        case 2: return "Width";
        case 3: return "Depth";
        case 4: return "Color";
        case 5: return "Material";
        case 6: return "Cost";
    }
    throw FactoryException("Furniture field index out of range");
}

std::string Furniture::get_field(int index) const {
    switch (index) {
        case 0: return type;
        case 1: return format_number(height);
        case 2: return format_number(width);
        case 3: return format_number(depth);
        case 4: return color;
        case 5: return material;
        case 6: return format_number(cost);
    }
    throw FactoryException("Furniture field index out of range");
}

void Furniture::set_field(int index, std::string value) {
    switch (index) {
        case 0: set_type(value); return;
        case 1: set_height(parse_number(value, "Furniture height")); return;
        case 2: set_width(parse_number(value, "Furniture width")); return;
        case 3: set_depth(parse_number(value, "Furniture depth")); return;
        case 4: set_color(value); return;
        case 5: set_material(value); return;
        case 6: set_cost(parse_number(value, "Furniture cost")); return;
    }
    throw FactoryException("Furniture field index out of range");
}

bool Furniture::field_is_numeric(int index) const {
    switch (index) {
        case 1: case 2: case 3: case 6: return true;
        case 0: case 4: case 5: return false;
    }
    throw FactoryException("Furniture field index out of range");
}

Base* Furniture::clone() const {
    try { return new Furniture(*this); }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to clone Furniture");
    }
}

