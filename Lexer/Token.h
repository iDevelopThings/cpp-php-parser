//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include "TokenType.h"


class Token {
public:
    TokenType type;
    std::string value;
    int bol; // Absolute position of the beginning of the line
    int pos;

    Token(TokenType type, std::string content);

    friend std::ostream &operator<<(std::ostream &os, const Token &obj);
};




#endif //INTERPRETER_TOKEN_H
