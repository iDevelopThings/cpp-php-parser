const types = require('./types.json');
const fs    = require('fs');

let token_type_struct = `\n
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
`;

let enum_lines = [
	'enum class Type {',
	...types.map(({key, description, matches}, i) => {
		return '\t' + key + ', /* ' + description + ' */';
		//		return '\t' + key + ' = 1LL << ' + i + ', /* ' + description + ' */';
	}),
	'};',
];

let special_matchers = types
	.filter(({matches}) => matches !== null)
	.filter(({matches}) => Array.isArray(matches));

let token_type_struct_lines = token_type_struct.split('\n');

const get_multi_char_rules = (keywords = false) => {
	let rules = {};
	for (const type of types) {
		const {key, description, matches} = type;
		if (matches === null) continue;
		type.keyword = type.keyword ?? false;
		if (type.keyword !== keywords) continue;
		if (!Array.isArray(matches)) continue;
		if (type?.ignore_in?.includes('multi_matches')) continue;
		
		for (const match of matches) {
			if (rules[match.length] === undefined) {
				rules[match.length] = {};
			}
			
			rules[match.length][match] = key;
		}
	}
	return rules;
};

let multi_char_rules         = get_multi_char_rules();
multi_char_rules             = Object.keys(multi_char_rules)
	.sort((a, b) => b - a)
	.map(key => [key, multi_char_rules[key]]);
let multi_char_keyword_rules = get_multi_char_rules(true);
multi_char_keyword_rules     = Object.keys(multi_char_keyword_rules)
	.sort((a, b) => b - a)
	.map(key => [key, multi_char_keyword_rules[key]]);

let single_char_rules = {};
for (const type of types) {
	const {key, description, matches} = type;
	if (matches === null) continue;
	if (Array.isArray(matches) && !type?.force_single_matches) {
		continue;
	}
	if (type?.ignore_in && type.ignore_in.includes('single_matches')) continue;
	
	const match = type.raw_matches ?? matches;
	
	if (single_char_rules[match] !== undefined) {
		console.error(`Match ${match}(${key}) is already defined as ${single_char_rules[match]}!`);
		continue;
	}
	
	const m = typeof match === 'string' ? `'${match}'` : match;
	
	if (Array.isArray(matches)) {
		matches.forEach(match => {
			const m              = typeof match === 'string' ? `'${match}'` : match;
			single_char_rules[m] = key;
		});
	} else
		single_char_rules[m] = key;
}

let types_map_lines = [
	'\n',
	'#pragma region Names',
	`inline std::map<Type, std::string> Names = {`,
	...types.map(({key, description}) => {
		return `\t{Type::${key}, "${key}"},`;
	}),
	'};',
	`inline std::string get_name(Type tokenType) {`,
	`	return Names[tokenType];`,
	`}`,
	
	'#pragma endregion',
	
	'\n',
	
	...token_type_struct_lines,
	
	'\n',
	
	'#pragma region Types',
	`inline std::map<Type, TokenType> Types = {`,
	...types.map(({key, description, matches, keyword}) => {
		const types = [`Type::${key}`];
		if (keyword) {
			types.push(...['Type::IDENTIFIER', 'Type::KEYWORD']);
		}
		
		const types_string = types.length > 1
			? `TokenType({${types.join(', ')}})`
			: `TokenType(Type::${key})`;
		
		return `\t{Type::${key}, ${types_string}},`;
	}),
	'};\n',
	
	`inline TokenType get(Type tokenType) {`,
	`	return Types[tokenType];`,
	`}`,
	
	'#pragma endregion',
	'\n',
	'#pragma region MultiCharTypes',
	
	`inline std::map<Type, std::vector<std::string>> MultiCharTypes = {`,
	...special_matchers.filter(k => !k.keyword).map(({key, description, matches}) => {
		return `\t{Type::${key}, {${matches.map(v => `"${v}"`).join(', ')}}},`;
	}),
	'};',
	`inline std::map<Type, std::vector<std::string>> KeywordTypes = {`,
	...special_matchers.filter(k => k.keyword).map(({key, description, matches}) => {
		return `\t{Type::${key}, {${matches.map(v => `"${v}"`).join(', ')}}},`;
	}),
	'};',
	`inline std::map<std::string, Type> KeywordLookupTable = {`,
	...types.filter(k => k.keyword).map(({key, description, matches}) => {
		const k = Array.isArray(matches) ? matches[0] : matches;
		return `\t{"${k}", Type::${key}},`;
	}),
	'};',
	'\n',
	`inline std::map<char, Token::Type> SingleCharRules = {`,
	...Object.entries(single_char_rules).map(([key, value]) => {
		return `\t{${key}, Type::${value}},`;
	}),
	'};',
	'\n',
	`inline std::map<char, std::map<std::string, Token::Type>> MultiCharRules = {`,
	...multi_char_rules.map(([key, value]) => {
		const values = Object.entries(value).map(([mkey, mvalue]) => {
			return `{"${mkey}", Type::${mvalue}}`;
		}).join(', ');
		return `\t{${key}, {${values}}},`;
	}),
	'};',
	`inline std::map<char, std::map<std::string, Token::Type>> KeywordRules = {`,
	...multi_char_keyword_rules.map(([key, value]) => {
		const values = Object.entries(value).map(([mkey, mvalue]) => {
			return `{"${mkey}", Type::${mvalue}}`;
		}).join(', ');
		return `\t{${key}, {${values}}},`;
	}),
	'};',
	`\n
	struct MultiRuleMatch {
        Type type;
        std::string content;
        int offset;
    };
    inline std::optional<MultiRuleMatch> tryMatchMultiCharRule(size_t pos, std::string &source, bool keywords = false) {
        std::map<char, std::map<std::string, Token::Type>> rules = keywords ? KeywordRules : MultiCharRules;
		
		std::optional<MultiRuleMatch> result;
        // The key of \`MultiCharRules\` is the additional offset that we need to apply to pos to get the content
        // For example, for \`<?php\` the key is 2, so our content range is [pos, pos + 2]
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
	`,
	'#pragma endregion',
];

let lines = [
	'#ifndef INTERPRETER_TOKENTYPE_H',
	'#define INTERPRETER_TOKENTYPE_H\n',
	'#include <map>',
	'#include <string>\n',
	'#include <vector>\n',
	'#include <algorithm>\n',
	'#include <fstream>\n',
	'#include <optional>\n',
	`namespace Token {\n`,
	...[
		...enum_lines,
		...types_map_lines,
	].map(line => '\t' + line),
	`}`,
	'#endif //INTERPRETER_TOKENTYPE_H'
];

fs.writeFileSync('TokenType.h', lines.join('\n'));
