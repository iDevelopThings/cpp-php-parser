//
// Created by Sam on 12/06/2023.
//

#include "Expression.h"

namespace AST {
    UnaryExpression::UnaryExpression(Tok* pTok, Expression* pExpression) {
        operator_  = pTok;
        expression = pExpression;
    }

    IdentifierExpression::IdentifierExpression(Tok* pTok) {
        identifier = pTok;
    }

    bool Expression::isIdentifier() {
        return this == dynamic_cast<IdentifierExpression*>(this);
    }

    BinaryExpression::BinaryExpression(Expression* lhs, Tok* op, Expression* rhs) {
        left      = lhs;
        operator_ = op;
        right     = rhs;
    }
} // AST
