//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H


#include "../Lexer/Lexer.h"
#include "../AST/ASTNode.h"
#include "../AST/ClassDeclaration.h"
#include "../AST/FunctionDeclaration.h"
#include "../AST/Script.h"
#include "../AST/NamespaceDeclaration.h"
#include "../AST/UseNamespaceDeclaration.h"

using namespace AST;


class Parser {

private:
    Lexer* lexer;
    unsigned long pos;
    Token* current;

    std::vector<ASTNode*> nodes;

public:
    explicit Parser(Lexer* lexer);

    friend std::ostream &operator<<(std::ostream &os, const Parser &obj);

private:
    Token* advance(int offset = 1);

    Token* peek(int offset = 0);

    bool match(TokenType type, int offset = 0);

    bool match_combination(const std::vector<TokenType> &types, int offset = 0);
    bool match_any(const std::vector<TokenType> &types, int offset = 0);

public:
    void process();

    AST::Script* parseScript();

    std::vector<ASTNode*> parseStatements();

    ClassDeclaration* parseClassDeclaration();
    FunctionDeclaration* parseFunctionDeclaration();

    NamespaceDeclaration* parseNamespaceDeclaration();

    UseNamespaceDeclaration* parseUseNamespaceDeclaration();
};


#endif //INTERPRETER_PARSER_H
