#include "factory_exception.h"

FactoryException::FactoryException(std::string message) : std::runtime_error(message) {}
