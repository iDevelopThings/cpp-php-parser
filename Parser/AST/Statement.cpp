//
// Created by Sam on 12/06/2023.
//

#include "Statement.h"

#include <utility>

namespace AST {
    IfStatement::IfStatement(Expression* pExpression, Statement* thenBranch, Statement* elseBranch) {
        condition = pExpression;
        this->thenBranch = thenBranch;
        this->elseBranch = elseBranch;
    }

    BlockStatement::BlockStatement(std::vector<Statement*> pStatements) {
        statements = std::move(pStatements);
    }

    void BlockStatement::addStatement(Statement* pStatement) {
        statements.push_back(pStatement);
    }

    BlockStatement::BlockStatement() = default;
} // AST
