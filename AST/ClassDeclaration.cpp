//
// Created by Sam on 10/06/2023.
//

#include "ClassDeclaration.h"

#include <utility>

AST::ClassDeclaration::ClassDeclaration(const std::string &name) {
    this->name = name;
}

void AST::ClassDeclaration::setExtends(std::string pToken) {
    this->extends = std::move(pToken);
}

void AST::ClassDeclaration::print_info(std::ostream &os, int indent) const {
    std::string ident(indent, ' ');

    os << ident << "ClassDeclaration: " << this->name << std::endl;

    if (!nodes.empty()) {
        os << ident << "Statements: " << std::endl;
        for (auto node: nodes) {
            node->print_info(os, indent + 2);
        }
    }
}

void AST::ClassDeclaration::to_json(std::ostream &os, int indent) {
    std::string ident(indent, ' ');
    os << ident << "{\n";
    os << ident << "  \"type\": \"" << typeid(this).name() << "\",\n";
    os << ident << "  \"name\": \"" << name << "\",\n";
    os << ident << "  \"position\": " << position << ",\n";
    if (!nodes.empty()) {
        os << ident << "  \"nodes\": [\n";
        for (auto node: nodes) {
            node->to_json(os, indent + 4);
            os << ",\n";
        }
        os << ident << "  ]\n";
    } else {
        os << ident << "  \"nodes\": []\n";
    }
    os << ident << "}";
}

