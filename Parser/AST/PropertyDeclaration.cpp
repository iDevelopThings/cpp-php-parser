//
// Created by Sam on 10/06/2023.
//

#include "PropertyDeclaration.h"

AST::PropertyDeclaration::PropertyDeclaration(const std::string &name) {
    this->name = name;
}

void AST::PropertyDeclaration::print_info(std::ostream &os, int indent) const {
    std::string ident(indent, ' ');
    os << ident << "PropertyDeclaration: " << this->name << std::endl;
    ident = std::string(indent + 2, ' ');

    if (this->visibility) {
        os << ident << "Visibility: " << this->visibility->value << std::endl;
    }
    if (this->is_optional) {
        os << ident << "Is Optional: true" << std::endl;
    }
    if (!this->modifiers.empty()) {
        os << ident << "Modifiers: ";
        // Join the modifiers with a comma
        for (const auto &modifier: this->modifiers) {
            os << modifier << ", ";
        }
        os << std::endl;
    }
    if (this->type != nullptr) {
        os << ident << "Type: " << this->type << std::endl;
    }
    if (this->value != "") {
        os << ident << "Value: " << this->value << std::endl;
    }
}


void AST::PropertyDeclaration::to_json(std::ostream &os, int indent) {
    std::string ident(indent, ' ');
    os << ident << "{\n";
    os << ident << "  \"type\": \"" << typeid(this).name() << "\",\n";
    os << ident << "  \"name\": \"" << name << "\",\n";
    os << ident << "  \"position\": " << position << ",\n";
    if (this->visibility) {
        os << ident << "  \"visibility\": \"" << this->visibility->value << "\",\n";
    }
    if (this->is_optional) {
        os << ident << "  \"is_optional\": true,\n";
    }
    if (!this->modifiers.empty()) {
        os << ident << "  \"modifiers\": [\n";
        for (const auto &modifier: this->modifiers) {
            os << ident << "    \"" << modifier << "\",\n";
        }
        os << ident << "  ],\n";
    }
    if (this->type != nullptr) {
        os << ident << "  \"type\": \"" << this->type << "\",\n";
    }
    if (this->value != "") {
        os << ident << "  \"value\": \"" << this->value << "\",\n";
    }

    os << ident << "}";
}

