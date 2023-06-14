//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_NEWAST_CLASSDECLARATION_H
#define INTERPRETER_NEWAST_CLASSDECLARATION_H


#include <string>
#include "ASTNode.h"
#include "FullyQualifiedTypeName.h"
#include "Attribute.h"
#include "BaseObjectDeclaration.h"

namespace AST {

    class ClassDeclaration : public BaseObjectDeclaration
    {

    public:
        // std::string name;
        // std::string                          extends;
        // std::vector<std::string>             implements = {};
        // std::vector<FullyQualifiedTypeName*> traits     = {};
        // Attribute* attribute;

        bool is_abstract = false;
    public:
        explicit ClassDeclaration(const std::string &name);

        void print_info(std::ostream &os, int indent) const override;

        void to_json(std::ostream &os, int indent) override;

    };

}

#endif //INTERPRETER_NEWAST_CLASSDECLARATION_H
