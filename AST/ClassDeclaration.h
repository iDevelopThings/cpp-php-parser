//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_CLASSDECLARATION_H
#define INTERPRETER_CLASSDECLARATION_H


#include <string>
#include "ASTNode.h"

namespace AST {

    class ClassDeclaration : public ASTNode {

    public:
        std::string name;
        std::string extends;
        std::vector<std::string> implements = {};

    public:
        explicit ClassDeclaration(const std::string &name);

        void print_info(std::ostream &os, int indent) const override;

        void to_json(std::ostream &os, int indent) override;

        void setExtends(std::string pToken);


    };

}

#endif //INTERPRETER_CLASSDECLARATION_H
