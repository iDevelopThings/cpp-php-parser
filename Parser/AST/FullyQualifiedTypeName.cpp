//
// Created by Sam on 11/06/2023.
//

#include "FullyQualifiedTypeName.h"

namespace AST {
    FullyQualifiedTypeName::FullyQualifiedTypeName(const std::string &name) {
        this->name = name;
    }

    void FullyQualifiedTypeName::print_info(std::ostream &os, int indent) const {
        std::string ident(indent, ' ');

        os << ident << "FullyQualifiedTypeName: " << this->name << std::endl;

        if (!nodes.empty()) {
            os << ident << "Statements: " << std::endl;
            for (auto node: nodes) {
                node->print_info(os, indent + 2);
            }
        }
    }

    void FullyQualifiedTypeName::to_json(std::ostream &os, int indent) {
        std::string ident(indent, ' ');
        os << ident << "{\n";
        os << ident << R"(  "type": ")" << typeid(this).name() << "\",\n";
        os << ident << R"(  "name": ")" << name << "\",\n";
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
} // AST
