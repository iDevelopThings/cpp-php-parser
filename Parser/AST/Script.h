//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_NEWAST_SCRIPT_H
#define INTERPRETER_NEWAST_SCRIPT_H

#include <string>
#include "ASTNode.h"

namespace AST {

    class Script : public ASTNode {

    public:
        std::string file_name;

    public:
        explicit Script(const std::string &file_name);

        virtual void print_info(std::ostream &os, int indent) const override;

        void to_json(std::ostream &os, int indent) override;
    };

} // AST

#endif //INTERPRETER_NEWAST_SCRIPT_H
