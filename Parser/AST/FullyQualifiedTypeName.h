//
// Created by Sam on 11/06/2023.
//

#ifndef GENERATE_TOKENS_JS_FULLYQUALIFIEDTYPENAME_H
#define GENERATE_TOKENS_JS_FULLYQUALIFIEDTYPENAME_H

#include "ASTNode.h"

namespace AST {

    class FullyQualifiedTypeName : public ASTNode
    {

    public:
        std::string name;

        explicit FullyQualifiedTypeName(const std::string &name);

        void print_info(std::ostream &os, int indent) const override;

        void to_json(std::ostream &os, int indent) override;

        // Allow converting std::string to FullyQualifiedTypeName implicitly
        FullyQualifiedTypeName &operator=(const std::string &pName) {
            this->name = pName;
            return *this;
        }

    };

} // AST

#endif //GENERATE_TOKENS_JS_FULLYQUALIFIEDTYPENAME_H
