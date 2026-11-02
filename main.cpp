#include <iostream>
#include <stdexcept>
#include "keeper.h"
#include "furniture.h"
#include "worker.h"
#include "car.h"

void print_object(Base* b) {
    if (b->type_code() == Furniture::CODE) {
        Furniture* f = static_cast<Furniture*>(b);
        std::cout << "Furniture: " << f->get_type() << ", " << f->get_height() << "x" << f->get_width() << "x"
                  << f->get_depth() << ", " << f->get_color() << ", " << f->get_material()
                  << ", cost " << f->get_cost() << std::endl;
    }
    else if (b->type_code() == Worker::CODE) {
        Worker* w = static_cast<Worker*>(b);
        std::cout << "Worker: " << w->get_full_name() << ", " << w->get_job_title() << ", salary " << w->get_salary()
                  << ", " << w->get_address() << ", " << w->get_phone_number() << std::endl;
    }
    else if (b->type_code() == Car::CODE) {
        Car* c = static_cast<Car*>(b);
        std::cout << "Car: " << c->get_brand() << " " << c->get_model() << ", " << c->get_plate_number() << std::endl;
    }
}

void print_all(Keeper &keeper) {
    for (int i = 0; i < keeper.get_count(); i++) {
        print_object(keeper.get(i));
    }
}

void test_add(Keeper &keeper) {
    std::cout << "\n Add objects " << std::endl;
    keeper.add(new Furniture("Table", 75, 120, 60, "Brown", "Oak", 15000));
    keeper.add(new Worker("Ivanov Ivan Ivanovich", "Carpenter", 65000, "Lenina st. 5", "+7 900 111-22-33"));
    keeper += new Car("Toyota", "Camry", "A123BC");

    Base* b = Keeper::create_object('C');
    Car* c = static_cast<Car*>(b);
    c->set_brand("Lada");
    c->set_model("Vesta");
    c->set_plate_number("B456CD");
    keeper.add(b);

    print_all(keeper);
}

void test_copy(Keeper &keeper) {
    std::cout << "\n Copy and remove " << std::endl;
    Keeper copy(keeper);
    copy.remove_at(0);
    std::cout << "\nOriginal:\n";
    print_all(keeper);
    std::cout << "\nCopy after remove:\n";
    print_all(copy);
}

int main() {
    try {
        Keeper keeper;
        test_add(keeper);
        test_copy(keeper);
        std::cout << "\nDone" << std::endl;
    }
    catch (std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
