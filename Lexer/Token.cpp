//
// Created by Sam on 10/06/2023.
//

#include "Token.h"

Token::Token(TokenType type, std::string content) :
        type(type),
        value(std::move(content)),
        bol(0),
        pos(0) {

}

std::ostream &operator<<(std::ostream &os, const Token &obj) {
    os << "Token -> \n";
    os << "\ttype: " << token_type_to_string(obj.type) << " - val: " << obj.value << " \n";
    os << "\tbol: " << obj.bol << " pos: " << obj.pos << " \n";
    return os;
}

