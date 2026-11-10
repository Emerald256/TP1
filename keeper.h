#pragma once
#include <string>
#include "factory_exception.h"
#include "base.h"

// Контейнер для хранения объектов фабрики, сохранения и загрузки
class Keeper {
private:
    Base** items;
    int count;
    void swap_with(Keeper &other);            // Обмен содержимым
    void clear();                             // Удаляет все объекты и массив
public:
    Keeper();
    explicit Keeper(std::string filepath);    // Создает Keeper и сразу загружает данные из файла
    Keeper(const Keeper &k);
    Keeper& operator=(const Keeper &k);
    ~Keeper();

    static int kind_count();                  // Количество видов объектов в фабрике
    static char kind_code_at(int index);      // Код вида по индексу таблицы; у объекта — Base::kind_code()
    static std::string kind_name_at(int index); // Название вида по индексу таблицы; у объекта — Base::kind_name()
    static Base* create_object(char code);    // Создает пустой объект по метке вида

    void add(Base* b);                        // Добавляет заполненный объект в конец массива
    void remove_at(int index);                // Удаляет объект и убирает указатель из массива
    void replace_at(int index, Base* b);      // Заменяет объект по индексу
    Base* get(int index) const;               // Возвращает указатель на объект по индексу (объект фабрики)
    int get_count() const;
    int count_of(char code) const;            // Сколько объектов данного вида

    std::string all_to_string() const;        // Упаковка фабрики в строку
    void load_all_from_string(std::string file_text); // Загрузка фабрики из строки
    void load_from_file(std::string filepath);// Загружает фабрику из файла
    void save_to_file(std::string filepath) const;  // Сохраняет фабрику в файл, создавая резервную копию

    Keeper& operator+=(Base* b);              // Короткая запись add
};
