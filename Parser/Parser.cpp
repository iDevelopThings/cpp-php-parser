//
// Created by Sam on 10/06/2023.
//

#include "Parser.h"
#include "../AST/PropertyDeclaration.h"
#include "../AST/NamespaceDeclaration.h"
#include "../AST/UseNamespaceDeclaration.h"

Parser::Parser(Lexer* lexer) {
    this->lexer = lexer;
    this->pos = 0;
    this->current = nullptr;
}

std::ostream &operator<<(std::ostream &os, const Parser &obj) {
    os << "Parser: " << obj.pos << " " << obj.current << std::endl;
    if (!obj.nodes.empty()) {
        os << "Nodes: " << std::endl;
        for (auto node: obj.nodes) {
            os << *node << std::endl;
        }
    }
    return os;
}

Token* Parser::advance(int offset) {
    this->pos += offset;
    if (this->pos > this->lexer->get_token_count() - 1) {
        this->current = nullptr;
    } else {
        this->current = this->lexer->get_token(this->pos);
    }

    return this->current;
}

Token* Parser::peek(int offset) {
    if (this->pos + offset > this->lexer->get_token_count() - 1) {
        return nullptr;
    }

    return this->lexer->get_token(this->pos + offset);
}

bool Parser::match(TokenType type, int offset) {
    if (this->pos + offset > this->lexer->get_token_count() - 1) {
        return false;
    }

    auto* token = this->lexer->get_token(this->pos + offset);
    if (token == nullptr) {
        return false;
    }

    return token->type == type;
}

bool Parser::match_combination(const std::vector<TokenType> &types, int offset) {
    if (this->pos + offset > this->lexer->get_token_count() - 1) {
        return false;
    }
    if (this->pos + offset + types.size() - 1 > this->lexer->get_token_count() - 1) {
        return false;
    }

    /**
     * This should use the offset for the primary token position.
     */
    unsigned long token_pos = this->pos + offset;
    /**
     * We will then loop through the types, adding 1 to the token_pos each time.
     * So that we can check if the next token is the next type in the vector.
     */
    for (auto type: types) {
        auto* token = this->lexer->get_token(token_pos);
        if (token == nullptr) {
            return false;
        }
        if (token->type != type) {
            return false;
        }
        token_pos++;
    }

    return true;
}

bool Parser::match_any(const std::vector<TokenType> &types, int offset) {
    if (this->pos + offset > this->lexer->get_token_count() - 1) {
        return false;
    }

    auto* token = this->lexer->get_token(this->pos + offset);
    if (token == nullptr) {
        return false;
    }

    for (auto type: types) {
        if (token->type == type) {
            return true;
        }
    }

    return true;
}

void Parser::process() {
    this->current = this->lexer->get_token(this->pos);
    auto* script = this->parseScript();
    if (script != nullptr) {
        nodes.push_back(script);
    }
}

Script* Parser::parseScript() {
    if (!match_combination({TokenType::LESS_THAN, TokenType::QUESTION_MARK})) {
        std::cout << "Error: Expected <?" << std::endl;
        return nullptr;
    }
    advance(2);

    // Skip over "php" for the open tag, if it exists
    if (match(TokenType::IDENTIFIER) && current->value == "php") {
        advance();
    }

    auto* script = new Script("");

    auto statements = parseStatements();
    if (!statements.empty()) {
        script->addNodes(statements);
    }

    if (match_combination({TokenType::QUESTION_MARK, TokenType::GREATER_THAN})) {
        advance(2);
    }

    return script;
}

std::vector<ASTNode*> Parser::parseStatements() {
    std::vector<ASTNode*> statements = {};

    while (current != nullptr && !match_combination({TokenType::QUESTION_MARK, TokenType::GREATER_THAN})) {
        // We need to check what type of statement we are parsing, and then call the appropriate function.

        if (match(TokenType::NAME_SPACE)) {
            auto* namespace_declaration = parseNamespaceDeclaration();
            if (namespace_declaration == nullptr) {
                std::cout << "Error: Expected namespace declaration" << std::endl;
                return {};
            }
            statements.push_back(namespace_declaration);
            continue;
        }

        if (match(TokenType::USE)) {
            auto* namespace_declaration = parseUseNamespaceDeclaration();
            if (namespace_declaration == nullptr) {
                std::cout << "Error: Expected use declaration" << std::endl;
                return {};
            }
            statements.push_back(namespace_declaration);
            continue;
        }

        // If we are parsing a class declaration
        if (match(TokenType::CLASS) || match(TokenType::TRAIT)) {
            auto* class_declaration = parseClassDeclaration();
            if (class_declaration == nullptr) {
                std::cout << "Error: Expected class declaration" << std::endl;
                return {};
            }

            statements.push_back(class_declaration);
            continue;
        }

        // handle unable to parse statement
        std::cout << "Error: Unable to parse statement: " << current->value << std::endl;

        lexer->dump_token_info(current);
    }

    return statements;
}

ClassDeclaration* Parser::parseClassDeclaration() {
    if (match(TokenType::CLASS) || match(TokenType::TRAIT)) {
        advance();
    }

    auto* class_declaration = new ClassDeclaration(current->value);

    if (match(TokenType::IDENTIFIER)) {
        advance();
    }

    if (match(TokenType::EXTENDS)) {
        advance();
        if (match(TokenType::IDENTIFIER)) {
            class_declaration->extends = current->value;
            advance();
        }
    }

    if (match(TokenType::IMPLEMENTS)) {
        advance();
        if (match(TokenType::IDENTIFIER)) {
            class_declaration->implements.push_back(current->value);
            advance();
        }
    }

    if (match(TokenType::LEFT_BRACE)) {
        advance();
    }

    while (current != nullptr && !match(TokenType::RIGHT_BRACE)) {
        Token* Visibility = nullptr;
        if (match_any({TokenType::PUBLIC, TokenType::PROTECTED, TokenType::PRIVATE})) {
            Visibility = current;
            advance();
        }
        Token* Static = nullptr;
        if (match(TokenType::STATIC)) {
            Static = current;
            advance();
        }

        // We're unsure at this point if we're parsing a property or a function.

        // We'll assume it's a property first, which can be any of the following:
        // $something = false;
        // ?int $somethingElse = 0;
        // private ?string $name = null;
        // public ?string $name2 = null;
        // protected ?string $name3 = null;
        // public static ?string $name4 = null;

        bool is_optional = false;
        Token* type = nullptr;
        if (match(TokenType::QUESTION_MARK)) {
            is_optional = true;
            advance();

            // If there is a `?` then there must be a type
            if (!match(TokenType::IDENTIFIER)) {
                std::cout << "Error: Expected type" << std::endl;
                return nullptr;
            }
            type = current;

            advance();
        }

        // We got the type, now we need the name, which is `$name`
        Token* name = nullptr;
        if (match(TokenType::DOLLAR)) {
            advance();

            if (!match(TokenType::IDENTIFIER)) {
                std::cout << "Error: Expected identifier" << std::endl;
                return nullptr;
            }

            name = current;

            advance();
        }

        // We got the name, now we need the value, which is `= null`
        std::string value_string = "";
        if (match(TokenType::EQUAL)) {
            advance();

            value_string = current->value;
            while (!match(TokenType::SEMICOLON)) {
                advance();
                value_string += current->value;
            }
        }

        // If we got this far, then we know we're parsing a property
        if (name != nullptr) {
            if (!match(TokenType::SEMICOLON)) {
                std::cout << "Error[parseClassDeclaration]: Expected ;" << std::endl;
                return nullptr;
            }

            advance();

            auto* property_declaration = new PropertyDeclaration(name->value);
            property_declaration->is_optional = is_optional;
            property_declaration->is_static = Static != nullptr;
            property_declaration->type = type;
            property_declaration->value = value_string;
            property_declaration->visibility = Visibility;

            class_declaration->addNode(property_declaration);
        }

        if (match(TokenType::FUNCTION)) {
            auto* function_declaration = parseFunctionDeclaration();
            if (function_declaration == nullptr) {
                std::cout << "Error: Expected function declaration" << std::endl;
                return nullptr;
            }

            if (Visibility != nullptr) {
                function_declaration->visibility = Visibility;
            }

            class_declaration->addNode(function_declaration);
        }
    }

    if (!match(TokenType::RIGHT_BRACE)) {
        std::cout << "Error: Expected }" << std::endl;
        return nullptr;
    }
    advance();

    return class_declaration;
}

FunctionDeclaration* Parser::parseFunctionDeclaration() {
    if (!match(TokenType::FUNCTION)) {
        std::cout << "Error: Expected function" << std::endl;
        return nullptr;
    }
    advance();

    if (!match(TokenType::IDENTIFIER)) {
        std::cout << "Error: Expected identifier" << std::endl;
        return nullptr;
    }

    auto* function_declaration = new FunctionDeclaration(current->value);

    advance();

    if (!match(TokenType::LEFT_PAREN)) {
        std::cout << "Error: Expected (" << std::endl;
        return nullptr;
    }

    advance();

    while (current != nullptr && !match(TokenType::RIGHT_PAREN)) {
        // function($name)
        if (match(TokenType::DOLLAR)) {
            advance();

            if (match(TokenType::IDENTIFIER)) {
                function_declaration->add_parameter(current);
                advance();
            }
        } else
            // function(string $name)
        {
            Token* type = nullptr;
            if (match(TokenType::IDENTIFIER)) {
                type = current;
                advance();
            }

            if (match(TokenType::DOLLAR)) {
                advance();

                if (match(TokenType::IDENTIFIER)) {
                    function_declaration->add_parameter(current);
                    advance();
                }
            }
        }

        // function($name = null)
        if (match(TokenType::EQUAL)) {
            advance();

            while (!match(TokenType::COMMA) && !match(TokenType::RIGHT_PAREN)) {
                advance();
            }
        }

        if (match(TokenType::COMMA)) {
            advance();
        }
    }

    if (!match(TokenType::RIGHT_PAREN)) {
        std::cout << "Error: Expected )" << std::endl;
        return nullptr;
    }

    advance();

    if (match(TokenType::COLON)) {
        advance();

        if (match(TokenType::QUESTION_MARK)) {
            advance();
        }

        if (!match(TokenType::IDENTIFIER)) {
            std::cout << "Error[parseFunctionDeclaration]: Expected type" << std::endl;
            return nullptr;
        }

        std::vector<Token*> return_type;
        return_type.push_back(current);

        while (!match(TokenType::LEFT_BRACE)) {
            advance();
            return_type.push_back(current);
        }

        function_declaration->return_types_tokens = return_type;
    }

    if (!match(TokenType::LEFT_BRACE)) {
        std::cout << "Error: Expected {" << std::endl;
        return nullptr;
    }

    advance();

    int depth = 1;
    while (current != nullptr && depth > 0) {
        advance();
        if (match(TokenType::LEFT_BRACE)) {
            depth++;
        } else if (match(TokenType::RIGHT_BRACE)) {
            depth--;
        }

        // TODO: Parse statements
        // if (match(TokenType::IDENTIFIER)) {
        //     function_declaration->add_statement(parseStatement());
        // }

    }

    if (!match(TokenType::RIGHT_BRACE)) {
        std::cout << "Error: Expected }" << std::endl;
        return nullptr;
    }

    advance();

    if(match(TokenType::RETURN)) {
        advance();
    }

    return function_declaration;
}

NamespaceDeclaration* Parser::parseNamespaceDeclaration() {
    if (!match(TokenType::NAME_SPACE)) {
        std::cout << "Error: Expected namespace" << std::endl;
        return nullptr;
    }
    advance();

    if (!match(TokenType::IDENTIFIER)) {
        std::cout << "Error: Expected identifier" << std::endl;
        return nullptr;
    }

    std::string namespace_name = current->value;
    auto* namespace_declaration = new NamespaceDeclaration(current->value);

    while (current != nullptr && !match(TokenType::SEMICOLON)) {
        advance();
        namespace_name += current->value;
    }

    if (!match(TokenType::SEMICOLON)) {
        std::cout << "Error[parseNamespaceDeclaration]: Expected ;" << std::endl;
        return nullptr;
    }
    advance();

    namespace_name = namespace_name.substr(0, namespace_name.length() - 1);
    namespace_declaration->name = namespace_name;

    return namespace_declaration;
}

UseNamespaceDeclaration* Parser::parseUseNamespaceDeclaration() {
    if (!match(TokenType::USE)) {
        std::cout << "Error: Expected use" << std::endl;
        return nullptr;
    }
    advance();

    if (!match(TokenType::IDENTIFIER)) {
        std::cout << "Error: Expected identifier" << std::endl;
        return nullptr;
    }

    std::string namespace_name = current->value;
    auto* namespace_declaration = new UseNamespaceDeclaration(current->value);

    while (current != nullptr && !match(TokenType::SEMICOLON)) {
        advance();
        namespace_name += current->value;
    }

    if (!match(TokenType::SEMICOLON)) {
        std::cout << "Error[parseUseNamespaceDeclaration]: Expected ;" << std::endl;
        return nullptr;
    }
    advance();

    namespace_name = namespace_name.substr(0, namespace_name.length() - 1);
    namespace_declaration->name = namespace_name;

    return namespace_declaration;
}





