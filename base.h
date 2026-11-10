#pragma once
#include <string>

const char FIELD_SEP = '\x1F';                                // Символ-разделитель полей в строке

// Абстрактный базовый класс всех объектов фабрики (мебель, работник, машина)
class Base {
public:
    Base();
    Base(const Base &b);
    virtual ~Base();

    virtual char kind_code() const = 0;                       // Код вида объекта
    virtual std::string kind_name() const = 0;                // Название вида объекта
    virtual int field_count() const = 0;                      // Количество полей
    virtual std::string field_name(int index) const = 0;      // Название поля
    virtual std::string get_field(int index) const = 0;       // Значение поля в виде строки
    virtual void set_field(int index, std::string value) = 0; // Запись поля
    virtual bool field_is_numeric(int index) const = 0;       // True, если поле числовое
    virtual Base* clone() const = 0;                          // Полная копия объекта

    // Сравнение значения поля с введённой строкой: числа сравниваются как числа, текстовые поля - как строки
    bool field_equals(int index, const std::string& value) const;

    // Проверка того, что строка является допустимым числом
    static bool is_valid_number(const std::string& value);

    bool is_complete() const;                                 // Все поля заполнены?
    void check_complete() const;                              // Ошибка, если есть незаполненные поля

    virtual std::string fields_to_string() const;             // Упаковка в строку
    virtual void load_fields_from_string(std::string fields_line); // Загрузка полей из строки

protected:
    // Проверка поля на соответствие требованиям
    void check_required(std::string value, std::string field_name);

    // Отрезает от data первое поле до разделителя и возвращает его
    std::string next_field(std::string &data, char delim);

    // Преобразования строки в число
    static double parse_number(std::string s, std::string field_name);

    // Проверка, что число не меньше минимального значения
    static void check_min_positive(double value, std::string field_name);

    // Форматирование числа под нужный вид
    static std::string format_number(double v);
};
