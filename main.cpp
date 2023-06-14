#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "Lexer/NewLexer.h"
#include "Parser/NewParser.h"
#include "FileSystem/Manager.h"


int main() {
    auto fs = FileSystem::Manager::load(
            R"(C:\Users\Sam8t\Downloads\framework-10.x)",
            false
    );

    // R"(F:\c++\interpreter\testing.php)"

    auto parsers = NewParser::from_file_manager(fs);

    // auto* lexers  = NewLexer::from_file_manager(fs);
    // auto* parser = new NewParser(lexer);
    // parser->parse();

    std::cout << "Done\n";
    // std::cout << *parser << "\n";

    /*std::vector<Tok> tokens = {};
    Tok              token;

    while (token = lexer->next(), token.isNot(Token::Type::END_OF_FILE)) {
        if (token.isError()) {
            token.printError();
            continue;
        }
        tokens.push_back(token);
    }


    for (const auto &item: tokens) {
        std::cout << item << "\n";
    }
     */

    /* AST::ASTNode* TestNode = new AST::ASTNode(new Position {0, 0});
     TestNode->nodes.push_back(new AST::ASTNode(new Position {0, 0}));

     auto total_timer = time("Total");

     auto fs = FileSystem::Manager::load(R"(C:\Users\Sam8t\Downloads\framework-10.x)");

     auto lex_timer = time("Lexing");

     auto lexers = Lexer::from_file_manager(fs);

     lex_timer();

     *//*Lexer* lexer = Lexer::from_file(R"(F:\c++\interpreter\testing.php)");
    if (lexer == nullptr) {
        return 1;
    }

    lexer->parse_all();*//*

    auto save_timer = time("Saving lex results");
    for (const auto &item: lexers) {

        std::string new_path = item.first.string();
        size_t pos = 0;
        while ((pos = new_path.find('\\', pos)) != std::string::npos) {
            new_path = new_path.replace(pos, 1, "_");
        }
        new_path = new_path.replace(0, 3, "");
        new_path = new_path.replace(0, 37, "");

        std::filesystem::path path = R"(F:\c++\interpreter\dump\)" + new_path + ".json";
        std::ofstream file(path);
        if (!file.is_open()) {
            std::cout << "Failed to open file: " << path << std::endl;
            return 1;
        }
        file << *item.second << std::endl;
        // item.second->get_tokens()->to_json(file, 0);
        file.close();
    }
    save_timer();

    auto parse_timer = time("Parsing");
    for (const auto &item: lexers) {
        auto* parser = new Parser(item.second);
        parser->process();
    }
    parse_timer();

    *//*auto parse_timer = time("Parsing");

    auto* parser = new Parser(lexer);
    parser->process();

    parse_timer();*//*

    // std::cout << *lexer << std::endl;
    // std::cout << "-------------------\n" << std::endl;
    // std::cout << *parser << std::endl;

    total_timer();*/

    return 0;
}

