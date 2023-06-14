//
// Created by Sam on 12/06/2023.
//

#ifndef GENERATE_TOKENS_JS_EXPRESSION_H
#define GENERATE_TOKENS_JS_EXPRESSION_H

#include <utility>

#include "ASTNode.h"
#include "../../Lexer/NewLexer.h"

namespace AST {

    class Expression : public ASTNode
    {

    public:
        bool isIdentifier();
    };


    class PlaceholderExpression : public Expression
    {
    public:
        Tok* token {};
        std::vector<Tok*>        tokens;
        std::vector<Expression*> expressions;

        explicit PlaceholderExpression(Tok* pToken) {
            token = pToken;
        }

        explicit PlaceholderExpression(std::vector<Tok*> pTokens) {
            tokens = std::move(pTokens);
        }

        explicit PlaceholderExpression(std::vector<Expression*> pExpressions) {
            expressions = std::move(pExpressions);
        }


        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "PlaceholderExpression" << std::endl;
            if (token != nullptr) {
                os << std::string(indent + 2, ' ') << token->value << std::endl;
            }
            if (!tokens.empty()) {
                os << std::string(indent + 2, ' ') << "Tokens" << std::endl;
                for (auto &token: tokens) {
                    os << std::string(indent + 4, ' ') << token->value << std::endl;
                }
            }
            if (!expressions.empty()) {
                os << std::string(indent + 2, ' ') << "Expressions" << std::endl;
                for (auto &expression: expressions) {
                    expression->print_info(os, indent + 4);
                }
            }
        }
    };


    class LiteralExpression : public Expression
    {
    public:
        Tok* literal;

        explicit LiteralExpression(Tok* pTok) {
            literal = pTok;
        }

        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "LiteralExpression" << std::endl;
            os << std::string(indent + 2, ' ') << literal->value << std::endl;
        }
    };


    class IdentifierExpression : public Expression
    {
    public:
        Tok* identifier;

        explicit IdentifierExpression(Tok* pTok);

        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "IdentifierExpression" << std::endl;
            os << std::string(indent + 2, ' ') << identifier->value << std::endl;
        }
    };


    class VariableExpression : public Expression
    {
    public:
        Expression* variable;

        VariableExpression(Expression* pVariable) {
            variable = pVariable;
        }

        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "VariableExpression" << std::endl;
            variable->print_info(os, indent + 2);
        }
    };


    class MemberAccessExpression : public Expression
    {
    public:
        Expression* object;
        Expression* property;

        MemberAccessExpression(Expression* pObject, Expression* pProperty) {
            object   = pObject;
            property = pProperty;
        }


        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "MemberAccessExpression" << std::endl;
            object->print_info(os, indent + 2);
            property->print_info(os, indent + 2);
        }
    };


    class ArrayAccessExpression : public Expression
    {
    public:
        Expression* index;

        ArrayAccessExpression(Expression* pIndex) {
            index = pIndex;
        }

        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "ArrayAccessExpression" << std::endl;
            index->print_info(os, indent + 2);
        }
    };


    class NumberExpression : public Expression
    {
    public:
        Tok* number;

        explicit NumberExpression(Tok* pTok) {
            number = pTok;
        }


        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "NumberExpression" << std::endl;
            os << std::string(indent + 2, ' ') << number->value << std::endl;
        }
    };


    class StringExpression : public Expression
    {
    public:
        Tok* string;

        explicit StringExpression(Tok* pTok) {
            string = pTok;
        }

        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "StringExpression" << std::endl;
            os << std::string(indent + 2, ' ') << string->value << std::endl;
        }
    };


    class UnaryExpression : public Expression
    {
    public:
        UnaryExpression(Tok* pTok, Expression* pExpression);

        Expression* expression;
        Tok       * operator_;

        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "UnaryExpression" << std::endl;
            os << std::string(indent + 2, ' ') << operator_->value << std::endl;
            expression->print_info(os, indent + 2);
        }
    };


    class BinaryExpression : public Expression
    {
    public:
        BinaryExpression(Expression* lhs, Tok* op, Expression* rhs);

        Expression* left;
        Expression* right;
        Tok       * operator_;


        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "BinaryExpression" << std::endl;
            os << std::string(indent + 2, ' ') << operator_->value << std::endl;
            left->print_info(os, indent + 2);
            right->print_info(os, indent + 2);
        }
    };


    class TernaryExpression : public Expression
    {
    public:
        Expression* condition;
        Expression* thenBranch;
        Expression* elseBranch;


        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "TernaryExpression" << std::endl;
            condition->print_info(os, indent + 2);
            thenBranch->print_info(os, indent + 2);
            elseBranch->print_info(os, indent + 2);
        }
    };


    class CallExpression : public Expression
    {
    public:
        Expression* callee;
        std::vector<Expression*> arguments;

        CallExpression(Expression* pCallee, std::vector<Expression*> pArguments) {
            callee    = pCallee;
            arguments = std::move(pArguments);
        }

        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "CallExpression" << std::endl;
            callee->print_info(os, indent + 2);
            for (auto &argument: arguments) {
                argument->print_info(os, indent + 2);
            }
        }
    };


    class Variable : public Expression
    {
    public:
        Tok* identifier;

        explicit Variable(Tok* pTok) {
            identifier = pTok;
        }

        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "Variable" << std::endl;
            os << std::string(indent + 2, ' ') << identifier->value << std::endl;
        }
    };


    class AssignmentExpression : public Expression
    {
    public:
        Expression* left;
        Expression* right;
        Tok       * operator_;

        AssignmentExpression(Expression* lhs, Expression* rhs, Tok* pOperator_) {
            left      = lhs;
            right     = rhs;
            operator_ = pOperator_;
        }


        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "AssignmentExpression" << std::endl;
            os << std::string(indent + 2, ' ') << operator_->value << std::endl;
            left->print_info(os, indent + 2);
            right->print_info(os, indent + 2);
        }

    };


    class NullCoalesceExpression : public Expression
    {
    public:
        Expression* left;
        Expression* right;

        NullCoalesceExpression(Expression* lhs, Expression* rhs) {
            left  = lhs;
            right = rhs;
        }

        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "NullCoalesceExpression" << std::endl;
            left->print_info(os, indent + 2);
            right->print_info(os, indent + 2);
        }
    };

} // AST

#endif //GENERATE_TOKENS_JS_EXPRESSION_H
