//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_FUNCTIONDECLARATION_H
#define INTERPRETER_FUNCTIONDECLARATION_H


#include <string>
#include "ASTNode.h"
#include "../Lexer/Token.h"

namespace AST {

    class FunctionDeclaration : public ASTNode {

    public:
        std::string name;
        std::vector<Token*> parameters;
        Token* visibility = nullptr;
        std::vector<Token*> return_types_tokens = {};
    public:
        explicit FunctionDeclaration(const std::string &name);

        void add_parameter(Token* pToken);

        void print_info(std::ostream &os, int indent) const override;

        void to_json(std::ostream &os, int indent) override;

    };

}


#endif //INTERPRETER_FUNCTIONDECLARATION_H
