//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_NEWAST_USENAMESPACEDECLARATION_H
#define INTERPRETER_NEWAST_USENAMESPACEDECLARATION_H


#include <string>
#include "ASTNode.h"
#include "FullyQualifiedTypeName.h"

namespace AST {

    class UseNamespaceDeclaration : public ASTNode {

    public:
        FullyQualifiedTypeName* name;
        FullyQualifiedTypeName* alias;
    public:
        explicit UseNamespaceDeclaration(FullyQualifiedTypeName* name);

        void print_info(std::ostream &os, int indent) const override;

        void to_json(std::ostream &os, int indent) override;
    };

}

#endif //INTERPRETER_NEWAST_USENAMESPACEDECLARATION_H
