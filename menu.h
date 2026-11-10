#pragma once
#include <string>
#include "base.h"
#include "keeper.h"

// Пользовательское меню для работы с контейнером
class Menu {
private:
    Keeper &keeper;

    std::string ask_line(std::string prompt);                     // Запрос на ввод строки
    int ask_int(std::string prompt);                              // Запрос на ввод целого числа
    int parse_int(std::string s);                                 // Преобразование строки в целое число, если это возможно
    char ask_kind(std::string action);                            // Запрос на ввод вида объекта (меню) для действия
    void print_object(Base* b);                                   // Печать объекта
    bool ask_filepath(std::string &filepath, std::string action); // Запрос на ввод пути к файлу

    // Диалог при запуске программы: загрузка данных из файла или создание пустой фабрики
    bool startup_dialog();

    int ask_main_choice();                                        // Запрос на ввод действия в главном меню
    void handle_add();                                            // Добавление объекта в фабрику

    // Запрос на поиск объекта в фабрике -> редактирование или удаление найденного объекта
    void handle_search();

    void handle_show_all();                                       // Печать всех объектов в фабрике
    void handle_summary();                                        // Печать сводной информации о фабрике
    void handle_save();                                           // Сохранение данных в файл
    bool handle_load();                                           // Загрузка данных из файла

    int select_object(char code);                                 // Поиск объекта в фабрике по фильтрам
    void edit_object(int index);                                  // Редактирование объекта в фабрике по индексу
public:
    Menu(Keeper &k);
    ~Menu();
    Menu(const Menu &) = delete;
    Menu& operator=(const Menu &) = delete;

    void run();                                                   // Запуск главного меню
};
