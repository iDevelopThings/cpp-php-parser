//
// Created by Sam on 12/06/2023.
//

#ifndef GENERATE_TOKENS_JS_STATEMENT_H
#define GENERATE_TOKENS_JS_STATEMENT_H

#include "ASTNode.h"
#include "Expression.h"

namespace AST {

    class Statement : public ASTNode
    {

    };


    class BlockStatement : public Statement
    {
    public:
        BlockStatement();

        std::vector<Statement*> statements;

        explicit BlockStatement(std::vector<Statement*> pStatements);

        void addStatement(Statement* pStatement);

        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "BlockStatement" << std::endl;
            for (auto &statement: statements) {
                statement->print_info(os, indent + 2);
            }
        }
    };


    class IfStatement : public Statement
    {
    public:

        Expression* condition;
        Statement * thenBranch;
        Statement * elseBranch;

        IfStatement(Expression* pExpression, Statement* thenBranch, Statement* elseBranch);


        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "IfStatement" << std::endl;
            os << std::string(indent + 2, ' ') << "Condition" << std::endl;
            condition->print_info(os, indent + 4);
            os << std::string(indent + 2, ' ') << "ThenBranch" << std::endl;
            thenBranch->print_info(os, indent + 4);
            os << std::string(indent + 2, ' ') << "ElseBranch" << std::endl;
            elseBranch->print_info(os, indent + 4);
        }
    };

    class ForEachStatement : public Statement
    {
    public:
        Expression* expr;
        Expression* key;
        Expression*  val;
        Statement*  body;

        ForEachStatement(Expression* pExpr, Expression* pKey, Expression* pVal, Statement* pBody) {
            expr = pExpr;
            key = pKey;
            val = pVal;
            body = pBody;
        }

        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "ForEachStatement" << std::endl;
            os << std::string(indent + 2, ' ') << "Expression" << std::endl;
            expr->print_info(os, indent + 4);
            os << std::string(indent + 2, ' ') << "Key" << std::endl;
            key->print_info(os, indent + 4);
            os << std::string(indent + 2, ' ') << "Val" << std::endl;
            val->print_info(os, indent + 4);
            os << std::string(indent + 2, ' ') << "Body" << std::endl;
            body->print_info(os, indent + 4);
        }
    };

    class ExpressionStatement : public Statement
    {
    public:
        Expression* expression;

        explicit ExpressionStatement(Expression* pExpression) {
            expression = pExpression;
        }

        void print_info(std::ostream &os, int indent) const override {
            os << std::string(indent, ' ') << "ExpressionStatement" << std::endl;
            expression->print_info(os, indent + 2);
        }
    };

} // AST

#endif //GENERATE_TOKENS_JS_STATEMENT_H
