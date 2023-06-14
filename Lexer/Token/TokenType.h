#ifndef INTERPRETER_TOKENTYPE_H
#define INTERPRETER_TOKENTYPE_H

#include <map>
#include <string>

#include <vector>

#include <algorithm>

#include <fstream>

#include <optional>

namespace Token {

	enum class Type {
		ILLEGAL, /* Illegal token */
		END_OF_FILE, /* End of file token */
		ERROR, /* Error token */
		KEYWORD, /* Keyword Token */
		PHP_OPEN_TAG, /* <?php or <? */
		PHP_CLOSE_TAG, /* ?> */
		LESS_THAN, /* < */
		GREATER_THAN, /* > */
		QUESTION_MARK, /* ? */
		BANG, /* ! */
		COLON, /* : */
		SEMICOLON, /* ; */
		COMMA, /* , */
		DOT, /* . */
		MINUS, /* - */
		UNDERSCORE, /* _ */
		HASH, /* # */
		AT, /* @ */
		DOLLAR, /* $ */
		SLASH, /* / */
		BACKSLASH, /* \ */
		PIPE, /* | */
		AMPERSAND, /* & */
		PERCENT, /* % */
		CARET, /* ^ */
		TILDE, /* ~ */
		BACKTICK, /* ` */
		LEFT_PAREN, /* ( */
		RIGHT_PAREN, /* ) */
		LEFT_BRACE, /* { */
		RIGHT_BRACE, /* } */
		LEFT_BRACKET, /* [ */
		RIGHT_BRACKET, /* ] */
		VARIADIC, /* ... */
		MEMBER_ACCESS, /* -> */
		DOUBLE_ARROW, /* => */
		STATIC_MEMBER_ACCESS, /* :: */
		QUESTION_QUESTION, /* ?? */
		LESS_THAN_EQUAL, /* <= */
		GREATER_THAN_EQUAL, /* >= */
		ASSIGN, /* = */
		EQUAL_EQUAL, /* == */
		EQUAL_EQUAL_EQUAL, /* === */
		NOT_EQUAL, /* != */
		NOT_EQUAL_EQUAL, /* !== */
		OR, /* || */
		AND, /* && */
		PLUS, /* + */
		MUL, /* * */
		IDENTIFIER, /* variable name */
		INTEGER, /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 */
		FLOAT, /* 1.0, 2.0, 3.0, 4.0, 5.0 */
		STRING, /*  */
		BOOLEAN, /* true, false */
		NULL_, /* null */
		NAMESPACE, /* namespace */
		TRAIT, /* trait */
		USE, /* use */
		CLASS, /* class */
		INTERFACE, /* interface */
		EXTENDS, /* extends */
		IMPLEMENTS, /* implements */
		FUNCTION, /* function */
		PUBLIC, /* public */
		PRIVATE, /* private */
		PROTECTED, /* protected */
		STATIC, /* static */
		ABSTRACT, /* abstract */
		FINAL, /* final */
		READONLY, /* readonly */
		RETURN, /* return */
		IF, /* if */
		ELSE, /* else */
		WHILE, /* while */
		FOR, /* for */
		FOREACH, /* foreach */
		AS, /* as */
		ECHO, /* echo */
		BREAK, /* break */
		CONTINUE, /* continue */
		NEW, /* new */
		THIS, /* $this */
		INSTANCEOF, /* instanceof */
	};
	

	#pragma region Names
	inline std::map<Type, std::string> Names = {
		{Type::ILLEGAL, "ILLEGAL"},
		{Type::END_OF_FILE, "END_OF_FILE"},
		{Type::ERROR, "ERROR"},
		{Type::KEYWORD, "KEYWORD"},
		{Type::PHP_OPEN_TAG, "PHP_OPEN_TAG"},
		{Type::PHP_CLOSE_TAG, "PHP_CLOSE_TAG"},
		{Type::LESS_THAN, "LESS_THAN"},
		{Type::GREATER_THAN, "GREATER_THAN"},
		{Type::QUESTION_MARK, "QUESTION_MARK"},
		{Type::BANG, "BANG"},
		{Type::COLON, "COLON"},
		{Type::SEMICOLON, "SEMICOLON"},
		{Type::COMMA, "COMMA"},
		{Type::DOT, "DOT"},
		{Type::MINUS, "MINUS"},
		{Type::UNDERSCORE, "UNDERSCORE"},
		{Type::HASH, "HASH"},
		{Type::AT, "AT"},
		{Type::DOLLAR, "DOLLAR"},
		{Type::SLASH, "SLASH"},
		{Type::BACKSLASH, "BACKSLASH"},
		{Type::PIPE, "PIPE"},
		{Type::AMPERSAND, "AMPERSAND"},
		{Type::PERCENT, "PERCENT"},
		{Type::CARET, "CARET"},
		{Type::TILDE, "TILDE"},
		{Type::BACKTICK, "BACKTICK"},
		{Type::LEFT_PAREN, "LEFT_PAREN"},
		{Type::RIGHT_PAREN, "RIGHT_PAREN"},
		{Type::LEFT_BRACE, "LEFT_BRACE"},
		{Type::RIGHT_BRACE, "RIGHT_BRACE"},
		{Type::LEFT_BRACKET, "LEFT_BRACKET"},
		{Type::RIGHT_BRACKET, "RIGHT_BRACKET"},
		{Type::VARIADIC, "VARIADIC"},
		{Type::MEMBER_ACCESS, "MEMBER_ACCESS"},
		{Type::DOUBLE_ARROW, "DOUBLE_ARROW"},
		{Type::STATIC_MEMBER_ACCESS, "STATIC_MEMBER_ACCESS"},
		{Type::QUESTION_QUESTION, "QUESTION_QUESTION"},
		{Type::LESS_THAN_EQUAL, "LESS_THAN_EQUAL"},
		{Type::GREATER_THAN_EQUAL, "GREATER_THAN_EQUAL"},
		{Type::ASSIGN, "ASSIGN"},
		{Type::EQUAL_EQUAL, "EQUAL_EQUAL"},
		{Type::EQUAL_EQUAL_EQUAL, "EQUAL_EQUAL_EQUAL"},
		{Type::NOT_EQUAL, "NOT_EQUAL"},
		{Type::NOT_EQUAL_EQUAL, "NOT_EQUAL_EQUAL"},
		{Type::OR, "OR"},
		{Type::AND, "AND"},
		{Type::PLUS, "PLUS"},
		{Type::MUL, "MUL"},
		{Type::IDENTIFIER, "IDENTIFIER"},
		{Type::INTEGER, "INTEGER"},
		{Type::FLOAT, "FLOAT"},
		{Type::STRING, "STRING"},
		{Type::BOOLEAN, "BOOLEAN"},
		{Type::NULL_, "NULL_"},
		{Type::NAMESPACE, "NAMESPACE"},
		{Type::TRAIT, "TRAIT"},
		{Type::USE, "USE"},
		{Type::CLASS, "CLASS"},
		{Type::INTERFACE, "INTERFACE"},
		{Type::EXTENDS, "EXTENDS"},
		{Type::IMPLEMENTS, "IMPLEMENTS"},
		{Type::FUNCTION, "FUNCTION"},
		{Type::PUBLIC, "PUBLIC"},
		{Type::PRIVATE, "PRIVATE"},
		{Type::PROTECTED, "PROTECTED"},
		{Type::STATIC, "STATIC"},
		{Type::ABSTRACT, "ABSTRACT"},
		{Type::FINAL, "FINAL"},
		{Type::READONLY, "READONLY"},
		{Type::RETURN, "RETURN"},
		{Type::IF, "IF"},
		{Type::ELSE, "ELSE"},
		{Type::WHILE, "WHILE"},
		{Type::FOR, "FOR"},
		{Type::FOREACH, "FOREACH"},
		{Type::AS, "AS"},
		{Type::ECHO, "ECHO"},
		{Type::BREAK, "BREAK"},
		{Type::CONTINUE, "CONTINUE"},
		{Type::NEW, "NEW"},
		{Type::THIS, "THIS"},
		{Type::INSTANCEOF, "INSTANCEOF"},
	};
	inline std::string get_name(Type tokenType) {
		return Names[tokenType];
	}
	#pragma endregion
	

	
	
	struct TokenType {
		std::vector<Token::Type> types = {};
		
		TokenType() = default;
		
		explicit TokenType(Type type) : types({type}) {}
		explicit TokenType(std::vector<Token::Type> types) : types(std::move(types)) {}
		
		bool is(Token::Type type) {
			return std::ranges::any_of(types, [type](Token::Type t) { return t == type; });
		}
		
		bool is(std::vector<Token::Type> pTypes) {
			return std::ranges::any_of(pTypes, [this](Token::Type t) { return is(t); });
		}
		
		bool is(TokenType other) {
			return std::ranges::any_of(other.types, [this](Token::Type t) { return is(t); });
		}
		bool hasAll(std::vector<Token::Type> otherTypes) {
			return std::ranges::all_of(otherTypes, [this](Token::Type t) { return is(t); });
		}
		
		bool operator==(const TokenType &other) {
			return hasAll(other.types);
		}
		
		bool operator!=(const TokenType &other) {
			return !hasAll(other.types);
		}
		
		friend std::ostream &operator<<(std::ostream &os, const TokenType &tokenType) {
			std::string types_string;
			
			// Join all types together using | as a separator, but don't add it to the end
			for (int i = 0; i < tokenType.types.size() - 1; i++) {
				types_string += get_name(tokenType.types[i]) + " | ";
			}
			
			// Add the last type to the string
			types_string += get_name(tokenType.types[tokenType.types.size() - 1]);
			
			os << types_string;
			
			return os;
		}
		
		TokenType add(Type type) {
			types.push_back(type);
			return *this;
		}
	
		TokenType add(std::vector<Type> pTypes) {
			types.insert(types.end(), pTypes.begin(), pTypes.end());
			return *this;
		}
	};
	
	

	#pragma region Types
	inline std::map<Type, TokenType> Types = {
		{Type::ILLEGAL, TokenType(Type::ILLEGAL)},
		{Type::END_OF_FILE, TokenType(Type::END_OF_FILE)},
		{Type::ERROR, TokenType(Type::ERROR)},
		{Type::KEYWORD, TokenType(Type::KEYWORD)},
		{Type::PHP_OPEN_TAG, TokenType(Type::PHP_OPEN_TAG)},
		{Type::PHP_CLOSE_TAG, TokenType(Type::PHP_CLOSE_TAG)},
		{Type::LESS_THAN, TokenType(Type::LESS_THAN)},
		{Type::GREATER_THAN, TokenType(Type::GREATER_THAN)},
		{Type::QUESTION_MARK, TokenType(Type::QUESTION_MARK)},
		{Type::BANG, TokenType(Type::BANG)},
		{Type::COLON, TokenType(Type::COLON)},
		{Type::SEMICOLON, TokenType(Type::SEMICOLON)},
		{Type::COMMA, TokenType(Type::COMMA)},
		{Type::DOT, TokenType(Type::DOT)},
		{Type::MINUS, TokenType(Type::MINUS)},
		{Type::UNDERSCORE, TokenType(Type::UNDERSCORE)},
		{Type::HASH, TokenType(Type::HASH)},
		{Type::AT, TokenType(Type::AT)},
		{Type::DOLLAR, TokenType(Type::DOLLAR)},
		{Type::SLASH, TokenType(Type::SLASH)},
		{Type::BACKSLASH, TokenType(Type::BACKSLASH)},
		{Type::PIPE, TokenType(Type::PIPE)},
		{Type::AMPERSAND, TokenType(Type::AMPERSAND)},
		{Type::PERCENT, TokenType(Type::PERCENT)},
		{Type::CARET, TokenType(Type::CARET)},
		{Type::TILDE, TokenType(Type::TILDE)},
		{Type::BACKTICK, TokenType(Type::BACKTICK)},
		{Type::LEFT_PAREN, TokenType(Type::LEFT_PAREN)},
		{Type::RIGHT_PAREN, TokenType(Type::RIGHT_PAREN)},
		{Type::LEFT_BRACE, TokenType(Type::LEFT_BRACE)},
		{Type::RIGHT_BRACE, TokenType(Type::RIGHT_BRACE)},
		{Type::LEFT_BRACKET, TokenType(Type::LEFT_BRACKET)},
		{Type::RIGHT_BRACKET, TokenType(Type::RIGHT_BRACKET)},
		{Type::VARIADIC, TokenType(Type::VARIADIC)},
		{Type::MEMBER_ACCESS, TokenType(Type::MEMBER_ACCESS)},
		{Type::DOUBLE_ARROW, TokenType(Type::DOUBLE_ARROW)},
		{Type::STATIC_MEMBER_ACCESS, TokenType(Type::STATIC_MEMBER_ACCESS)},
		{Type::QUESTION_QUESTION, TokenType(Type::QUESTION_QUESTION)},
		{Type::LESS_THAN_EQUAL, TokenType(Type::LESS_THAN_EQUAL)},
		{Type::GREATER_THAN_EQUAL, TokenType(Type::GREATER_THAN_EQUAL)},
		{Type::ASSIGN, TokenType(Type::ASSIGN)},
		{Type::EQUAL_EQUAL, TokenType(Type::EQUAL_EQUAL)},
		{Type::EQUAL_EQUAL_EQUAL, TokenType(Type::EQUAL_EQUAL_EQUAL)},
		{Type::NOT_EQUAL, TokenType(Type::NOT_EQUAL)},
		{Type::NOT_EQUAL_EQUAL, TokenType(Type::NOT_EQUAL_EQUAL)},
		{Type::OR, TokenType(Type::OR)},
		{Type::AND, TokenType(Type::AND)},
		{Type::PLUS, TokenType(Type::PLUS)},
		{Type::MUL, TokenType(Type::MUL)},
		{Type::IDENTIFIER, TokenType(Type::IDENTIFIER)},
		{Type::INTEGER, TokenType(Type::INTEGER)},
		{Type::FLOAT, TokenType(Type::FLOAT)},
		{Type::STRING, TokenType(Type::STRING)},
		{Type::BOOLEAN, TokenType(Type::BOOLEAN)},
		{Type::NULL_, TokenType(Type::NULL_)},
		{Type::NAMESPACE, TokenType({Type::NAMESPACE, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::TRAIT, TokenType({Type::TRAIT, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::USE, TokenType({Type::USE, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::CLASS, TokenType({Type::CLASS, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::INTERFACE, TokenType({Type::INTERFACE, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::EXTENDS, TokenType({Type::EXTENDS, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::IMPLEMENTS, TokenType({Type::IMPLEMENTS, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::FUNCTION, TokenType({Type::FUNCTION, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::PUBLIC, TokenType({Type::PUBLIC, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::PRIVATE, TokenType({Type::PRIVATE, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::PROTECTED, TokenType({Type::PROTECTED, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::STATIC, TokenType({Type::STATIC, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::ABSTRACT, TokenType({Type::ABSTRACT, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::FINAL, TokenType({Type::FINAL, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::READONLY, TokenType({Type::READONLY, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::RETURN, TokenType({Type::RETURN, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::IF, TokenType({Type::IF, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::ELSE, TokenType({Type::ELSE, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::WHILE, TokenType({Type::WHILE, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::FOR, TokenType({Type::FOR, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::FOREACH, TokenType({Type::FOREACH, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::AS, TokenType({Type::AS, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::ECHO, TokenType({Type::ECHO, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::BREAK, TokenType({Type::BREAK, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::CONTINUE, TokenType({Type::CONTINUE, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::NEW, TokenType({Type::NEW, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::THIS, TokenType({Type::THIS, Type::IDENTIFIER, Type::KEYWORD})},
		{Type::INSTANCEOF, TokenType({Type::INSTANCEOF, Type::IDENTIFIER, Type::KEYWORD})},
	};

	inline TokenType get(Type tokenType) {
		return Types[tokenType];
	}
	#pragma endregion
	

	#pragma region MultiCharTypes
	inline std::map<Type, std::vector<std::string>> MultiCharTypes = {
		{Type::PHP_OPEN_TAG, {"<?php", "<?"}},
		{Type::PHP_CLOSE_TAG, {"?>"}},
		{Type::BACKSLASH, {"92", "39"}},
		{Type::VARIADIC, {"..."}},
		{Type::MEMBER_ACCESS, {"->"}},
		{Type::DOUBLE_ARROW, {"=>"}},
		{Type::STATIC_MEMBER_ACCESS, {"::"}},
		{Type::QUESTION_QUESTION, {"??"}},
		{Type::LESS_THAN_EQUAL, {"<="}},
		{Type::GREATER_THAN_EQUAL, {">="}},
		{Type::EQUAL_EQUAL, {"=="}},
		{Type::EQUAL_EQUAL_EQUAL, {"==="}},
		{Type::NOT_EQUAL, {"!="}},
		{Type::NOT_EQUAL_EQUAL, {"!=="}},
		{Type::OR, {"||"}},
		{Type::AND, {"&&"}},
		{Type::BOOLEAN, {"true", "false"}},
		{Type::NULL_, {"null"}},
	};
	inline std::map<Type, std::vector<std::string>> KeywordTypes = {
		{Type::NAMESPACE, {"namespace"}},
		{Type::TRAIT, {"trait"}},
		{Type::USE, {"use"}},
		{Type::CLASS, {"class"}},
		{Type::INTERFACE, {"interface"}},
		{Type::EXTENDS, {"extends"}},
		{Type::IMPLEMENTS, {"implements"}},
		{Type::FUNCTION, {"function"}},
		{Type::PUBLIC, {"public"}},
		{Type::PRIVATE, {"private"}},
		{Type::PROTECTED, {"protected"}},
		{Type::STATIC, {"static"}},
		{Type::ABSTRACT, {"abstract"}},
		{Type::FINAL, {"final"}},
		{Type::READONLY, {"readonly"}},
		{Type::RETURN, {"return"}},
		{Type::IF, {"if"}},
		{Type::ELSE, {"else"}},
		{Type::WHILE, {"while"}},
		{Type::FOR, {"for"}},
		{Type::FOREACH, {"foreach"}},
		{Type::AS, {"as"}},
		{Type::ECHO, {"echo"}},
		{Type::BREAK, {"break"}},
		{Type::CONTINUE, {"continue"}},
		{Type::NEW, {"new"}},
		{Type::THIS, {"$this"}},
		{Type::INSTANCEOF, {"instanceof"}},
	};
	inline std::map<std::string, Type> KeywordLookupTable = {
		{"namespace", Type::NAMESPACE},
		{"trait", Type::TRAIT},
		{"use", Type::USE},
		{"class", Type::CLASS},
		{"interface", Type::INTERFACE},
		{"extends", Type::EXTENDS},
		{"implements", Type::IMPLEMENTS},
		{"function", Type::FUNCTION},
		{"public", Type::PUBLIC},
		{"private", Type::PRIVATE},
		{"protected", Type::PROTECTED},
		{"static", Type::STATIC},
		{"abstract", Type::ABSTRACT},
		{"final", Type::FINAL},
		{"readonly", Type::READONLY},
		{"return", Type::RETURN},
		{"if", Type::IF},
		{"else", Type::ELSE},
		{"while", Type::WHILE},
		{"for", Type::FOR},
		{"foreach", Type::FOREACH},
		{"as", Type::AS},
		{"echo", Type::ECHO},
		{"break", Type::BREAK},
		{"continue", Type::CONTINUE},
		{"new", Type::NEW},
		{"$this", Type::THIS},
		{"instanceof", Type::INSTANCEOF},
	};
	

	inline std::map<char, Token::Type> SingleCharRules = {
		{39, Type::BACKSLASH},
		{92, Type::BACKSLASH},
		{'<', Type::LESS_THAN},
		{'>', Type::GREATER_THAN},
		{'?', Type::QUESTION_MARK},
		{'!', Type::BANG},
		{':', Type::COLON},
		{';', Type::SEMICOLON},
		{',', Type::COMMA},
		{'.', Type::DOT},
		{'-', Type::MINUS},
		{'_', Type::UNDERSCORE},
		{'#', Type::HASH},
		{'@', Type::AT},
		{'$', Type::DOLLAR},
		{'/', Type::SLASH},
		{'|', Type::PIPE},
		{'&', Type::AMPERSAND},
		{'%', Type::PERCENT},
		{'^', Type::CARET},
		{'~', Type::TILDE},
		{'`', Type::BACKTICK},
		{'(', Type::LEFT_PAREN},
		{')', Type::RIGHT_PAREN},
		{'{', Type::LEFT_BRACE},
		{'}', Type::RIGHT_BRACE},
		{'[', Type::LEFT_BRACKET},
		{']', Type::RIGHT_BRACKET},
		{'=', Type::ASSIGN},
		{'+', Type::PLUS},
		{'*', Type::MUL},
	};
	

	inline std::map<char, std::map<std::string, Token::Type>> MultiCharRules = {
		{5, {{"<?php", Type::PHP_OPEN_TAG}, {"false", Type::BOOLEAN}}},
		{4, {{"true", Type::BOOLEAN}, {"null", Type::NULL_}}},
		{3, {{"...", Type::VARIADIC}, {"===", Type::EQUAL_EQUAL_EQUAL}, {"!==", Type::NOT_EQUAL_EQUAL}}},
		{2, {{"<?", Type::PHP_OPEN_TAG}, {"?>", Type::PHP_CLOSE_TAG}, {"->", Type::MEMBER_ACCESS}, {"=>", Type::DOUBLE_ARROW}, {"::", Type::STATIC_MEMBER_ACCESS}, {"??", Type::QUESTION_QUESTION}, {"<=", Type::LESS_THAN_EQUAL}, {">=", Type::GREATER_THAN_EQUAL}, {"==", Type::EQUAL_EQUAL}, {"!=", Type::NOT_EQUAL}, {"||", Type::OR}, {"&&", Type::AND}}},
	};
	inline std::map<char, std::map<std::string, Token::Type>> KeywordRules = {
		{10, {{"implements", Type::IMPLEMENTS}, {"instanceof", Type::INSTANCEOF}}},
		{9, {{"namespace", Type::NAMESPACE}, {"interface", Type::INTERFACE}, {"protected", Type::PROTECTED}}},
		{8, {{"function", Type::FUNCTION}, {"abstract", Type::ABSTRACT}, {"readonly", Type::READONLY}, {"continue", Type::CONTINUE}}},
		{7, {{"extends", Type::EXTENDS}, {"private", Type::PRIVATE}, {"foreach", Type::FOREACH}}},
		{6, {{"public", Type::PUBLIC}, {"static", Type::STATIC}, {"return", Type::RETURN}}},
		{5, {{"trait", Type::TRAIT}, {"class", Type::CLASS}, {"final", Type::FINAL}, {"while", Type::WHILE}, {"break", Type::BREAK}, {"$this", Type::THIS}}},
		{4, {{"else", Type::ELSE}, {"echo", Type::ECHO}}},
		{3, {{"use", Type::USE}, {"for", Type::FOR}, {"new", Type::NEW}}},
		{2, {{"if", Type::IF}, {"as", Type::AS}}},
	};
	

	struct MultiRuleMatch {
        Type type;
        std::string content;
        int offset;
    };
    inline std::optional<MultiRuleMatch> tryMatchMultiCharRule(size_t pos, std::string &source, bool keywords = false) {
        std::map<char, std::map<std::string, Token::Type>> rules = keywords ? KeywordRules : MultiCharRules;
		
		std::optional<MultiRuleMatch> result;
        // The key of `MultiCharRules` is the additional offset that we need to apply to pos to get the content
        // For example, for `<?php` the key is 2, so our content range is [pos, pos + 2]
        // We then check if the content at that range matches any of the strings in the map

        // We need to iterate over MultiCharRules in reverse order, because we want to match the longest string first
        for (auto it = rules.rbegin(); it != rules.rend(); ++it) {
            auto &[offset, rules] = *it;
            auto content          = source.substr(pos-1, offset);
            auto it2              = rules.find(content);
            if (it2 != rules.end()) {
                // We found a match, so we set the token type and return true
                result = {it2->second, content, offset};
                return result;
            }
        }

        // No match was found, so we return false
        return result;
    }
	
	#pragma endregion
}
#endif //INTERPRETER_TOKENTYPE_H