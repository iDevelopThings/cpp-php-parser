//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_NEWAST_PROPERTYDECLARATION_H
#define INTERPRETER_NEWAST_PROPERTYDECLARATION_H

#include <string>
#include "ASTNode.h"
#include "../../Lexer/NewLexer.h"
#include "FullyQualifiedTypeName.h"


namespace AST {
    class PropertyDeclaration : public ASTNode
    {

    public:
        std::string name;

        bool             is_optional = false;
        std::vector<Tok> modifiers   = {};
        FullyQualifiedTypeName* type = nullptr;
        std::string value = "";
        Tok* visibility = nullptr;

    public:
        explicit PropertyDeclaration(const std::string &name);

        void print_info(std::ostream &os, int indent) const override;

        void to_json(std::ostream &os, int indent) override;
    };
}


#endif //INTERPRETER_NEWAST_PROPERTYDECLARATION_H
