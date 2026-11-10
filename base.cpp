#include "base.h"
#include "factory_exception.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <new>

// Попытка преобразовать строку в число, возвращает true, если удалось
static bool try_parse_number(const std::string &s, double &value) {
    size_t pos = 0;
    try {
        value = std::stod(s, &pos);
    }
    catch (std::logic_error&) {
        return false;
    }
    // Сравнение с самим собой ложно для NaN
    return pos == s.size() && value == value;
}

static const double MIN_POSITIVE_VALUE = 0.000005;

Base::Base() {
    std::cout << "Constructor for Base class called (object "
              << static_cast<const void*>(this) << ")" << std::endl;
}

Base::Base(const Base &b) {
    std::cout << "Copy constructor for Base class called: source "
              << static_cast<const void*>(&b) << ", new object "
              << static_cast<const void*>(this) << std::endl;
}

Base::~Base() {
    std::cout << "Destructor for Base class called (object "
              << static_cast<const void*>(this) << ")" << std::endl;
}

std::string Base::fields_to_string() const {
    try {
        std::string res;
        for (int i = 0; i < field_count(); i++) {
            if (i > 0) res += FIELD_SEP;
            res += get_field(i);
        }
        return res;
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to serialize object");
    }
}

// Загрузка объекта из строки, поля разделены символом FIELD_SEP
void Base::load_fields_from_string(std::string fields_line) {
    try {
        for (int i = 0; i < field_count(); i++) {
            set_field(i, next_field(fields_line, FIELD_SEP));
        }
        if (fields_line != "") {
            throw FactoryException("Invalid data in file: too many fields in a record");
        }
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to load object");
    }
}

std::string Base::next_field(std::string &data, char delim) {
    try {
        size_t pos = data.find(delim);
        std::string res;
        if (pos == std::string::npos) {
            res = data;
            data = "";
        }
        else {
            res = data.substr(0, pos);
            data = data.substr(pos + 1);
        }
        return res;
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to read a field");
    }
}

void Base::check_required(std::string value, std::string field_name) {
    try {
        if (value.find(FIELD_SEP) != std::string::npos) {
            throw FactoryException(field_name + " must not contain the service character");
        }
        if (value.find('\n') != std::string::npos || value.find('\r') != std::string::npos) {
            throw FactoryException(field_name + " must not contain line breaks");
        }
        if (value == "") {
            throw FactoryException(field_name + " must not be empty");
        }
        if (value.find_first_not_of(" \t\r\n") == std::string::npos) {
            throw FactoryException(field_name + " must not contain only spaces");
        }
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to check " + field_name);
    }
}

double Base::parse_number(std::string s, std::string field_name) {
    try {
        double v = 0;
        if (!try_parse_number(s, v)) {
            throw FactoryException(field_name + ": \"" + s + "\" is not a valid number");
        }
        return v;
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to parse " + field_name);
    }
}

bool Base::field_equals(int index, const std::string& value) const {
    try {
        std::string current = get_field(index);
        if (!field_is_numeric(index)) return current == value;
        double current_number = 0;
        double value_number = 0;
        if (!try_parse_number(current, current_number) || !try_parse_number(value, value_number)) {
            return false;
        }
        return current_number == value_number;
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to compare fields");
    }
}

void Base::check_min_positive(double value, std::string field_name) {
    try {
        if (value > 0 && value < MIN_POSITIVE_VALUE) {
            throw FactoryException(field_name + " is too small: minimum positive value is 0.000005");
        }
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to check " + field_name);
    }
}

bool Base::is_valid_number(const std::string& value) {
    double number = 0;
    return try_parse_number(value, number);
}

std::string Base::format_number(double v) {
    try {
        std::ostringstream os;
        os << std::fixed << std::setprecision(5) << v;
        std::string res = os.str();

        size_t dot = res.find('.');
        if (dot != std::string::npos) {
            size_t last = res.size();
            while (last > dot + 1 && res[last - 1] == '0') {
                last--;
            }
            if (last == dot + 1) last--;
            res.erase(last);
        }
        if (res == "-0") res = "0";
        return res;
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to format a number");
    }
}

bool Base::is_complete() const {
    try {
        for (int i = 0; i < field_count(); i++) {
            std::string value = get_field(i);
            if (field_is_numeric(i)) {
                double number = 0;
                if (!try_parse_number(value, number)) return false;
            }
            else if (value.empty()) return false;
        }
        return true;
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to check object completeness");
    }
}

void Base::check_complete() const {
    try {
        if (!is_complete()) {
            throw FactoryException(kind_name() + " has empty fields");
        }
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to check object completeness");
    }
}
