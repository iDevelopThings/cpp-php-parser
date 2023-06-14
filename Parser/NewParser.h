//
// Created by Sam on 11/06/2023.
//

#ifndef GENERATE_TOKENS_JS_NEWPARSER_H
#define GENERATE_TOKENS_JS_NEWPARSER_H


#include "../Lexer/NewLexer.h"
#include "../Lexer/Token/TokenType.h"
#include "AST/Script.h"
#include "AST/NamespaceDeclaration.h"
#include "AST/UseNamespaceDeclaration.h"
#include "AST/ClassDeclaration.h"
#include "AST/FunctionDeclaration.h"
#include "AST/PropertyDeclaration.h"
#include "AST/FullyQualifiedTypeName.h"
#include "AST/Attribute.h"
#include "AST/Statement.h"

using namespace Token;
using namespace AST;


class NewParser
{

private:

    FileSystem::File file;
    NewLexer* lexer;
    unsigned long pos;
    Tok           current;

    std::vector<ASTNode*> nodes;
    std::vector<Tok>      previousTokens = {};
    std::vector<Tok>      errors         = {};

    void init();

    Tok advance(int offset = 1);

    void error(const std::string &message);

public:
    explicit NewParser(NewLexer* lexer);

    explicit NewParser(FileSystem::File file);

    static std::map<std::filesystem::path, NewParser*> from_file_manager(FileSystem::Manager* manager);

    friend std::ostream &operator<<(std::ostream &os, const NewParser &obj);

    void parse();

    Script* parseScript();

    std::vector<ASTNode*> parseStatements();

    Statement* parseStatement();

    FullyQualifiedTypeName* parseTypeName(const std::vector<Token::Type> &end_types = {});

    NamespaceDeclaration* parseNamespaceDeclaration();

    UseNamespaceDeclaration* parseUseNamespaceDeclaration();

    ClassDeclaration* parseClassDeclaration();

    FunctionDeclaration* parseFunctionDeclaration();

    Attribute* parseAttribute();

    Tok advanceWithExpectation(Type type);

    std::map<FullyQualifiedTypeName*, std::vector<Tok>> parseClassTraitUsage();

    std::vector<Parameter*> parseFunctionDeclarationParameters();

    void parseObjectStatements(BaseObjectDeclaration* object);

    void consume(Type type, const std::string &message);

    IfStatement* parseIfStatement();

    Expression* parseExpression();

    BlockStatement* parseBlockStatement();

    Expression* parsePrimary();

    bool isAtEnd();

    bool check(Type type);

    bool match(std::initializer_list<Type> types);

    Tok previous();

    Expression* parseAssignment();

    Expression* parseOr();

    bool match(Type type);

    Expression* parseAnd();

    Expression* parseEquality();

    Expression* parseRelational();

    Expression* parseAddition();

    Expression* parseMultiplication();

    Expression* parseUnary();

    Expression* finishCall(Expression* callee);

    ForEachStatement* parseForEachStatement();
};


#endif //GENERATE_TOKENS_JS_NEWPARSER_H
