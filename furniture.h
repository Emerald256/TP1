#pragma once
#include <string>
#include "base.h"

// Мебель: тип, габариты, цвет, материал, стоимость
class Furniture : public Base {
private:
    std::string type;
    double height, width, depth;
    std::string color;
    std::string material;
    double cost;
public:
    static const char CODE;                 // Метка вида в файле
    static const char* const NAME;          // Название вида

    Furniture();
    Furniture(std::string new_type, double new_height, double new_width, double new_depth,
              std::string new_color, std::string new_material, double new_cost);
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

    char kind_code() const override;
    std::string kind_name() const override;
    int field_count() const override;
    std::string field_name(int index) const override;
    std::string get_field(int index) const override;
    void set_field(int index, std::string value) override;
    bool field_is_numeric(int index) const override;
    Base* clone() const override;
};
