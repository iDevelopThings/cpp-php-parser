//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_LEXER_H
#define INTERPRETER_LEXER_H

#include <string>
#include <vector>
#include <optional>
#include "Token.h"
#include "../FileSystem/Manager.h"




class Lexer {

public:
    explicit Lexer(const std::string &text);

    static Lexer* from_file(const std::string &filename);

    static std::map<std::filesystem::path, Lexer*> from_file_manager(FileSystem::Manager* manager);

private:
    std::string file = "";
    std::string text;
    int pos;
    int bol;
    int line;
    char current_char;
    std::vector<Token*> tokens;
    std::vector<std::string> comments = {};



private:
    static bool is_identifier_char(char c);

    static bool is_letter(char c);

    static bool is_digit(char c);

    static bool is_whitespace(char c);

    void skip_whitespace();

    std::string get_current_line_content();

    [[nodiscard]] Token* make_token(TokenType type, std::string content) const;

    char peek(int offset = 1);

public:
    void advance();

    void parse_all();

    [[nodiscard]] std::vector<Token*> get_tokens() const;

    [[nodiscard]] Token* get_token(unsigned long index) const;

    [[nodiscard]] unsigned long get_token_count() const;

    friend std::ostream &operator<<(std::ostream &os, const Lexer &obj);

    void show_file();


    void dump_token_info(Token* pToken);
};


#endif //INTERPRETER_LEXER_H
