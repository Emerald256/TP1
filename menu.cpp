#include "menu.h"
#include "factory_exception.h"
#include <iostream>
#include <new>

// Проверка на пустую строку
static bool is_blank(const std::string &s) {
    return s.find_first_not_of(" \t\r\n") == std::string::npos;
}

// Проверка допустимости имени файла
static void check_file_name(const std::string &name) {
    if (is_blank(name)) {
        throw FactoryException("File name must not be empty");
    }
    const std::string forbidden = "<>:\"/\\|?*";
    if (name.find_first_of(forbidden) != std::string::npos) {
        throw FactoryException("File name must not contain path separators or characters < > : \" / \\ | ? *");
    }
    char last = name[name.size() - 1];
    if (last == ' ' || last == '.') {
        throw FactoryException("File name must not end with a space or a dot");
    }
}

// Проверка пути к папке (не пустой)
static void check_folder_path(const std::string &folder) {
    if (is_blank(folder)) {
        throw FactoryException("Folder path must not be empty");
    }
}

// Объединение пути к папке и имени файла в один путь
static std::string join_path(const std::string &folder, const std::string &file) {
    char last = folder[folder.size() - 1];
    if (last == '/' || last == '\\') return folder + file;
    return folder + "\\" + file;
}

// Печать правил ввода имени файла
static void print_file_name_rules() {
    std::cout << "File name rules:" << std::endl;
    std::cout << "- enter only the name, without extension;" << std::endl;
    std::cout << "- the program adds .txt automatically;" << std::endl;
    std::cout << "- path separators and characters < > : \" / \\ | ? * are not allowed;" << std::endl;
    std::cout << "- if you enter an extension, it becomes part of the name (the program still adds .txt)." << std::endl;
}

Menu::Menu(Keeper &k) : keeper(k) {
    std::cout << "Constructor for Menu called" << std::endl;
}

Menu::~Menu() {
    std::cout << "Destructor for Menu called" << std::endl;
}

std::string Menu::ask_line(std::string prompt) {
    std::cout << prompt;
    std::string s;
    if (!std::getline(std::cin, s)) {
        throw std::runtime_error("Input stream closed");
    }
    return s;
}

int Menu::parse_int(std::string s) {
    size_t pos = 0;
    int v = 0;
    try { v = std::stoi(s, &pos); }
    catch (std::logic_error&) {
        throw FactoryException("\"" + s + "\" is not a valid integer");
    }
    if (pos != s.size()) {
        throw FactoryException("\"" + s + "\" is not a valid integer");
    }
    return v;
}

int Menu::ask_int(std::string prompt) {
    while (true) {
        try { return parse_int(ask_line(prompt)); }
        catch (FactoryException &e) { std::cout << "Error: " << e.what() << " Try again." << std::endl; }
    }
}

char Menu::ask_kind(std::string action) {
    int kinds = Keeper::kind_count();
    std::cout << action << ":" << std::endl;
    for (int k = 0; k < kinds; k++) {
        std::cout << k + 1 << ". " << Keeper::kind_name_at(k) << std::endl;
    }
    std::cout << "0. Back" << std::endl << std::endl;
    while (true) {
        int choice = ask_int("Select kind: ");
        if (choice == 0) return 0;
        if (choice >= 1 && choice <= kinds) {
            return Keeper::kind_code_at(choice - 1);
        }
        std::cout << "Incorrect value. Try again." << std::endl;
    }
}

void Menu::print_object(Base* b) {
    try {
        for (int i = 0; i < b->field_count(); i++) {
            std::cout << "- " << b->field_name(i) << ": " << b->get_field(i) << "\n";
        }
    }
    catch (std::bad_alloc&) {
        throw FactoryException("Not enough memory to print object");
    }
}

bool Menu::ask_filepath(std::string &filepath, std::string action) {
    std::cout << action << ":" << std::endl;
    std::cout << "1. File in current folder" << std::endl;
    std::cout << "2. Full path" << std::endl;
    std::cout << "0. Back" << std::endl << std::endl;

    int mode = ask_int("Select: ");
    while (mode < 0 || mode > 2) {
        std::cout << "Incorrect value. Try again." << std::endl;
        mode = ask_int("Select: ");
    }
    if (mode == 0) return false;
    if (mode == 1) {
        print_file_name_rules();
        std::string name = ask_line("File name: ");
        check_file_name(name);
        filepath = name + ".txt";
        return true;
    }
    std::string folder = ask_line("Folder path: ");
    check_folder_path(folder);
    print_file_name_rules();
    std::string name = ask_line("File name: ");
    check_file_name(name);
    filepath = join_path(folder, name + ".txt");
    return true;
}

void Menu::handle_show_all() {
    for (int k = 0; k < Keeper::kind_count(); k++) {
        char code = Keeper::kind_code_at(k);
        std::cout << "   " << Keeper::kind_name_at(k) << " (" << keeper.count_of(code) << ")    " << std::endl;
        int number = 1;
        for (int i = 0; i < keeper.get_count(); i++) {
            Base* b = keeper.get(i);
            if (b->kind_code() == code) {
                std::cout << "#" << number << std::endl;
                print_object(b);
                number++;
            }
        }
    }
}

void Menu::handle_summary() {
    std::cout << "Factory information:";
    for (int k = 0; k < Keeper::kind_count(); k++) {
        std::cout << "\nNumber of " << Keeper::kind_name_at(k) << ": " << keeper.count_of(Keeper::kind_code_at(k));
    }
    std::cout << std::endl;
}

bool Menu::startup_dialog() {
    while (true) {
        std::cout << "=== FACTORY ===\nSelect initialization option:\n\n"
                  << "1 - Create blank Factory\n"
                  << "2 - Load data from file\n"
                  << "0 - Exit\n\n";
        int selection = ask_int("Select: ");
        if (selection == 0) return false;
        if (selection == 1) return true;
        if (selection == 2) {
            if (handle_load()) return true;
            std::cout << "Returning to initialization menu.\n" << std::endl;
            continue;
        }
        std::cout << "Incorrect value. Try again.\n" << std::endl;
    }
}

int Menu::ask_main_choice() {
    std::cout << "\n=== MENU ===\n\n"
              << "1. Add object\n2. Search object (edit / remove)\n"
              << "3. Show all objects\n4. Factory information\n"
              << "5. Save to file\n6. Load from file\n"
              << "0. Exit\n\n";
    while (true) {
        int choice = ask_int("Select: ");
        if (choice >= 0 && choice <= 6) {
            return choice;
        }
        std::cout << "Unknown function " << choice << ". Select available options" << std::endl;
    }
}

void Menu::run() {
    if (!startup_dialog()) return;
    int choice = -1;
    while (choice != 0) {
        choice = ask_main_choice();
        switch (choice) {
            case 1: handle_add(); break;
            case 2: handle_search(); break;
            case 3: handle_show_all(); break;
            case 4: handle_summary(); break;
            case 5: handle_save(); break;
            case 6: handle_load(); break;
            default: break;
        }
    }
}

void Menu::handle_add() {
    Base* obj = NULL;
    try {
        char code = ask_kind("Add");
        if (code == 0) return;
        obj = Keeper::create_object(code);
        for (int i = 0; i < obj->field_count(); i++) {
            while (true) {
                std::string value = ask_line("Input " + obj->field_name(i) + ": ");
                try {
                    obj->set_field(i, value);
                    break;
                }
                catch (FactoryException &e) {
                    std::cout << "Error: " << e.what() << " Try again." << std::endl;
                }
            }
        }
        std::string name = obj->kind_name();
        keeper += obj;
        obj = NULL;
        std::cout << name << " added" << std::endl;
    }
    catch (std::bad_alloc&) {
        delete obj;
        throw FactoryException("Not enough memory to add object");
    }
    catch (...) {
        delete obj;
        throw;
    }
}

int Menu::select_object(char code) {
    int* cand = NULL;
    bool* used = NULL;
    try {
        cand = new int[keeper.get_count()];
        int n = 0;
        for (int i = 0; i < keeper.get_count(); i++) {
            if (keeper.get(i)->kind_code() == code) {
                cand[n] = i;
                n++;
            }
        }
        int result = -1;
        if (n == 0) {
            std::cout << "No objects of this kind." << std::endl;
        }
        else {
            Base* sample = keeper.get(cand[0]);
            int field_count = sample->field_count();
            used = new bool[field_count];
            for (int i = 0; i < field_count; i++) {
                used[i] = false;
            }
            int used_count = 0;
            bool back = false;
            while (n > 1 && !back) {
                std::cout << "Search by:" << std::endl;
                for (int i = 0; i < field_count; i++) {
                    std::cout << i + 1 << ". " << sample->field_name(i) << std::endl;
                }
                std::cout << "0. Back" << std::endl << std::endl;
                int c = ask_int("Select field: ");
                if (c == 0) back = true;
                else if (c < 1 || c > field_count) {
                    std::cout << "Incorrect value. Try again." << std::endl;
                }
                else if (used[c - 1]) {
                    std::cout << "Field \"" << sample->field_name(c - 1) << "\" was already used. Select another field." << std::endl;
                }
                else {
                    std::string field = sample->field_name(c - 1);
                    std::string value = ask_line(field + ": ");
                    if (sample->field_is_numeric(c - 1) && !Base::is_valid_number(value)) {
                        std::cout << "Error: " << field << ": \"" << value << "\" is not a valid number. Returning to menu." << std::endl;
                        back = true;
                        break;
                    }
                    used[c - 1] = true;
                    used_count++;
                    int m = 0;
                    for (int k = 0; k < n; k++) {
                        if (keeper.get(cand[k])->field_equals(c - 1, value)) {
                            cand[m] = cand[k];
                            m++;
                        }
                    }
                    n = m;
                    if (n == 0) std::cout << "Not found" << std::endl;
                    else if (n > 1 && used_count == field_count) {
                        std::cout << "Several identical objects found, the first one is used" << std::endl;
                        break;
                    }
                }
                if (n == 0) break;
            }
            if (!back && n >= 1) result = cand[0];
        }
        delete[] used;
        delete[] cand;
        return result;
    }
    catch (std::bad_alloc&) {
        delete[] used;
        delete[] cand;
        throw FactoryException("Not enough memory to search objects");
    }
    catch (...) {
        delete[] used;
        delete[] cand;
        throw;
    }
}

void Menu::edit_object(int index) {
    Base* tmp = NULL;
    std::string name;
    try {
        Base* original = keeper.get(index);
        name = original->kind_name();
        tmp = original->clone();

        std::cout << "Input new values. All fields are required." << std::endl;
        for (int i = 0; i < tmp->field_count(); i++) {
            while (true) {
                std::string value = ask_line(tmp->field_name(i) + ": ");
                try {
                    tmp->set_field(i, value);
                    break;
                }
                catch (FactoryException &e) {
                    std::cout << "Error: " << e.what() << " Try again." << std::endl;
                }
            }
        }

        std::cout << "New values:" << std::endl;
        print_object(tmp);
        std::cout << "\n1. Save changes\n0. Discard changes\n";
        int choice = ask_int("Select: ");
        while (choice != 0 && choice != 1) {
            std::cout << "Incorrect value. Try again." << std::endl;
            choice = ask_int("Select: ");
        }
        if (choice == 0) {
            delete tmp;
            std::cout << "Changes discarded." << std::endl;
            return;
        }
        keeper.replace_at(index, tmp);
        tmp = NULL;
        std::cout << name << " updated" << std::endl;
        print_object(keeper.get(index));
    }
    catch (std::bad_alloc&) {
        delete tmp;
        throw FactoryException("Not enough memory to edit object");
    }
    catch (...) {
        delete tmp;
        throw;
    }
}

void Menu::handle_search() {
    char code = ask_kind("Search");
    if (code == 0) return;
    int index = select_object(code);
    if (index < 0) return;

    Base* b = keeper.get(index);
    std::cout << b->kind_name() << " found\n" << std::endl;
    print_object(b);
    std::cout << "\n1. Edit\n2. Remove\n\n0. Back to menu\n";
    int choice = ask_int("Select task: ");
    while (choice < 0 || choice > 2) {
        std::cout << "Incorrect value. Try again." << std::endl;
        choice = ask_int("Select task: ");
    }
    if (choice == 1) edit_object(index);
    else if (choice == 2) {
        std::string name = b->kind_name();
        keeper.remove_at(index);
        std::cout << name << " removed" << std::endl;
    }
}

void Menu::handle_save() {
    try {
        std::string filepath;
        if (!ask_filepath(filepath, "Save to file")) return;
        keeper.save_to_file(filepath);
        std::cout << "File saved successfully: " << filepath << std::endl;
    }
    catch (FactoryException &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    catch (std::bad_alloc&) {
        std::cout << "Error: not enough memory" << std::endl;
    }
}

bool Menu::handle_load() {
    try {
        std::string filepath;
        if (!ask_filepath(filepath, "Load from file")) return false;
        keeper.load_from_file(filepath);
        std::cout << "Data loaded successfully: " << filepath << std::endl;
        return true;
    }
    catch (FactoryException &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    catch (std::bad_alloc&) {
        std::cout << "Error: not enough memory" << std::endl;
    }
    return false;
}
