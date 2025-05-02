#include <cstddef>
#include <scanner.hpp>
#include <cctype>
#include <ostream>
#include <regex>

bool scanner::Token::equals(Regex reg)
{
    Match m;
    return std::regex_match(this->string, m, reg);
}

bool scanner::Token::equals(scanner::TokenType type)
{
    return this->type == type;
}

bool scanner::Token::equals(String str)
{
    return this->string == str;
}

bool scanner::Token::equals(scanner::Token& other)
{
    return (
        this->column == other.column
        and this->line == other.line
        and this->string == other.string
        and this->type == other.type
    );
}

void scanner::resolveWords(Vector<Token>& tokens) noexcept
{
    for(auto& t : tokens)
    {
        if(t.type != scanner::TokenType::WORD)
            continue;
        bool foundMatch {false};
        for(auto& rule : wordRules)
        {
            if(rule.first.contains(t.string))
            {
                t.type = rule.second;
                foundMatch = true;
                break;
            }
        }
        if(not foundMatch)
            t.type = TokenType::USER_DEFINED_IDENTIFIER;
    }
}

void scanner::trim(Vector<Token> &tokens) noexcept
{
    for(Size i {0}; i < tokens.size(); i += 1)
        if(tokens[i].type == TokenType::SEPARATOR_SPACE or tokens[i].type == TokenType::COMMENT)
            tokens.erase(tokens.begin() + i);
}

Vector<scanner::Token> scanner::tokenize(String& code) noexcept
{
    std::vector<Token> tokens {};
    std::size_t line {1};
    std::size_t column {0};

    while(not code.empty())
    {
        bool foundMatch {false};
        for(auto& rule : regexRules)
        {
            std::smatch match;
            if(not std::regex_search(code, match, rule.first))
                continue;
            std::size_t sumLine {0};
            std::size_t newColumn {column};
            for(char c : match.str())
            {
                sumLine += c == '\n' ? 1 : 0;
                newColumn = c == '\n' ? 0 : newColumn + 1;
            }
            tokens.push_back({rule.second, match.str(), line, column});
            code = code.substr(match.length());
            column = newColumn;
            line += sumLine;
            foundMatch = true;
            break;
        }
        if(not foundMatch)
        {
            tokens.push_back({TokenType::INVALID, {code[0]}, line, column});
            code.erase(0,1);
            column += 1;
        }
    }
    resolveWords(tokens);
    trim(tokens);
    return tokens;
}

std::ostream& operator<<(std::ostream& os, const scanner::TokenType& type)
{
    switch(type)
    {
        case scanner::TokenType::KEYWORD: return os << "KEYWORD";
        case scanner::TokenType::DEFAULT_IDENTIFIER: return os << "DEFAULT_IDENTIFIER";
        case scanner::TokenType::USER_DEFINED_IDENTIFIER: return os << "USER_DEFINED_IDENTIFIER";
        case scanner::TokenType::SEPARATOR: return os << "SEPARATOR";
        case scanner::TokenType::BLOCK_START: return os << "BLOCK_START";
        case scanner::TokenType::BLOCK_END: return os << "BLOCK_END";
        case scanner::TokenType::LITERAL: return os << "LITERAL";
        case scanner::TokenType::NUMBER: return os << "NUMBER";
        case scanner::TokenType::INVALID: return os << "INVALID";
        case scanner::TokenType::SEMANTIC_DEPENDANT: return os << "SEMANTIC_DEPENDANT";
        case scanner::TokenType::COMMENT: return os << "COMMENT";
        case scanner::TokenType::WORD: return os << "WORD";
        case scanner::TokenType::DEFAULT_TYPE: return os << "DEFAULT_TYPE";
        case scanner::TokenType::SEPARATOR_SPACE: return os << "SEPARATOR_SPACE";
        case scanner::TokenType::BINARY_TYPE_OPERATOR: return os << "BINARY_TYPE_OPERATOR";
        case scanner::TokenType::BINARY_BINARY_OPERATOR: return os << "BINARY_BINARY_OPERATOR";
        case scanner::TokenType::BINARY_ARITHMETIC_OPERATOR: return os << "BINARY_ARITHMETIC_OPERATOR";
        case scanner::TokenType::BINARY_ACCESS_OPERATOR: return os << "BINARY_ACCESS_OPERATOR";
        case scanner::TokenType::BINARY_LOGIC_OPERATOR: return os << "BINARY_LOGIC_OPERATOR";
        case scanner::TokenType::UNARY_TYPE_OPERATOR: return os << "UNARY_TYPE_OPERATOR";
        case scanner::TokenType::UNARY_BINARY_OPERATOR: return os << "UNARY_BINARY_OPERATOR";
        case scanner::TokenType::UNARY_ACCESS_OPERATOR: return os << "UNARY_ACCESS_OPERATOR";
        case scanner::TokenType::UNARY_LOGIC_OPERATOR: return os << "UNARY_LOGIC_OPERATOR";
        case scanner::TokenType::AMBIGUOUS_ARITHMETIC_OPERATOR: return os << "AMBIGUOUS_ARITHMETIC_OPERATOR";
        case scanner::TokenType::ASSIGNMENT_OPERATOR: return os << "ASSIGNMENT_OPERATOR";
        case scanner::TokenType::COMPARISON_OPERATOR: return os << "COMPARISON_OPERATOR";
        case scanner::TokenType::POINTER_OPERATOR: return os << "POINTER_OPERATOR";
        }
    return os;
}

std::ostream& operator<<(std::ostream& os, const scanner::Token& token)
{
    return os << "{" << token.type << ", \"" << (token.string == "\n" ? "\\n" : token.string) << "\", line: " << token.line << ", column: " << token.column << "}";
}