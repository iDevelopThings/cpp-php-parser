//
// Created by Sam on 10/06/2023.
//

#include "Lexer.h"

#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>

Lexer::Lexer(const std::string &text) {
    this->text = text;
    this->pos = 0;
    this->bol = 0;
    this->line = 1;
    this->current_char = this->text[this->pos];
    this->tokens = std::vector<Token*>();

    this->comments = std::vector<std::string>();
}

Lexer* Lexer::from_file(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open file.\n";
        return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string fileContents = buffer.str();

    return new Lexer(fileContents);
}

std::map<std::filesystem::path, Lexer*> Lexer::from_file_manager(FileSystem::Manager* manager) {
    if (manager->files.empty()) {
        std::cerr << "No files in manager.\n";
        return {};
    }

    std::map<std::filesystem::path, Lexer*> lexers = {};

    for (auto const &[path, content]: manager->files) {
        lexers[path] = new Lexer(content);
        lexers[path]->file = path.string();
        lexers[path]->parse_all();

        std::cout << ".";
        std::cout.flush();
    }

    std::cout << "\n";

    return lexers;
}

void Lexer::advance() {
    this->pos++;
    if (this->pos > this->text.length() - 1) {
        this->current_char = '\0';
        return;
    }

    this->current_char = this->text[this->pos];
    if (this->current_char == '\n') {
        this->line++;
        this->bol = this->pos + 1;
    }
}

void Lexer::skip_whitespace() {
    while (Lexer::is_whitespace(this->current_char) && this->current_char != '\0') {
        this->advance();
    }
}

void Lexer::parse_all() {
    while (this->current_char != '\0') {
        if (Lexer::is_whitespace(this->current_char)) {
            this->skip_whitespace();
            continue;
        }

        // Parse comments
        if (this->current_char == '/' && this->peek() == '/') {
            this->advance();
            this->advance();

            std::string comment;
            while (this->current_char != '\n' && this->current_char != '\0') {
                comment += this->current_char;
                this->advance();
            }
            comments.push_back(comment);
            continue;
        }
        // Parse php block comments
        // Typically /** ... */
        if (this->current_char == '/' && this->peek() == '*') {
            std::string comment = "";
            comment += this->current_char;
            this->advance();
            comment += this->current_char;
            this->advance();

            while (this->current_char != '\0') {
                comment += this->current_char;
                if (this->current_char == '*' && this->peek() == '/') {
                    this->advance();
                    comment += this->current_char;
                    this->advance();
                    comment += this->current_char;
                    break;
                }
                this->advance();
            }

            comments.push_back(comment);
            continue;
        }

        // Parse identifiers
        if (Lexer::is_identifier_char(this->current_char)) {
            std::string id;
            while (Lexer::is_identifier_char(this->current_char) && this->current_char != '\0') {
                id += this->current_char;
                this->advance();
            }

            TokenType type = TokenType::IDENTIFIER;

            // Check if the identifier is a keyword
            if (KeywordLookupTable.contains(id))
                type = KeywordLookupTable[id];

            this->tokens.push_back(make_token(type, id));

            continue;
        }

        // Parse numbers
        if (this->current_char >= '0' && this->current_char <= '9') {
            std::string num;
            while (this->current_char >= '0' && this->current_char <= '9' && this->current_char != '\0') {
                num += this->current_char;
                this->advance();
            }
            if (this->current_char == '.') {
                num += this->current_char;
                this->advance();
                while (this->current_char >= '0' && this->current_char <= '9' && this->current_char != '\0') {
                    num += this->current_char;
                    this->advance();
                }
                this->tokens.push_back(make_token(TokenType::FLOAT, num));
            } else {
                this->tokens.push_back(make_token(TokenType::INTEGER, num));
            }
            continue;
        }

        // Parse strings
        if (this->current_char == '"') {
            std::string str;
            this->advance();
            while (this->current_char != '"' && this->current_char != '\0') {
                str += this->current_char;
                this->advance();
            }
            this->advance();
            this->tokens.push_back(make_token(TokenType::STRING, str));
            continue;
        }
        // Parse string
        if (this->current_char == '\'') {
            std::string str;
            this->advance();
            while (this->current_char != '\'' && this->current_char != '\0') {
                str += this->current_char;
                this->advance();
            }
            this->advance();
            this->tokens.push_back(make_token(TokenType::STRING, str));
            continue;
        }

        // Parse all basic tokens, operators, etc
        if (TokenLookupTable.contains(this->current_char)) {
            this->tokens.push_back(make_token(TokenLookupTable[this->current_char], std::string(1, this->current_char)));
            this->advance();
            continue;
        }

        this->tokens.push_back(make_token(TokenType::ILLEGAL, std::string(1, this->current_char)));
        //show_file();
        std::cerr << "Unexpected character '" << this->current_char << "' at line " << this->line << " and column " << this->pos - this->bol << "\n";

        this->advance();
        //break;
    }
}

bool Lexer::is_identifier_char(char c) {
    return Lexer::is_letter(c) || Lexer::is_digit(c) || c == '_';
}

bool Lexer::is_letter(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return true;
    // Add support for unicode letters

    bool isUni = (c >= 0x00C0 && c <= 0x00D6) ||
                 (c >= 0x00D8 && c <= 0x00F6) ||
                 (c >= 0x00F8 && c <= 0x00FF) ||
                 (c >= 0x0100 && c <= 0x1FFF) ||
                 (c >= 0x200C && c <= 0x200D) ||
                 (c >= 0x2070 && c <= 0x218F) ||
                 (c >= 0x2C00 && c <= 0x2FEF) ||
                 (c >= 0x3001 && c <= 0xD7FF) ||
                 (c >= 0xF900 && c <= 0xFDCF) ||
                 (c >= 0xFDF0 && c <= 0xFFFD) ||
                 (c >= 0x10000 && c <= 0xEFFFF);

    return isUni;
}

bool Lexer::is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

Token* Lexer::make_token(TokenType type, std::string content) const {
    auto* tok = new Token(type, std::move(content));
    tok->bol = this->bol;
    tok->pos = this->pos;

    return tok;
}

char Lexer::peek(int offset) {
    int peek_pos = this->pos + offset;
    if (peek_pos > this->text.length() - 1) {
        return '\0';
    }

    return this->text[peek_pos];
}

std::vector<Token*> Lexer::get_tokens() const {
    return this->tokens;
}

Token* Lexer::get_token(unsigned long index) const {
    if (index < 0 || index >= this->tokens.size()) {
        std::cerr << "Index out of bounds\n";
        return nullptr;
    }

    return this->tokens[index];
}

unsigned long Lexer::get_token_count() const {
    return this->tokens.size();
}

std::ostream &operator<<(std::ostream &os, const Lexer &obj) {
    os << "Lexer: - ";
    os << "pos: " << obj.pos << " ";
    os << "bol: " << obj.bol << " ";
    os << "line: " << obj.line << " ";
    os << "current_char: " << obj.current_char << " ";
    os << "\n";
    os << "Tokens -> \n";
    for (auto &token: obj.tokens) {
        os << *token;
    }
    os << "}";


    return os;
}

std::string Lexer::get_current_line_content() {
    // int start = this->bol;
    // int end = this->pos;
    int from = this->pos - 20;
    int to = this->pos + 20;
    return this->text.substr(from, to - from);
    // return this->text.substr(start, end - start);
}

void Lexer::show_file() {
    std::string command = "phpstorm --line " + std::to_string(this->line) + " --column " + std::to_string(this->pos - this->bol) + " " + this->file;

    std::system(std::string(command).c_str());
}

void Lexer::dump_token_info(Token* pToken) {
    std::cout << "Token: " << pToken->value << "\n";
    std::cout << "Type: " << token_type_to_string(pToken->type) << "\n";
    std::cout << "Pos: " << pToken->pos << "\n";
    std::cout << "Bol: " << pToken->bol << "\n";
    std::cout << "File: " << file << "\n";
    std::cout << "------------------\n";

    // Get the content from the source
    // We should show 3 lines before and 3 lines after the token
    // We should also highlight the token

    std::string content = this->text;
    std::vector<int> lines;
    int line = 1;
    for (int i = 0; i < content.length(); i++) {
        if (content[i] == '\n') {
            lines.push_back(i);
            line++;
        }
    }

    int start = 0;
    int end = 0;
    for (int i = 0; i < lines.size(); i++) {
        if (lines[i] > pToken->pos) {
            start = lines[i - 10];
            end = lines[i + 10];
            break;
        }
    }

    std::string output = content.substr(start, end - start);
    std::cout << output << "\n";
    std::cout << "------------------\n";

}









