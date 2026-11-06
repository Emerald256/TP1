#pragma once
#include <stdexcept>
#include <string>

// Исключение программы. Текст message поясняет, что именно произошло.
class FactoryException : public std::runtime_error {
public:
    FactoryException(std::string message) : std::runtime_error(message) {
    }
};
