#include <iostream>
#include "keeper.h"
#include "menu.h"

int main() {
    try {
        Keeper keeper;
        Menu menu(keeper);
        menu.run();
        std::cout << "Exit" << std::endl;
    }
    catch (std::exception &e) {
        std::cout << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
