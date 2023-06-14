//
// Created by Sam on 11/06/2023.
//

#ifndef INTERPRETER_TOKENTYPEMIXIN_H
#define INTERPRETER_TOKENTYPEMIXIN_H

#include <utility>

#include "TokenType.h"

namespace Token {

    template<class Derived>
    struct TypeMixin {
        TokenType type;

        TypeMixin() = default;

        explicit TypeMixin(Type type) : type(Token::get(type)) {}

        explicit TypeMixin(std::vector<Token::Type> types) : type(TokenType(std::move(types))) {}

        [[nodiscard]] bool is(Type tokenType) {
            return type.is(tokenType);
        }
        [[nodiscard]] bool is(const std::vector<Type>& tokenTypes) {
            return type.is(tokenTypes);
        }

        [[nodiscard]] bool isAny(const std::vector<Type>& tokenTypes) {
            return std::ranges::any_of(tokenTypes, [this](Type tokenType) {
                return this->is(tokenType);
            });
        }
        [[nodiscard]] bool isNotAny(const std::vector<Type>& tokenTypes) {
            return !isAny(tokenTypes);
        }

        [[nodiscard]] bool isNot(Type tokenType) {
            return !type.is(tokenType);
        }

        [[nodiscard]] bool hasAll(std::vector<Token::Type> otherTypes) {
            return type.hasAll(std::move(otherTypes));
        }

        [[nodiscard]] bool operator==(const Derived &other) const {
            return type == other.type;
        }

        [[nodiscard]] bool operator!=(const Derived &other) const {
            return type != other.type;
        }

        [[nodiscard]] bool isError() {
            return is(Type::ERROR);
        }

        [[nodiscard]] bool isNotError() {
            return isNot(Type::ERROR);
        }


    };

}

#endif //INTERPRETER_TOKENTYPEMIXIN_H
