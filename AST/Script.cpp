//
// Created by Sam on 10/06/2023.
//

#include "Script.h"

#include <utility>

namespace AST {
    Script::Script(const std::string &file_name) : ASTNode() {
        this->file_name = file_name;
    }

    void Script::print_info(std::ostream &os, int indent) const {
        std::string ident(indent, ' ');

        os << ident << "Script: " << this->file_name << std::endl;

        if (!nodes.empty()) {
            os << ident << "Statements: " << std::endl;
            for (auto node: nodes) {
                node->print_info(os, indent + 2);
            }
        }

    }

    void AST::Script::to_json(std::ostream &os, int indent) {
        std::string ident(indent, ' ');
        os << ident << "{\n";
        os << ident << "  \"type\": \"" << typeid(this).name() << "\",\n";
        os << ident << "  \"file_name\": \"" << file_name << "\",\n";
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
