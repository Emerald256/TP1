#include "keeper.h"
#include "furniture.h"
#include "worker.h"
#include "car.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <new>

// Структура вида объекта
struct Kind {
    char code;              // Метка вида в файле
    const char* name;       // Название вида для пользователя
    Base* (*make)();        // Создает пустой объект этого вида
};

static Base* make_furniture() { return new Furniture(); }
static Base* make_worker() { return new Worker(); }
static Base* make_car() { return new Car(); }

static const Kind KINDS[] = {
    {Furniture::CODE, Furniture::NAME, make_furniture},
    {Worker::CODE, Worker::NAME, make_worker},
    {Car::CODE, Car::NAME, make_car},
};

static const int KIND_COUNT = sizeof(KINDS) / sizeof(KINDS[0]);

// Проверка на корректность пути
static void check_filepath(const std::string &filepath) {
    if (filepath.find_first_not_of(" \t\r\n") == std::string::npos) {
        throw FactoryException("File path must not be empty");
    }
}

Keeper::Keeper() {
    items = NULL;
    count = 0;
    std::cout << "Constructor for Keeper without params called" << std::endl;
}

Keeper::Keeper(std::string filepath) {
    items = NULL;
    count = 0;
    load_from_file(filepath);
    std::cout << "Constructor for Keeper with filepath \"" << filepath << "\" called" << std::endl;
}

Keeper::Keeper(const Keeper &k) {
    items = NULL;
    count = 0;
    try {
        for (int i = 0; i < k.count; i++) {
            Base* copy = k.items[i]->clone();
            try { add(copy); }
            catch (...) {
                delete copy;
                throw;
            }
        }
    }
    catch (...) {
        clear();
        throw;
    }
    std::cout << "Copy constructor for Keeper called" << std::endl;
}

Keeper& Keeper::operator=(const Keeper &k) {
    if (this == &k) return *this;
    Keeper tmp(k);
    swap_with(tmp);
    std::cout << "Assignment operator for Keeper called" << std::endl;
    return *this;
}

Keeper::~Keeper() {
    clear();
    std::cout << "Destructor for Keeper called" << std::endl;
}

void Keeper::swap_with(Keeper &other) {
    Base** tmp_items = items;
    items = other.items;
    other.items = tmp_items;

    int tmp_count = count;
    count = other.count;
    other.count = tmp_count;
}

void Keeper::clear() {
    for (int i = 0; i < count; i++) {
        delete items[i];
    }
    delete[] items;
    items = NULL;
    count = 0;
}

int Keeper::kind_count() { return KIND_COUNT; }

char Keeper::kind_code_at(int index) {
    if (index < 0 || index >= KIND_COUNT) {
        throw FactoryException("Object kind index out of range");
    }
    return KINDS[index].code;
}

std::string Keeper::kind_name_at(int index) {
    if (index < 0 || index >= KIND_COUNT) {
        throw FactoryException("Object kind index out of range");
    }
    try { return std::string(KINDS[index].name); }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to get object kind name");
    }
}

Base* Keeper::create_object(char code) {
    try {
        for (int i = 0; i < KIND_COUNT; i++) {
            if (KINDS[i].code == code) return KINDS[i].make();
        }
        throw FactoryException(std::string("Unknown object kind: ") + code);
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to create object");
    }
}

void Keeper::add(Base* b) {
    if (b == NULL) {
        throw FactoryException("Cannot add null object");
    }
    b->check_complete();
    for (int i = 0; i < count; i++) {
        if (items[i] == b) {
            throw FactoryException("Object is already in this Keeper");
        }
    }
   Base** new_arr = NULL;
    try { new_arr = new Base*[count + 1]; }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to add object");
    }
    for (int i = 0; i < count; i++) {
        new_arr[i] = items[i];
    }
    new_arr[count] = b;
    delete[] items;
    items = new_arr;
    count++;
}

void Keeper::remove_at(int index) {
    if (index < 0 || index >= count) {
        throw FactoryException("Object index out of range");
    }
    if (count == 1) {
        delete items[index];
        delete[] items;
        items = NULL;
    }
    else {
        Base** new_arr = NULL;
        try { new_arr = new Base*[count - 1]; }
        catch (std::bad_alloc&) {
            throw FactoryException("Not enough memory to remove object");
        }
        int j = 0;
        for (int i = 0; i < count; i++) {
            if (i != index) {
                new_arr[j] = items[i];
                j++;
            }
        }
        delete items[index];
        delete[] items;
        items = new_arr;
    }
    count--;
}

void Keeper::replace_at(int index, Base* b) {
    if (index < 0 || index >= count) {
        throw FactoryException("Object index out of range");
    }
    if (b == NULL) {
        throw FactoryException("Cannot replace with null object");
    }
    b->check_complete();
    if (items[index] == b) return;
    for (int i = 0; i < count; i++) {
        if (items[i] == b) {
            throw FactoryException("Object is already in this Keeper");
        }
    }
    Base* old = items[index];
    items[index] = b;
    delete old;
}

Base* Keeper::get(int index) const {
    if (index < 0 || index >= count) {
        throw FactoryException("Object index out of range");
    }
    return items[index];
}

// Возвращает количество объектов в Keeper
int Keeper::get_count() const { return count; }

int Keeper::count_of(char code) const {
    int n = 0;
    for (int i = 0; i < count; i++) {
        if (items[i]->kind_code() == code) n++;
    }
    return n;
}

// Формат файла:
//   FACTORY
//   F: <количество>
//   <поле1>\x1F<поле2>...   (поля разделены символом 0x1F, записи - переводом строки)
//   W: <количество>
//   ...
//   C: <количество>
//   ...
//   END
// В save_to_file() данные пишутся во временный .tmp, а старая версия сохраняется в .bak.
std::string Keeper::all_to_string() const {
    try {
        std::string res = "FACTORY\n";
        for (int k = 0; k < KIND_COUNT; k++) {
            char code = KINDS[k].code;
            res += std::string(1, code) + ": " + std::to_string(count_of(code)) + "\n";
            for (int i = 0; i < count; i++) {
                if (items[i]->kind_code() == code) {
                    res += items[i]->fields_to_string() + "\n";
                }
            }
        }
        res += "END\n";
        return res;
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to save data");
    }
}

// Вспомогательная функция для чтения следующей строки из потока данных
static std::string next_line(std::istringstream &data_stream) {
    std::string line;
    if (!std::getline(data_stream, line)) {
        throw FactoryException("Invalid file format: unexpected end of file");
    }
    return line;
}

// Вспомогательная функция для преобразования строки в число объектов
static int parse_count(std::string s) {
    size_t pos = 0;
    int v = 0;
    try { v = std::stoi(s, &pos); }
    catch (std::logic_error&) {
        throw FactoryException("Invalid file format: wrong number of objects \"" + s + "\"");
    }
    if (pos != s.size()) {
        throw FactoryException("Invalid file format: wrong number of objects \"" + s + "\"");
    }
    if (v < 0) {
        throw FactoryException("Invalid file format: negative number of objects");
    }
    return v;
}

void Keeper::load_all_from_string(std::string file_text) {
    try {
        std::istringstream text_stream(file_text);
        if (next_line(text_stream) != "FACTORY") {
            throw FactoryException("Invalid file format: no starting tag FACTORY");
        }
        Keeper tmp;
        for (int k = 0; k < KIND_COUNT; k++) {
            char code = KINDS[k].code;
            std::string header = next_line(text_stream); // "F: n"
            if (header.size() < 4 || header[0] != code || header[1] != ':' || header[2] != ' ') {
                throw FactoryException(std::string("Invalid file format: no section ") + code);
            }
            int n = parse_count(header.substr(3));
            for (int i = 0; i < n; i++) {
                std::string object_line = next_line(text_stream);
                Base* obj = create_object(code);
                try {
                    obj->load_fields_from_string(object_line);
                    tmp.add(obj);
                }
                catch (...) {
                    delete obj;
                    throw;
                }
            }
        }
        if (next_line(text_stream) != "END") {
            throw FactoryException("Invalid file format: no ending tag END");
        }
        std::string extra;
        while (std::getline(text_stream, extra)) {
            if (extra.find_first_not_of(" \t\r") != std::string::npos) {
                throw FactoryException("Invalid file format: extra data after END");
            }
        }
        swap_with(tmp);
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to load data");
    }
}

void Keeper::load_from_file(std::string filepath) {
    check_filepath(filepath);
    try {
        std::ifstream file(filepath.c_str());
        if (!file.is_open()) {
            throw FactoryException("Failed to open file \"" + filepath + "\" for reading");
        }
        std::string data, line;
        while (std::getline(file, line)) {
            data += line;
            data += "\n";
        }
        load_all_from_string(data);
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to read file \"" + filepath + "\"");
    }
}

void Keeper::save_to_file(std::string filepath) const {
    check_filepath(filepath);

    std::string tmp_path, bak_path;
    bool target_moved = false;
    try {
        tmp_path = filepath + ".tmp";
        bak_path = filepath + ".bak";
        std::string data = all_to_string();

        std::ofstream file(tmp_path.c_str());
        if (!file.is_open()) {
            throw FactoryException("Failed to open file \"" + tmp_path + "\" for writing");
        }
        file << data;
        file.close();
        if (file.fail()) {
            std::remove(tmp_path.c_str());
            throw FactoryException("Write error! File \"" + filepath + "\" was not saved.");
        }

        std::ifstream check(filepath.c_str());
        bool target_exists = check.good();
        check.close();

        if (target_exists) {
            std::remove(bak_path.c_str());
            if (std::rename(filepath.c_str(), bak_path.c_str()) != 0) {
                std::remove(tmp_path.c_str());
                throw FactoryException("Failed to create backup file \"" + bak_path + "\"");
            }
            target_moved = true;
        }

        if (std::rename(tmp_path.c_str(), filepath.c_str()) != 0) {
            if (target_moved && std::rename(bak_path.c_str(), filepath.c_str()) == 0) {
                target_moved = false;
            }
            std::remove(tmp_path.c_str());
            throw FactoryException("Failed to replace file \"" + filepath + "\"");
        }
        target_moved = false; // Новый файл установлен, .bak остаётся резервной копией
    }
    catch (std::bad_alloc&) {
        if (target_moved) {
            std::rename(bak_path.c_str(), filepath.c_str());
        }
        std::remove(tmp_path.c_str());
        throw FactoryException("Not enough memory to save data");
    }
    catch (...) {
        if (target_moved) {
            std::rename(bak_path.c_str(), filepath.c_str());
        }
        std::remove(tmp_path.c_str());
        throw;
    }
}

Keeper& Keeper::operator+=(Base* b) {
    add(b);
    return *this;
}
