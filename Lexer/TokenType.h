//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_TOKENTYPE_H
#define INTERPRETER_TOKENTYPE_H

#include <map>
#include <string>

/**
 * Defines the token types used within PHP
 * We are parsing PHP, so we only need to define the token types used within PHP
 */


enum class TokenType {
    ILLEGAL,            // Illegal token
    END_OF_FILE,        // End of file token
    ERROR,              // Error token

    LESS_THAN,          // <
    GREATER_THAN,       // >

    QUESTION_MARK,      // ?
    EXCLAMATION_MARK,   // !
    COLON,              // :
    SEMICOLON,          // ;
    COMMA,              // ,
    DOT,                // .
    MINUS,              // -
    UNDERSCORE,         // _
    BANG,               // #
    AT,                 // @
    DOLLAR,             // $
    SLASH,              // /
    BACKSLASH,          // <- no comment, it breaks c++
    PIPE,               // |
    AMPERSAND,          // &
    PERCENT,            // %
    CARET,              // ^
    TILDE,              // ~
    BACKTICK,           // `

    LEFT_PAREN,         // (
    RIGHT_PAREN,        // )
    LEFT_BRACE,         // {
    RIGHT_BRACE,        // }
    LEFT_BRACKET,       // [
    RIGHT_BRACKET,      // ]

    LESS_THAN_EQUAL,    // <=
    GREATER_THAN_EQUAL, // >=
    EQUAL,              // ==
    NOT_EQUAL,          // !=
    PLUS,               // +
    MUL,                // *

    IDENTIFIER,         // variable name
    INTEGER,            // 1, 2, 3, 4, 5, 6, 7, 8, 9, 0
    FLOAT,              // 1.0, 2.0, 3.0, 4.0, 5.0
    STRING,             // "Hello, World!"
    BOOLEAN,            // true, false
    NULL_,              // null

    // Keywords
    NAME_SPACE,         // namespace
    TRAIT,              // trait
    USE,                // use
    CLASS,              // class
    EXTENDS,            // extends
    IMPLEMENTS,         // implements
    FUNCTION,           // function
    PUBLIC,             // public
    PRIVATE,            // private
    PROTECTED,          // protected
    STATIC,             // static
    RETURN,             // return
    IF,                 // if
    ELSE,               // else
    WHILE,              // while
    FOR,                // for
    FOREACH,            // foreach
    AS,                 // as
    ECHO,               // echo
    BREAK,              // break
    CONTINUE,           // continue
    NEW,                // new

};

inline std::map<char, TokenType> TokenLookupTable {
        {'<',  TokenType::LESS_THAN},
        {'>',  TokenType::GREATER_THAN},
        {'?',  TokenType::QUESTION_MARK},
        {'!',  TokenType::EXCLAMATION_MARK},
        {':',  TokenType::COLON},
        {';',  TokenType::SEMICOLON},
        {',',  TokenType::COMMA},
        {'.',  TokenType::DOT},
        {'-',  TokenType::MINUS},
        {'_',  TokenType::UNDERSCORE},
        {'#',  TokenType::BANG},
        {'@',  TokenType::AT},
        {'$',  TokenType::DOLLAR},
        {'/',  TokenType::SLASH},
        {'\\', TokenType::BACKSLASH},
        {'|',  TokenType::PIPE},
        {'&',  TokenType::AMPERSAND},
        {'%',  TokenType::PERCENT},
        {'^',  TokenType::CARET},
        {'~',  TokenType::TILDE},
        {'`',  TokenType::BACKTICK},
        {'(',  TokenType::LEFT_PAREN},
        {')',  TokenType::RIGHT_PAREN},
        {'{',  TokenType::LEFT_BRACE},
        {'}',  TokenType::RIGHT_BRACE},
        {'[',  TokenType::LEFT_BRACKET},
        {']',  TokenType::RIGHT_BRACKET},
        {'+',  TokenType::PLUS},
        {'-',  TokenType::MINUS},
        {'*',  TokenType::MUL},
        {'=',  TokenType::EQUAL},
};

inline std::map<std::string, TokenType> KeywordLookupTable {
        {"trait",      TokenType::TRAIT},
        {"namespace",  TokenType::NAME_SPACE},
        {"use",        TokenType::USE},
        {"extends",    TokenType::EXTENDS},
        {"implements", TokenType::IMPLEMENTS},
        {"class",      TokenType::CLASS},
        {"function",   TokenType::FUNCTION},
        {"public",     TokenType::PUBLIC},
        {"private",    TokenType::PRIVATE},
        {"protected",  TokenType::PROTECTED},
        {"static",     TokenType::STATIC},
        {"return",     TokenType::RETURN},
        {"if",         TokenType::IF},
        {"else",       TokenType::ELSE},
        {"while",      TokenType::WHILE},
        {"for",        TokenType::FOR},
        {"foreach",    TokenType::FOREACH},
        {"as",         TokenType::AS},
        {"echo",       TokenType::ECHO},
        {"break",      TokenType::BREAK},
        {"continue",   TokenType::CONTINUE},
        {"new",        TokenType::NEW},
};

inline std::string token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::ILLEGAL:return "illegal";
        case TokenType::LESS_THAN:return "less_than(<)";
        case TokenType::GREATER_THAN:return "greater_than(>)";
        case TokenType::QUESTION_MARK:return "question_mark(?)";
        case TokenType::EXCLAMATION_MARK:return "exclamation_mark(!)";
        case TokenType::COLON:return "colon(:)";
        case TokenType::SEMICOLON:return "semicolon(;)";
        case TokenType::COMMA:return "comma(,)";
        case TokenType::DOT:return "dot(.)";
        case TokenType::MINUS:return "minus(-)";
        case TokenType::UNDERSCORE:return "underscore(_)";
        case TokenType::BANG:return "bang(#)";
        case TokenType::AT:return "at(@)";
        case TokenType::DOLLAR:return "dollar($)";
        case TokenType::SLASH:return "slash(/)";
        case TokenType::BACKSLASH:return "backslash(\\)";
        case TokenType::PIPE:return "pipe(|)";
        case TokenType::AMPERSAND:return "ampersand(&)";
        case TokenType::PERCENT:return "percent(%)";
        case TokenType::CARET:return "caret(^)";
        case TokenType::TILDE:return "tilde(~)";
        case TokenType::BACKTICK:return "backtick(`)";
        case TokenType::LEFT_PAREN:return "left_paren(()";
        case TokenType::RIGHT_PAREN:return "right_paren())";
        case TokenType::LEFT_BRACE:return "left_brace({)";
        case TokenType::RIGHT_BRACE:return "right_brace(})";
        case TokenType::LEFT_BRACKET:return "left_bracket([)";
        case TokenType::RIGHT_BRACKET:return "right_bracket(])";
        case TokenType::LESS_THAN_EQUAL:return "less_than_equal(<=)";
        case TokenType::GREATER_THAN_EQUAL:return "greater_than_equal(>=)";
        case TokenType::EQUAL:return "equal(==)";
        case TokenType::NOT_EQUAL:return "not_equal(!=)";
        case TokenType::PLUS:return "plus(+)";
        case TokenType::MUL:return "mul(*)";
        case TokenType::IDENTIFIER:return "identifier";
        case TokenType::INTEGER:return "integer";
        case TokenType::FLOAT:return "float";
        case TokenType::STRING:return "string";
        case TokenType::BOOLEAN:return "boolean";
        case TokenType::NULL_:return "null";

            // Keywords:
        case TokenType::NAME_SPACE: return "namespace";
        case TokenType::TRAIT: return "trait";
        case TokenType::USE: return "use";
        case TokenType::CLASS: return "class";
        case TokenType::EXTENDS: return "extends";
        case TokenType::IMPLEMENTS: return "implements";
        case TokenType::FUNCTION: return "function";
        case TokenType::PUBLIC:return "public";
        case TokenType::PRIVATE:return "private";
        case TokenType::PROTECTED:return "protected";
        case TokenType::STATIC:return "static";
        case TokenType::RETURN:return "return";
        case TokenType::IF:return "if";
        case TokenType::ELSE:return "else";
        case TokenType::WHILE:return "while";
        case TokenType::FOR:return "for";
        case TokenType::FOREACH:return "foreach";
        case TokenType::AS:return "as";
        case TokenType::ECHO:return "echo";
        case TokenType::BREAK:return "break";
        case TokenType::CONTINUE:return "continue";
        case TokenType::NEW:return "new";

        default:return "unknown";
    }
}


#endif //INTERPRETER_TOKENTYPE_H
