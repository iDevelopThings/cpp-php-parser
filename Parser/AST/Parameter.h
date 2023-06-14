//
// Created by Sam on 11/06/2023.
//

#ifndef GENERATE_TOKENS_JS_PARAMETER_H
#define GENERATE_TOKENS_JS_PARAMETER_H

#include "ASTNode.h"
#include "FullyQualifiedTypeName.h"

namespace AST {

    class Parameter : ASTNode
    {
    public:
        std::string name;
        FullyQualifiedTypeName* type;
        std::string value;
        bool        is_variadic  = false;
        bool        is_optional  = false;
        bool        is_reference = false;

        Parameter();

        Parameter(std::string name, FullyQualifiedTypeName* type, std::string value);

        Parameter(std::string name, FullyQualifiedTypeName* type);

        Parameter(std::string name, std::string value);

        explicit Parameter(std::string value);


    };

} // AST

#endif //GENERATE_TOKENS_JS_PARAMETER_H
