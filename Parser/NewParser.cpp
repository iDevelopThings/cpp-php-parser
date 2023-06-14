#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
//
// Created by Sam on 11/06/2023.
//

#include <iostream>
#include "NewParser.h"

NewParser::NewParser(NewLexer* lexer) : lexer(lexer), file(lexer->file) {
    this->pos = 0;
}

NewParser::NewParser(FileSystem::File file) : lexer(nullptr), file(std::move(file)) {
    this->pos = 0;
}

void NewParser::init() {
    if (this->lexer == nullptr) {
        this->lexer = new NewLexer(this->file);
    }
}

std::map<std::filesystem::path, NewParser*> NewParser::from_file_manager(FileSystem::Manager* manager) {
    if (manager->files.empty()) {
        std::cerr << "No files in manager.\n";
        return {};
    }

    std::map<std::filesystem::path, NewParser*> parsers = {};
    for (auto const &[path, file]: manager->files) {
        auto* p = new NewParser(file);
        p->parse();

        if (!p->errors.empty()) {
            std::cout << "Errors in " << path << std::endl;
            for (auto const &error: p->errors) {
                std::cout << error << std::endl;
            }
            exit(1);
        }

        parsers[path] = p;

        std::cout << ".";
        std::cout.flush();
    }
    std::cout << "\n";

    return parsers;
}

std::ostream &operator<<(std::ostream &os, const NewParser &obj) {
    os << "Parser: " << obj.pos << " " << obj.current << std::endl;
    if (!obj.nodes.empty()) {
        os << "Nodes: " << std::endl;
        for (auto node: obj.nodes) {
            os << *node << std::endl;
        }
    }
    return os;
}

Tok NewParser::advance(int offset) {
    for (int i = 0; i < offset; i++) {
        this->current = this->lexer->next();

        if (current.isError()) {
            std::cout << "Error: " << current << std::endl;
            current.printError();
            exit(1);
        }

        if (current.isNot(Type::END_OF_FILE))
            previousTokens.push_back(current);

        this->pos++;
    }

    if (previousTokens.size() > 15) {
        // Ensure that we only keep the last 15 tokens
        previousTokens.erase(previousTokens.begin(), previousTokens.begin() + (previousTokens.size() - 15)); // NOLINT(cppcoreguidelines-narrowing-conversions)
    }
    return this->current;
}

void NewParser::consume(Type type, const std::string &message) {
    if (current.is(type)) {
        advance();
        return;
    }

    error(message);
    exit(1);
}

bool NewParser::match(std::initializer_list<Type> types) {
    for (Type type: types) {
        if (match(type))
            return true;
    }
    return false;
}

bool NewParser::match(Type type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool NewParser::check(Type type) {
    if (isAtEnd()) return false;
    return current.is(type);
}

bool NewParser::isAtEnd() {
    return current.is(Type::END_OF_FILE);
}

Tok NewParser::previous() {
    return previousTokens[previousTokens.size() - 2];
}

Tok NewParser::advanceWithExpectation(Type type) {
    this->lexer->expect(type);
    Tok tok = advance();
    this->lexer->removeExpectation(type);
    return tok;
}

void NewParser::parse() {
    init();

    this->advance();
    while (this->current.isNot(Type::END_OF_FILE)) {

        // std::cout << this->current << std::endl;

        if (current.is(Type::PHP_OPEN_TAG)) {
            if (auto* script = parseScript()) {
                this->nodes.push_back(script);
                continue;
            }

            error("Expected script");
            break;
        }

        this->advance();
    }
}

Script* NewParser::parseScript() {
    if (current.isNot(Type::PHP_OPEN_TAG)) {
        error("Expected <?php or <?");
        return nullptr;
    }

    // advanceWithExpectation(Type::KEYWORD);

    auto* script = new Script("");
    auto statements = parseStatements();
    if (!statements.empty()) {
        script->addNodes(statements);
    }

    if (current.is(Type::PHP_CLOSE_TAG)) {
        advance();
    }

    return script;
}

std::vector<ASTNode*> NewParser::parseStatements() {
    std::vector<ASTNode*> statements = {};

    while (!current.isError() && current.isNotAny({Type::PHP_CLOSE_TAG, Type::END_OF_FILE})) {
        if (current.is(Type::END_OF_FILE)) {
            break;
        }

        advance();

        // Parse the namespace
        if (current.is(Type::NAMESPACE)) {
            if (auto* namespace_declaration = parseNamespaceDeclaration()) {
                statements.push_back(namespace_declaration);
                continue;
            }

            error("[parseStatements]: Expected namespace declaration");
            break;
        }

        // Parse the use namespace
        if (current.is(Type::USE)) {
            if (auto* namespace_declaration = parseUseNamespaceDeclaration()) {
                statements.push_back(namespace_declaration);
                continue;
            }

            error("[parseStatements]: Expected use namespace declaration");
            break;
        }

        // Parse class or trait declaration
        if (current.isAny({Type::ABSTRACT, Type::CLASS, Type::INTERFACE, Type::TRAIT, Type::HASH})) {
            if (auto* classDeclaration = parseClassDeclaration()) {
                statements.push_back(classDeclaration);
                continue;
            }

            error("[parseStatements] Expected class declaration");
            break;
        }

        if (auto* statement = parseStatement()) {
            statements.push_back(statement);
            continue;
        }

        error("[parseStatements] Expected statement");
        break;
    }

    return statements;
}

Statement* NewParser::parseStatement() {
    if (current.is(Type::LEFT_BRACE)) {
        return parseBlockStatement();
    }
    if (current.is(Type::IF)) {
        return parseIfStatement();
    }
    if (current.is(Type::FOREACH)) {
        return parseForEachStatement();
    }
    if (current.is(Type::FUNCTION)) {
        return parseFunctionDeclaration();
    }

    if (auto* expr = parsePrimary()) {
        return new ExpressionStatement(expr);
    }


    return nullptr;
}

IfStatement* NewParser::parseIfStatement() {
    consume(Type::IF, "[parseIfStatement]: Expected if");
    // consume(Type::LEFT_PAREN, "[parseIfStatement]: Expected `(` after `if`");
    auto* condition  = parsePrimary();
    auto* thenBranch = parseStatement();

    std::cout << "condition: " << *condition << std::endl;
    if (thenBranch)
        std::cout << "thenBranch: " << *thenBranch << std::endl;

    if (current.is(Type::RIGHT_BRACE)) {
        advance();
    }

    Statement* elseBranch = nullptr;
    if (current.is(Type::ELSE)) {
        advance();
        elseBranch = parseStatement();
    }

    return new IfStatement(condition, thenBranch, elseBranch);
}

ForEachStatement* NewParser::parseForEachStatement() {
    consume(Type::FOREACH, "[parseForEachStatement]: Expected foreach");
    consume(Type::LEFT_PAREN, "[parseForEachStatement]: Expected `(` after `foreach`");

    auto* expr = parsePrimary();
    if (!expr) {
        error("[parseForEachStatement]: Expected expression");
        return nullptr;
    }

    if (!previous().is(Type::AS)) {
        error("[parseForEachStatement]: Expected `as` after expression");
        return nullptr;
    }

    auto* key = parsePrimary();
    if (!key) {
        error("[parseForEachStatement]: Expected key");
        return nullptr;
    }

    Expression* value = nullptr;

    if (current.is(Type::DOUBLE_ARROW)) {
        advance();
        value = parsePrimary();
        if (!value) {
            error("[parseForEachStatement]: Expected value");
            return nullptr;
        }
    }

    consume(Type::RIGHT_PAREN, "[parseForEachStatement]: Expected `)` after value");
    auto* body = parseStatement();

    return new ForEachStatement(expr, key, value, body);
}

BlockStatement* NewParser::parseBlockStatement() {
    consume(Type::LEFT_BRACE, "[parseBlockStatement]: Expected `{`");
    auto* block = new BlockStatement();
    int brace_count    = 1;
    int max_iterations = 100;
    while (brace_count > 0) {
        if (max_iterations-- == 0) {
            error("[parseBlockStatement]: Max iterations reached");
            break;
        }

        if (current.is(Type::LEFT_BRACE)) {
            brace_count++;
        } else if (current.is(Type::RIGHT_BRACE)) {
            brace_count--;
        }

        auto* statement = parseStatement();
        if (statement) {
            block->addStatement(statement);
        } else {
            if (!brace_count) {
                break;
            }
            advance();
        }
    }

    if (!current.is(Type::RIGHT_BRACE)) {
        error("[parseBlockStatement]: Expected `}`");
    } else {
        advance();
    }
    // consume(Type::RIGHT_BRACE, "[parseBlockStatement]: Expected `}`");

    return block;
}

Expression* NewParser::parsePrimary() {

    if (current.isAny({Type::BOOLEAN, Type::NULL_})) {
        auto* literal = new Tok(current);
        advance();
        return new LiteralExpression(literal);
    }

    if (current.is(Type::FLOAT) || current.is(Type::INTEGER)) {
        auto* number = new Tok(current);
        advance();
        return new NumberExpression(number);
    }

    if (current.is(Type::STRING)) {
        auto* string = new Tok(current);
        advance();
        return new StringExpression(string);
    }

    // Parse: `some_func(...)`
    if (match(Type::IDENTIFIER)) {
        auto* identifier = new IdentifierExpression(new Tok(previous()));
        // If the next token is a left parenthesis, we treat it as a function call
        if (match(Type::LEFT_PAREN)) {
            return finishCall(identifier);
        }

        // Parse: `[..]`
        if (match(Type::LEFT_BRACKET)) {
            std::vector<Expression*> elements;
            if (!check(Type::RIGHT_BRACKET)) {
                do {
                    if (elements.size() >= 255) {
                        error("Cannot have more than 255 elements.");
                        exit(1);
                    }
                    elements.push_back(parseExpression());
                } while (match(Type::COMMA));
            }

            consume(Type::RIGHT_BRACKET, "[parsePrimary][arrayExpression]: Expected `]` after array elements.");

            return new MemberAccessExpression(identifier, new ArrayAccessExpression(new PlaceholderExpression(elements)));
        }

        if (match(Type::STATIC_MEMBER_ACCESS)) {
            auto* right = new IdentifierExpression(new Tok(previous()));
            return finishCall(identifier);
        }

        // Otherwise, we treat it as a variable reference
        return identifier;

    }

    if (current.is(Type::LEFT_PAREN)) {
        advance();
        auto* expression = parsePrimary();
        consume(Type::RIGHT_PAREN, "[parseExpression]: Expected `)`");
        return expression;
    }

    if (match({Type::BANG, Type::MINUS})) {
        auto      * op    = new Tok(previous());
        Expression* right = parsePrimary();
        return new UnaryExpression(op, right);
    }

    // Parse: `$foo` or `$foo->bar`
    if (match(Type::DOLLAR)) {
        if (match(Type::IDENTIFIER)) {
            auto    * var   = new VariableExpression(new IdentifierExpression(new Tok(previous())));
            if (auto* right = parsePrimary()) {
                return new MemberAccessExpression(var, right);
            }

            if (match(Type::QUESTION_QUESTION)) {
                return new NullCoalesceExpression(var, parsePrimary());
            }
            return var;
        }
        error("[parsePrimary][memberAccessExpression]: Expected identifier after `$`");
    }

    // Parse: `->foo`
    if (match(Type::MEMBER_ACCESS)) {
        Tok* identifier = nullptr;
        if (check(Type::IDENTIFIER)) {
            identifier = new Tok(current);
            auto* left = new IdentifierExpression(identifier);

            if (match(Type::LEFT_PAREN)) {
                return finishCall(left);
            }

            if (auto* right = parsePrimary()) {
                return new MemberAccessExpression(left, right);
            }

            if (match(Type::ASSIGN)) {
                return new AssignmentExpression(left, parsePrimary(), new Tok(previous()));
            }

            if (match(Type::QUESTION_QUESTION)) {
                return new NullCoalesceExpression(left, parsePrimary());
            }

            if (match(Type::SEMICOLON)) {
                return left;
            }
        }

        if (identifier != nullptr) {
            return new IdentifierExpression(identifier);
        }

        error("[parsePrimary][memberAccessExpression]: Expected identifier after `->`");
    }

    if (auto* expr = parseAssignment()) {
        return expr;
    }

    return nullptr;
}

Expression* NewParser::parseExpression() {
    return parseAssignment();
}

Expression* NewParser::finishCall(Expression* callee) {
    std::vector<Expression*> arguments;
    if (!check(Type::RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                error("Cannot have more than 255 arguments.");
                exit(1);
            }
            arguments.push_back(parseExpression());
        } while (match(Type::COMMA));
    }

    if (!current.is(Type::RIGHT_PAREN)) {
        error("[finishCall]: Expected `)` after arguments.");
    }
    // consume(Type::RIGHT_PAREN, "[finishCall]: Expected `)` after arguments.");

    return new CallExpression(callee, arguments);
}

Expression* NewParser::parseAssignment() {
    Expression* expr = parseOr();

    if (match(Type::ASSIGN)) {
        auto* equals = new Tok(previous());
        auto* value  = parsePrimary();

        if (expr->isIdentifier()) {
            auto* name = dynamic_cast<IdentifierExpression*>(expr);
            return new AssignmentExpression(name, value, equals);
        }
    }

    return expr;
}

Expression* NewParser::parseOr() {
    Expression* expr = parseAnd();

    while (match(Type::OR)) {
        auto* op    = new Tok(previous());
        auto* right = parseAnd();
        expr = new BinaryExpression(expr, op, right);
    }

    return expr;
}

Expression* NewParser::parseAnd() {
    Expression* expr = parseEquality();

    while (match(Type::AND)) {
        auto* op    = new Tok(previous());
        auto* right = parseEquality();
        expr = new BinaryExpression(expr, op, right);
    }

    return expr;
}

Expression* NewParser::parseEquality() {
    Expression* expr = parseRelational();

    while (match({Type::EQUAL_EQUAL, Type::NOT_EQUAL, Type::EQUAL_EQUAL_EQUAL, Type::NOT_EQUAL_EQUAL, Type::INSTANCEOF})) {
        auto      * op    = new Tok(previous());
        Expression* right = parseRelational();
        expr = new BinaryExpression(expr, op, right);
    }

    return expr;
}

Expression* NewParser::parseRelational() {
    Expression* expr = parseAddition();

    while (match({Type::LESS_THAN, Type::LESS_THAN_EQUAL, Type::GREATER_THAN, Type::GREATER_THAN_EQUAL})) {
        auto      * op    = new Tok(previous());
        Expression* right = parseAddition();
        expr = new BinaryExpression(expr, op, right);
    }

    return expr;
}

Expression* NewParser::parseAddition() {
    Expression* expr = parseMultiplication();

    while (match({Type::PLUS, Type::MINUS})) {
        auto      * op    = new Tok(previous());
        Expression* right = parseMultiplication();
        expr = new BinaryExpression(expr, op, right);
    }

    return expr;
}

Expression* NewParser::parseMultiplication() {
    Expression* expr = parseUnary();

    while (match({Type::MUL, Type::SLASH})) {
        auto      * op    = new Tok(previous());
        Expression* right = parseUnary();
        expr = new BinaryExpression(expr, op, right);
    }

    return expr;
}

Expression* NewParser::parseUnary() {
    if (match({Type::BANG, Type::MINUS})) {
        auto      * op    = new Tok(previous());
        Expression* right = parseUnary();
        return new UnaryExpression(op, right);
    }

    return nullptr;//parsePrimary();
}

FullyQualifiedTypeName* NewParser::parseTypeName(const std::vector<Token::Type> &end_types) {
    if (current.isNotAny({Type::IDENTIFIER, Type::BACKSLASH})) {
        error("[parseTypeName]: Expected identifier or backslash");
        return nullptr;
    }

    std::string name;

    while (!current.isError() && current.isNotAny(end_types)) {
        name += current.value;
        advance();
    }

    if (current.isNotAny(end_types)) {
        error("[parseTypeName]: Expected identifier or backslash");
        return nullptr;
    }

    return new FullyQualifiedTypeName(name);
}

NamespaceDeclaration* NewParser::parseNamespaceDeclaration() {
    if (current.isNot(Type::NAMESPACE)) {
        error("[parseNamespaceDeclaration]: Expected namespace");
        return nullptr;
    }

    advance();

    auto* namespaceName = parseTypeName({Type::SEMICOLON});
    if (!namespaceName) {
        error("[parseNamespaceDeclaration]: Expected namespace name");
        return nullptr;
    }

    auto* ns = new NamespaceDeclaration(namespaceName);

    if (current.isNot(Type::SEMICOLON)) {
        error("[parseNamespaceDeclaration]: Expected ;");
        return nullptr;
    }
    // advance();

    return ns;
}

UseNamespaceDeclaration* NewParser::parseUseNamespaceDeclaration() {
    if (current.isNot(Type::USE)) {
        error("[parseUseNamespaceDeclaration]: Expected use");
        return nullptr;
    }
    advance();

    auto* namespaceName = parseTypeName({Type::SEMICOLON});
    if (!namespaceName) {
        error("[parseUseNamespaceDeclaration]: Expected namespace name");
        return nullptr;
    }

    auto* ns = new UseNamespaceDeclaration(namespaceName);

    // If there is an `as`, parse the alias
    if (current.is(Type::AS)) {
        advance();

        auto* alias = parseTypeName({Type::SEMICOLON});
        if (!alias) {
            error("[parseNamespaceDeclaration]: Expected alias");
            return nullptr;
        }

        ns->alias = alias;
    }

    if (current.isNot(Type::SEMICOLON)) {
        error("[parseUseNamespaceDeclaration]: Expected ;");
        return nullptr;
    }
    // advance();

    return ns;
}

ClassDeclaration* NewParser::parseClassDeclaration() {
    Attribute* attribute = nullptr;
    if (current.is(Type::HASH)) {
        attribute = parseAttribute();
    }
    bool is_abstract = false;
    if (current.is(Type::ABSTRACT)) {
        is_abstract = true;
        advance();
    }

    if (current.isAny({Type::CLASS, Type::TRAIT, Type::INTERFACE})) {
        advance();
    }

    auto* class_declaration = new ClassDeclaration(current.value);
    class_declaration->is_abstract = is_abstract;
    class_declaration->addAttribute(attribute);

    if (current.is(Type::IDENTIFIER)) {
        advance();
        // advanceWithExpectation(Type::KEYWORD);
    }

    if (current.is(Type::EXTENDS)) {
        advance();
        while (current.isNot(Type::LEFT_BRACE) && current.isNot(Type::IMPLEMENTS)) {
            class_declaration->addExtends(current.value);
            advance();
        }
    }

    if (current.is(Type::IMPLEMENTS)) {
        advance();
        while (current.isNot(Type::LEFT_BRACE)) {
            class_declaration->addImplements(current.value);
            advance();
        }
    }

    if (current.is(Type::LEFT_BRACE)) {
        advance();
    }

    parseObjectStatements(class_declaration);

    if (current.isNot(Type::RIGHT_BRACE)) {
        error("[parseClassDeclaration]: Expected }");
        return nullptr;
    }
    advance();

    return class_declaration;
}

void NewParser::parseObjectStatements(BaseObjectDeclaration* object) {

    while (current.isNotError() && current.isNotAny({Type::RIGHT_BRACE, Type::END_OF_FILE})) {

        // Parse traits `use Trait1, Trait2;`
        if (current.is(Type::USE)) {
            auto            traits = parseClassTraitUsage();
            for (const auto &item: traits) {
                object->addTrait(item.first, item.second);
            }
            continue;
        }

        std::vector<Tok> modifiers;
        if (current.is(Type::ABSTRACT)) {
            modifiers.push_back(*(new Tok(current)));
            advance();
        }

        Tok* Visibility = nullptr;
        if (current.isAny({Type::PUBLIC, Type::PROTECTED, Type::PRIVATE})) {
            Visibility = new Tok(current);
            advance();
        }

        if (current.isAny({Type::STATIC, Type::FINAL, Type::READONLY})) {
            modifiers.push_back(*(new Tok(current)));
            advance();
        }

        if (current.is(Type::FUNCTION)) {

            auto* function_declaration = parseFunctionDeclaration();
            if (function_declaration == nullptr) {
                error("[parseObjectStatements]: Expected function declaration");
                return;
            }
            if (Visibility != nullptr) {
                function_declaration->visibility = Visibility;
            }

            object->addNode(function_declaration);
        } else {
            // We're unsure at this point if we're parsing a property or a function.
            // We'll assume it's a property first, which can be any of the following:

            // $something = false;
            // ?int $somethingElse = 0;
            // private ?string $name = null;
            // public ?string $name2 = null;
            // protected ?string $name3 = null;
            // public static ?string $name4 = null;


            bool is_optional = false;
            if (current.is(Type::QUESTION_MARK)) {
                is_optional = true;
                advance();
            }


            FullyQualifiedTypeName* type = nullptr;
            if (current.is(Type::IDENTIFIER) && current.isNot(Type::DOLLAR)) {
                type = parseTypeName({Type::DOLLAR, Type::ASSIGN, Type::SEMICOLON});
                //advance();
            }

            // We got the type, now we need the name, which is `$name`
            std::string name_string;
            if (current.is(Type::DOLLAR)) {
                advance();

                if (current.isNot(Type::IDENTIFIER)) {
                    error("[parseObjectStatements]: Expected identifier");
                    return;
                }

                name_string = current.value;

                advance();
            }

            // We got the name, now we need the value, which is `= null`
            std::string value_string;
            if (current.is(Type::ASSIGN)) {
                advance();

                // value_string = current.value;
                while (current.isNot(Type::SEMICOLON)) {
                    value_string += current.value;
                    advance();
                }
            }

            if (name_string.empty()) {
                error("[parseObjectStatements]: Expected property name");
                return;
            }

            if (current.isNot(Type::SEMICOLON)) {
                error("[parseObjectStatements]: Expected ;");
                return;
            }

            advance();

            auto* property_declaration = new PropertyDeclaration(name_string);
            property_declaration->is_optional = is_optional;
            property_declaration->modifiers   = modifiers;
            property_declaration->type        = type;
            property_declaration->value       = value_string;
            property_declaration->visibility  = Visibility;

            object->addNode(property_declaration);
        }

    }

}

std::map<FullyQualifiedTypeName*, std::vector<Tok>> NewParser::parseClassTraitUsage() {
    if (current.isNot(Type::USE)) {
        error("[parseClassTraitUsage]: Expected use");
        return {};
    }

    advance();

    std::map<FullyQualifiedTypeName*, std::vector<Tok>> traits;
    while (current.isNot(Type::SEMICOLON)) {
        std::vector<Tok> traitBody;
        auto* traitName = parseTypeName({Type::SEMICOLON, Type::COMMA, Type::LEFT_BRACE});
        if (current.is(Type::LEFT_BRACE)) {
            advance();
            while (current.isNot(Type::RIGHT_BRACE)) {
                traitBody.push_back(current);
                advance();
            }
            if (current.isNot(Type::RIGHT_BRACE)) {
                error("[parseClassTraitUsage]: Expected }");
                return {};
            }
            advance();
        }
        traits[traitName] = traitBody;
        if (current.is(Type::COMMA)) {
            advance();
        }
    }

    if (current.isNot(Type::SEMICOLON)) {
        error("[parseClassTraitUsage]: Expected ;");
        return {};
    }
    advance();

    return traits;
}

FunctionDeclaration* NewParser::parseFunctionDeclaration() {
    if (current.isNot(Type::FUNCTION)) {
        error("[parseFunctionDeclaration]: Expected function");
        return nullptr;
    }

    advance();

    if (current.isNot(Type::IDENTIFIER)) {
        error("[parseFunctionDeclaration]: Expected identifier");
        return nullptr;
    }

    auto* function_declaration = new FunctionDeclaration(current.value);

    advance();

    if (current.isNot(Type::LEFT_PAREN)) {
        error("[parseFunctionDeclaration]: Expected (");
        return nullptr;
    }

    advance();

    function_declaration->parameters = parseFunctionDeclarationParameters();

    if (current.isNot(Type::RIGHT_PAREN)) {
        error("[parseFunctionDeclaration]: expected )");
        return nullptr;
    }

    advance();

    if (current.is(Type::COLON)) {
        advance();

        if (current.is(Type::QUESTION_MARK)) {
            advance();
        }

        if (current.isNot(Type::IDENTIFIER)) {
            error("[parseFunctionDeclaration]: Expected type");
            return nullptr;
        }

        std::vector<Tok*> return_type;
        return_type.push_back(new Tok(current));

        while (current.isNot(Type::LEFT_BRACE) && current.isNot(Type::SEMICOLON)) {
            advance();
            return_type.push_back(new Tok(current));
        }

        function_declaration->return_types_tokens = return_type;
    }

    // If there is no body, then this is an abstract/interface function
    if (current.isNot(Type::LEFT_BRACE) && current.is(Type::SEMICOLON)) {
        advance();
        return function_declaration;
    }

    if (current.isNot(Type::LEFT_BRACE)) {
        error("[parseFunctionDeclaration]: Expected {");
        return nullptr;
    }

    auto* block = parseBlockStatement();
    if (block == nullptr) {
        error("[parseFunctionDeclaration]: Expected block statement");
        return nullptr;
    }

    function_declaration->body = block;

    /*advance();

    int depth = 1;
    if (current.is(Type::RIGHT_BRACE)) {
        depth = 0;
    }

    std::vector<Tok*> body = {new Tok(current)};

    while (current.isNotError() && depth > 0) {
        advance();

        body.push_back(new Tok(current));

        if (current.is(Type::LEFT_BRACE)) {
            depth++;
        } else if (current.is(Type::RIGHT_BRACE)) {
            depth--;
        }
    }

    function_declaration->body_tokens = body;*/

    /*if (current.isNot(Type::RIGHT_BRACE)) {
        error("[parseFunctionDeclaration]: Expected }");
        return nullptr;
    }

    advance();*/

    return function_declaration;
}

Attribute* NewParser::parseAttribute() {
    if (current.isNot(Type::HASH)) {
        error("[parseAttribute]: Expected #");
        return nullptr;
    }

    advance();

    // Attribute format is roughly:
    // #[AttributeClass(args)]
    // #[AttributeClass(name: ...)]
    // #[AttributeClass(name: ..., name: ...)]

    // Parse the attribute class name
    if (current.isNot(Type::LEFT_BRACKET)) {
        error("[parseAttribute]: Expected [");
        return nullptr;
    }
    advance();

    if (current.isNot(Type::IDENTIFIER)) {
        error("[parseAttribute]: Expected identifier");
        return nullptr;
    }

    auto* attribute = new Attribute(current.value);
    advance();

    // Parse the attribute arguments

    if (current.is(Type::LEFT_PAREN)) {
        advance();

        while (current.isNot(Type::RIGHT_PAREN)) {
            std::string key;
            std::string value;

            // If we have a colon, we're using `name: value` syntax
            if (lexer->peekToken().is(Type::COLON)) {
                key = current.value;
                advance();
                advance();
            }

            value = current.value;
            advance();

            attribute->parameters.push_back(new Parameter(key, value));

            if (current.is(Type::COMMA)) {
                advance();
            }
        }

        if (current.isNot(Type::RIGHT_PAREN)) {
            error("[parseAttribute]: Expected )");
            return nullptr;
        }
        advance();
    }

    if (current.isNot(Type::RIGHT_BRACKET)) {
        error("[parseAttribute]: Expected ]");
        return nullptr;
    }

    advance();

    return attribute;
}

void NewParser::error(const std::string &message) {
    auto* err_tok = new Tok(current);
    err_tok->error_message = message;
    err_tok->error(message);
    errors.push_back(*err_tok);
    std::cerr << "\n";

    if (!previousTokens.empty()) {
        std::cerr << "Previous 15 tokens: ";
        for (const auto &tok: previousTokens) {
            std::cerr << tok << "\n";
        }
        std::cerr << "\n";
    }

}

std::vector<Parameter*> NewParser::parseFunctionDeclarationParameters() {
    std::vector<Parameter*> parameters;

    int max_iterations = 100;

    while (current.isNotError() && current.isNot(Type::RIGHT_PAREN)) {
        if (max_iterations-- == 0) {
            error("[parseFunctionDeclarationParameters]: Infinite loop detected");
            return {};
        }

        auto* parameter = new Parameter();

        if (current.is(Type::QUESTION_MARK)) {
            parameter->is_optional = true;
            advance();
        }

        // If we don't have a $ or ... then we're parsing a type
        if (current.isNotAny({Type::AMPERSAND, Type::DOLLAR, Type::VARIADIC})) {
            if (auto* type = parseTypeName({Type::DOLLAR, Type::PIPE})) {
                parameter->type = type;
            }
        }

        // If we have a ... then we're parsing a variadic parameter
        if (current.is(Type::VARIADIC)) {
            parameter->is_variadic = true;
            advance();
        }

        if (bool is_reference = current.is(Type::AMPERSAND)) {
            parameter->is_reference = true;
            advance();
        }

        // If we have a $ then we're parsing a parameter name
        if (current.is(Type::DOLLAR)) {
            advance();

            if (current.isNot(Type::IDENTIFIER)) {
                error("[parseFunctionDeclarationParameters]: Expected identifier");
                return {};
            }

            parameter->name = current.value;
            advance();
        }

        // If we have a = then we're parsing a default value
        if (current.is(Type::ASSIGN)) {
            advance();

            while (current.isNot(Type::COMMA) && current.isNot(Type::RIGHT_PAREN)) {
                // This is a default array value (e.g. [1, 2, 3])
                if (current.is(Type::LEFT_BRACKET)) {
                    advance();
                    int depth = 1;
                    while (current.isNotError() && depth > 0) {
                        if (current.is(Type::LEFT_BRACKET)) {
                            depth++;
                        } else if (current.is(Type::RIGHT_BRACKET)) {
                            depth--;
                        }
                        advance();
                    }

                    continue;
                }

                advance();
                if (current.is(Type::PIPE)) {
                    advance();
                }
            }
        }

        parameters.push_back(parameter);

        if (current.is(Type::COMMA)) {
            advance();
        }
    }

    return parameters;
}


#pragma clang diagnostic pop
