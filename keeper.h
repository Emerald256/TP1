#pragma once
#include <string>
#include "factory_exception.h"
#include "base.h"

class Keeper {
private:
    Base** items;
    int count;
    void swap_with(Keeper &other); // обмен содержимым
    void clear();                  // удаляет все объекты и массив
public:
    Keeper();
    Keeper(const Keeper &k);
    Keeper& operator=(const Keeper &k);
    ~Keeper();

    static Base* create_object(char code);    // создает пустой объект по метке вида

    void add(Base* b);                // добавляет указатель в конец (невозможно добавление одного и того же объекта без clone())
    void remove_at(int index);        // удаляет объект и убирает указатель из массива
    Base* get(int index) const;
    int get_count() const;

    Keeper& operator+=(Base* b);      // короткая запись add
};
