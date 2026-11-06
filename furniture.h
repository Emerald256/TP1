#pragma once
#include <string>
#include "base.h"

class Furniture : public Base {
private:
    std::string type;
    double height, width, depth;
    std::string color;
    std::string material;
    double cost;
public:
    static const char CODE;                 // метка вида в файле

    Furniture();
    Furniture(std::string type, double height, double width, double depth,
              std::string color, std::string material, double cost);
    Furniture(const Furniture &f);
    ~Furniture();

    void set_type(std::string new_type);
    void set_height(double new_height);
    void set_width(double new_width);
    void set_depth(double new_depth);
    void set_color(std::string new_color);
    void set_material(std::string new_material);
    void set_cost(double new_cost);

    std::string get_type() const;
    double get_height() const;
    double get_width() const;
    double get_depth() const;
    std::string get_color() const;
    std::string get_material() const;
    double get_cost() const;

    char type_code() const override;
    Base* clone() const override;
    void print(std::ostream &os) const override;
};
