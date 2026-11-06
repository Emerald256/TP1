#pragma once
#include <string>
#include <iosfwd>

// Абстрактный базовый класс всех объектов фабрики (мебель, работник, машина)
class Base {
public:
    Base();
    Base(const Base &b);
    virtual ~Base();

    virtual char type_code() const = 0;                       // метка секции в файле
    virtual Base* clone() const = 0;                          // полная копия объекта
    virtual void print(std::ostream &os) const = 0;           // вывод объекта в поток
};
