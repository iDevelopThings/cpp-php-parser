//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_NEWAST_FUNCTIONDECLARATION_H
#define INTERPRETER_NEWAST_FUNCTIONDECLARATION_H


#include <string>
#include "ASTNode.h"
#include "../../Lexer/NewLexer.h"
#include "Parameter.h"
#include "Statement.h"

namespace AST {

    class FunctionDeclaration : public Statement
    {

    public:
        std::string             name;
        std::vector<Parameter*> parameters;
        Tok* visibility = nullptr;
        std::vector<Tok*> return_types_tokens = {};
        BlockStatement* body;
    public:
        explicit FunctionDeclaration(const std::string &name);

        void print_info(std::ostream &os, int indent) const override;

        void to_json(std::ostream &os, int indent) override;

    };

}


#endif //INTERPRETER_NEWAST_FUNCTIONDECLARATION_H
