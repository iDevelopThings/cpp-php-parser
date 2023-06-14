//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_NEWAST_NAMESPACEDECLARATION_H
#define INTERPRETER_NEWAST_NAMESPACEDECLARATION_H


#include <string>
#include "ASTNode.h"
#include "FullyQualifiedTypeName.h"

namespace AST {

    class NamespaceDeclaration : public ASTNode {

    public:
        FullyQualifiedTypeName* name;

    public:
        explicit NamespaceDeclaration(FullyQualifiedTypeName* name);

        void print_info(std::ostream &os, int indent) const override;

        void to_json(std::ostream &os, int indent) override;

    };

}

#endif //INTERPRETER_NEWAST_NAMESPACEDECLARATION_H
