//
// Created by Sam on 11/06/2023.
//

#ifndef GENERATE_TOKENS_JS_ATTRIBUTE_H
#define GENERATE_TOKENS_JS_ATTRIBUTE_H

#include <map>
#include "ASTNode.h"
#include "Parameter.h"

namespace AST {

    class Attribute : public ASTNode {

    public:
        std::string name;
        std::vector<Parameter*> parameters;

        explicit Attribute(const std::string &name);

        void print_info(std::ostream &os, int indent) const override;

        void to_json(std::ostream &os, int indent) override;
    };
} // AST

#endif //GENERATE_TOKENS_JS_ATTRIBUTE_H
