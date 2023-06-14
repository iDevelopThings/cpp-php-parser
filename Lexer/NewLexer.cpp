#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
//
// Created by Sam on 10/06/2023.
//

#include <sstream>
#include <iostream>
#include "NewLexer.h"
#include "Token/TokenType.h"

using namespace Token;

NewLexer::NewLexer(FileSystem::File file) : file(std::move(file)) {
    input     = this->file.get_content();
    file_name = this->file.path.string();
    std::istringstream iss(input);
    std::string        line;
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }
}

// NewLexer::NewLexer(const std::string &input) : input(input) {
//     std::istringstream iss(input);
//     std::string        line;
//     while (std::getline(iss, line)) {
//         lines.push_back(line);
//     }
// }

// NewLexer* NewLexer::from_file(const std::string &file_name) {
//     std::ifstream file(file_name);
//     if (!file) {
//         std::cerr << "Could not open file.\n";
//         return nullptr;
//     }
//
//     std::stringstream buffer;
//     buffer << file.rdbuf();
//     std::string fileContents = buffer.str();
//     file.close();
//
//     auto* lexer = new NewLexer(fileContents);
//     lexer->file_name = file_name;
//     return lexer;
// }

std::map<std::filesystem::path, NewLexer*> NewLexer::from_file_manager(FileSystem::Manager* manager) {
    if (manager->files.empty()) {
        std::cerr << "No files in manager.\n";
        return {};
    }

    std::map<std::filesystem::path, NewLexer*> lexers = {};
    for (auto const &[path, file]: manager->files) {
        lexers[path] = new NewLexer(file);

        std::cout << ".";
        std::cout.flush();
    }

    std::cout << "\n";

    return lexers;
}

void NewLexer::skipWhitespace() {
    while (pos < input.size() && std::isspace(input[pos])) {
        if (input[pos] == '\n') {
            // If it's a newline, increment the line count and reset the column count
            currentPosition.line++;
            currentPosition.column = 1;
        } else {
            // If it's not a newline, increment the column count
            currentPosition.column++;
        }
        pos++;

        currentPosition.abs = pos;
    }
}

Tok NewLexer::getNextToken() {
    skipWhitespace();

    if (pos >= input.size()) {
        return {Type::END_OF_FILE, "", currentPosition};
    }


    Position tokenPosition = currentPosition;
    char     ch            = input[pos++];
    ++currentPosition.column;
    currentPosition.abs = pos;

#pragma region Lexing Rules

    std::optional<Tok> tok = {};

    if (input[pos] & 0x80) {  // If the high bit is set, this is a multi-byte sequence
        if ((input[pos] & 0xE0) == 0xC0) {
            // This is a two-byte sequence
            if (pos + 1 >= input.size()) {
                return {Type::ERROR, "Incomplete a two-byte sequence", tokenPosition};
                // Error: incomplete sequence
            }
            ch = ((input[pos] & 0x1F) << 6) | (input[pos + 1] & 0x3F);
            pos += 2;
            // Now you have the full character in ch
        } else if ((input[pos] & 0xF0) == 0xE0) {
            // This is a three-byte sequence
            if (pos + 2 >= input.size()) {
                // Error: incomplete sequence
            }
            ch = ((input[pos] & 0x0F) << 12) | ((input[pos + 1] & 0x3F) << 6) | (input[pos + 2] & 0x3F);
            pos += 3;
            // Now you have the full character in ch
        } else {
            return {Type::ERROR, "Invalid multi-byte sequence", tokenPosition};
        }
    }

    // Check for multi-character operators
    tok = lexMultiCharOperators(ch, currentPosition);
    if (meetsExpectation(tok)) return tok.value();

    // Identifier: [a-zA-Z_][a-zA-Z0-9_]*
    tok = lexIdentifier(ch, currentPosition);
    // if (tok.has_value() && hasExpectation(Type::KEYWORD)) {
    // Check if KeywordTypes contains a value for the token
    if (tok.has_value() && KeywordLookupTable.contains(tok.value().value)) {
        // If it does, return a keyword token
        return {{Type::IDENTIFIER, Type::KEYWORD, KeywordLookupTable[tok.value().value]}, tok.value().value, tokenPosition};
    }
    // }

    if (meetsExpectation(tok)) return tok.value();

    // Number: [0-9]+
    tok = lexNumber(ch, currentPosition);
    if (meetsExpectation(tok)) return tok.value();

    // Comments
    tok = lexComment(ch, currentPosition);
    if (meetsExpectation(tok)) return tok.value();

    // string literal
    tok = lexString(ch, currentPosition);
    if (meetsExpectation(tok)) return tok.value();

    // Check for single-character operators
    tok = lexSingleCharOperators(ch, currentPosition);
    if (meetsExpectation(tok)) return tok.value();


#pragma endregion

    return {Type::ERROR, std::string("Unexpected character: ") + ch, tokenPosition};

}

Tok NewLexer::next() {
    Position tempCurrentPosition = currentPosition;

    Tok t;

    if (lookahead) {
        t = lookahead.value();
        lookahead.reset();
    } else {
        t = getNextToken();
    }

    t.lexer       = this;
    t.previousPos = tempCurrentPosition;

    return t;
}

Tok NewLexer::peekToken() {
    size_t   tempPos             = pos;
    Position tempCurrentPosition = currentPosition;

    Tok token = getNextToken();

    pos             = tempPos;
    currentPosition = tempCurrentPosition;

    return token;
}

void NewLexer::printError(Position tokenPos, const std::string &message) {
    std::cerr << "Error at line " << tokenPos.line << ", column " << tokenPos.column << ": " << message << std::endl;
    std::cerr << "File: " << this->file_name << std::endl;

    int startLine = std::max(1, tokenPos.line - 2);
    int endLine   = std::min(static_cast<int>(lines.size()), tokenPos.line + 2);

    for (int line = startLine; line <= endLine; ++line) {
        std::string lineNumber = std::to_string(line);
        std::cerr << lineNumber << std::string(5 - lineNumber.length(), ' ') << "| " << lines[line - 1] << std::endl;

        if (line == tokenPos.line) {
            std::cerr << std::string(6, ' ') << std::string(tokenPos.column, ' ') << "^" << std::endl;
        }
    }
}

void NewLexer::expect(Token::Type type) {
    expecting.add(type);
}

bool NewLexer::hasExpectation(Type type) {
    if (!hasExpectations()) return false;

    return expecting.hasAll({type});
}

bool NewLexer::hasExpectations() {
    return !expecting.types.empty();
}

bool NewLexer::meetsExpectation(std::optional<Tok> tok) {
    if (!hasExpectations()) {
        return tok.has_value();
    }

    if (!tok.has_value()) return false;

    return expecting.hasAll(tok.value().type.types);
}

void NewLexer::removeExpectation(Token::Type type) {
    erase(expecting.types, type);
}

std::optional<Tok> NewLexer::lexMultiCharOperators(char ch, Position position) {
    if (pos > input.size()) {
        return {};
    }

    auto result = Token::tryMatchMultiCharRule(pos, input);
    if (!result.has_value()) return {};

    pos += result.value().offset - 1;

    return {{result.value().type, result.value().content, position}};
}

std::optional<Tok> NewLexer::lexIdentifier(char ch, Position position) {
    if (isalpha(ch) || ch == '_') {
        std::string identStr = std::string(1, ch);
        while (pos < input.size() && (isalnum(input[pos]) || input[pos] == '_')) {
            identStr += input[pos++];
            ++currentPosition.column;
            currentPosition.abs = pos;
        }
        return {{Type::IDENTIFIER, identStr, position}};
    }

    return {};
}

std::optional<Tok> NewLexer::lexNumber(char ch, Position position) {
    if (isdigit(ch)) {
        std::string numStr = std::string(1, ch);
        while (pos < input.size() && isdigit(input[pos])) {
            numStr += input[pos++];
            ++currentPosition.column;
            currentPosition.abs = pos;
        }
        return {{Type::INTEGER, numStr, position}};
    }
    return {};
}

std::optional<Tok> NewLexer::lexComment(char ch, Position position) {
    if (ch == '/') {
        if (pos < input.size()) {
            if (input[pos] == '/') {  // single line comment
                while (pos < input.size() && input[pos] != '\n') {
                    ++pos;
                    currentPosition.abs = pos;
                }
                return getNextToken();  // discard comment
            } else if (input[pos] == '*') {  // multi-line comment
                ++pos;
                while (pos + 1 < input.size() && !(input[pos] == '*' && input[pos + 1] == '/')) {
                    if (input[pos] == '\n') {
                        ++currentPosition.line;
                        currentPosition.column = 1;
                    } else {
                        ++currentPosition.column;
                    }
                    ++pos;
                }
                pos += 2;  // skip '*/'
                currentPosition.abs = pos;
                return getNextToken();  // discard comment
            } else {
                return {{Type::SLASH, "/", position}};
            }
        } else {
            return {{Type::SLASH, "/", position}};
        }
    }
    return {};
}

std::optional<Tok> NewLexer::lexString(char ch, Position position) {
    if (ch != '"' && ch != '\'') return {};

    char endChar = ch;

    std::string str;
    while (pos < input.size() && input[pos] != endChar) {
        if (input[pos] == '\\') {  // Handle escaped characters
            if (++pos >= input.size()) {
                return {{Type::ERROR, "Unterminated string literal", position}};
            }
            switch (input[pos]) {
                case 'n':str += '\n';
                    break;
                case 't':str += '\t';
                    break;
                default:str += input[pos];

                    // if (input[pos] == '\\' || input[pos] == endChar) {
                    //     str += input[pos];
                    // } else {
                    //     return {{Type::ERROR, "Unknown escape sequence: \\" + std::string(1, input[pos]), position}};
                    // }
            }
        } else {
            str += input[pos];
        }
        pos++;
        currentPosition.abs = pos;
    }
    if (pos >= input.size()) {
        return {{Type::ERROR, "Unterminated string literal", position}};
    }
    ++pos;
    currentPosition.abs = pos;
    return {{Type::STRING, str, position}};
}

std::optional<Tok> NewLexer::lexSingleCharOperators(char ch, Position position) {
    if (SingleCharRules.contains(ch)) {
        return {{SingleCharRules[ch], std::string(1, ch), position}};
    }
    return {};
}

Tok NewLexer::previous(Tok tok) {
    // lookahead = tok;

    // find the absolute position from the tokens line & column
    // int      absolutePos = 0;
    // for (int line        = 1; line < tok.pos.line; ++line) {
    //     absolutePos += lines[line - 1].size() + 1;
    // }
    // absolutePos += tok.pos.column;
    //
    // pos = absolutePos;

    size_t   tempPos             = pos;
    Position tempCurrentPosition = currentPosition;

    pos             = tok.pos.abs - 1;
    currentPosition = tok.pos;

    Tok result = getNextToken();

    pos             = tempPos;
    currentPosition = tempCurrentPosition;

    return result;
}


void Tok::printError() const {
    if (!lexer) return;

    lexer->printError(pos, value);
}

void Tok::error(const std::string &message) const {
    if (!lexer) return;
    lexer->printError(pos, message);
}

void Tok::relexWithExpectation(Token::Type type) {
    if (!lexer) return;
    lexer->expect(type);
    Tok result = lexer->previous(*this);
    lexer->removeExpectation(type);

    (*this) = result;

}

#pragma clang diagnostic pop
