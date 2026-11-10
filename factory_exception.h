#pragma once
#include <stdexcept>
#include <string>

// Исключение программы: message поясняет, что именно произошло
class FactoryException : public std::runtime_error {
public:
    FactoryException(std::string message);
};
