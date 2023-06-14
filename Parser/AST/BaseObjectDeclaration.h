//
// Created by Sam on 11/06/2023.
//

#ifndef GENERATE_TOKENS_JS_BASEOBJECTDECLARATION_H
#define GENERATE_TOKENS_JS_BASEOBJECTDECLARATION_H

#include <utility>

#include "ASTNode.h"
#include "Attribute.h"
#include "../../Lexer/NewLexer.h"

namespace AST {


    template<class Derived>
    class Named
    {
    public:
        FullyQualifiedTypeName* name = nullptr;

        void setName(FullyQualifiedTypeName* pName) {
            name = pName;
        }

        void setName(const std::string &pName) {
            name = new FullyQualifiedTypeName(pName);
        }
    };


    template<class Derived>
    class WithExtends
    {
    public:
        std::vector<FullyQualifiedTypeName*> extends = {};

        void addExtends(const std::string& name) { addExtends(new FullyQualifiedTypeName(name)); }

        void addExtends(FullyQualifiedTypeName* pExtends) {
            if (!pExtends) return;
            extends.push_back(pExtends);
        }
    };


    template<class Derived>
    class WithImplements
    {
    public:
        std::vector<FullyQualifiedTypeName*> implements = {};

        void addImplements(const std::string& name) { addImplements(new FullyQualifiedTypeName(name)); }

        void addImplements(FullyQualifiedTypeName* pImplements) {
            if (!pImplements) return;
            implements.push_back(pImplements);
        }
    };


    template<class Derived>
    class WithAttributes
    {
    public:
        std::vector<Attribute*> attributes = {};

        void addAttribute(Attribute* pAttribute) {
            if (!pAttribute) return;
            attributes.push_back(pAttribute);
        }
    };


    template<class Derived>
    class WithTraits
    {
    public:
        std::map<FullyQualifiedTypeName*, std::vector<Tok>> traits = {};

        void addTrait(FullyQualifiedTypeName* pTrait, std::vector<Tok> pTraitArgs = {}) {
            if (!pTrait) return;
            traits[pTrait] = std::move(pTraitArgs);
        }
    };


    class BaseObjectDeclaration :
            public ASTNode,
            public Named<BaseObjectDeclaration>,
            public WithAttributes<BaseObjectDeclaration>,
            public WithTraits<BaseObjectDeclaration>,
            public WithExtends<BaseObjectDeclaration>,
            public WithImplements<BaseObjectDeclaration>
    {

    };

} // AST

#endif //GENERATE_TOKENS_JS_BASEOBJECTDECLARATION_H
