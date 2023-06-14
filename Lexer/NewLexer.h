//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_NEWLEXER_H
#define INTERPRETER_NEWLEXER_H


#include <string>
#include <optional>
#include <utility>
#include <vector>
#include "Token/TokenType.h"
#include "Token/TokenTypeMixin.h"
#include "../FileSystem/Manager.h"


class NewLexer;


struct Position {
    int    line;
    int    column;
    size_t abs;
};


struct Tok : public Token::TypeMixin<Tok> {
    std::string value;
    Position    pos {};
    Position    previousPos {};

    NewLexer* lexer = nullptr;

    std::string error_message;

    Tok(Token::Type type, std::string value, Position pos) : TypeMixin(type), value(std::move(value)), pos(pos) {}

    Tok(std::vector<Token::Type> types, std::string value, Position pos) : TypeMixin(std::move(types)), value(std::move(value)), pos(pos) {}

    explicit Tok() : TypeMixin(Token::Type::ILLEGAL) {}

    void printError() const;

    void error(const std::string &message) const;

    friend std::ostream &operator<<(std::ostream &os, const Tok &obj) {
        os << "Token -> (";
        os << obj.type << " - \"" << obj.value << "\" ) ";
        os << obj.pos.line << ":" << obj.pos.column;
        return os;
    }

    void relexWithExpectation(Token::Type type);
};


class NewLexer {

private:

    std::string file_name;
    std::string input;

    size_t                   pos = 0;
    std::optional<Tok>       lookahead;
    Position                 currentPosition {1, 1, 0};
    std::vector<std::string> lines;

    Token::TokenType expecting = {};

public:
    FileSystem::File file;

    explicit NewLexer(FileSystem::File file);

    // explicit NewLexer(const std::string &input);
    // static NewLexer* from_file(const std::string &file_name);
    static std::map<std::filesystem::path, NewLexer*> from_file_manager(FileSystem::Manager* manager);

private:
    Tok getNextToken();

public:
    Tok next();

    void skipWhitespace();

    Tok peekToken();

    void printError(Position tokenPos, const std::string &message);

    void expect(Token::Type type);

    void removeExpectation(Token::Type type);

    bool hasExpectations();

    std::optional<Tok> lexMultiCharOperators(char ch, Position position);

    bool meetsExpectation(std::optional<Tok> tok);

    std::optional<Tok> lexIdentifier(char ch, Position position);

    std::optional<Tok> lexNumber(char ch, Position position);

    std::optional<Tok> lexComment(char ch, Position position);

    std::optional<Tok> lexString(char ch, Position position);

    std::optional<Tok> lexSingleCharOperators(char ch, Position position);

    Tok previous(Tok tok);

    bool hasExpectation(Token::Type type);
};


#endif //INTERPRETER_NEWLEXER_H
