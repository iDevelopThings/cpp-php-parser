//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_USENAMESPACEDECLARATION_H
#define INTERPRETER_USENAMESPACEDECLARATION_H


#include <string>
#include "ASTNode.h"

namespace AST {

    class UseNamespaceDeclaration : public ASTNode {

    public:
        std::string name;

    public:
        explicit UseNamespaceDeclaration(const std::string &name);

        void print_info(std::ostream &os, int indent) const override;

        void to_json(std::ostream &os, int indent) override;
    };

}

#endif //INTERPRETER_USENAMESPACEDECLARATION_H
