//
// Created by Sam on 11/06/2023.
//

#include "Parameter.h"

namespace AST {
    Parameter::Parameter(std::string name, FullyQualifiedTypeName* type, std::string value) {
        this->name  = std::move(name);
        this->type  = type;
        this->value = std::move(value);
    }

    Parameter::Parameter(std::string name, FullyQualifiedTypeName* type) {
        this->name  = std::move(name);
        this->type  = type;
        this->value = "";
    }

    Parameter::Parameter(std::string name, std::string value) {
        this->name  = std::move(name);
        this->type  = nullptr;
        this->value = std::move(value);
    }

    Parameter::Parameter(std::string value) {
        this->name  = "";
        this->type  = nullptr;
        this->value = std::move(value);
    }

    Parameter::Parameter() {
        this->name  = "";
        this->type  = nullptr;
        this->value = "";
    }
} // AST
