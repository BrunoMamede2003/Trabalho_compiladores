#include <cstddef>
#include <scanner.hpp>
#include <cctype>
#include <ostream>
#include <regex>


static void checkWords(std::vector<Scanner::Token>& tokens) noexcept
{
    for(auto& token : tokens)
    {
        if(token.type != Scanner::TokenType::WORD)
            continue;
        if(Scanner::Words::keywords.contains(token.string))
            token.type = Scanner::TokenType::KEYWORD;
        else if(Scanner::Words::defaultIdentifiers.contains(token.string))
            token.type = Scanner::TokenType::DEFAULT_IDENTIFIER;
        else if(Scanner::Words::namedOperators.contains(token.string))
            token.type = Scanner::TokenType::NAMED_OPERATOR;
        else
            token.type = Scanner::TokenType::USER_DEFINED_IDENTIFIER;
    }
}

std::vector<Scanner::Token> Scanner::tokenize(std::string& code) noexcept
{
    std::vector<Token> tokens {};
    std::size_t line {1};
    std::size_t column {0};

    while(not code.empty())
    {
        bool foundMatch {false};
        for(auto& rule : rules)
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
    checkWords(tokens);
    return tokens;
}

std::ostream& operator<<(std::ostream& os, const Scanner::TokenType& type)
{
    switch(type)
    {
        case Scanner::TokenType::KEYWORD: return os << "KEYWORD";
        case Scanner::TokenType::DEFAULT_IDENTIFIER: return os << "DEFAULT_IDENTIFIER";
        case Scanner::TokenType::USER_DEFINED_IDENTIFIER: return os << "USER_DEFINED_IDENTIFIER";
        case Scanner::TokenType::UNARY_OPERATOR: return os << "UNARY_OPERATOR";
        case Scanner::TokenType::BINARY_OPERATOR: return os << "BINARY_OPERATOR";
        case Scanner::TokenType::NAMED_OPERATOR: return os << "NAMED_OPERATOR";
        case Scanner::TokenType::AMBIGUOUS_OPERATOR: return os << "AMBIGUOUS_OPERATOR";
        case Scanner::TokenType::SEPARATOR: return os << "SEPARATOR";
        case Scanner::TokenType::BLOCK_START: return os << "BLOCK_START";
        case Scanner::TokenType::BLOCK_END: return os << "BLOCK_END";
        case Scanner::TokenType::LITERAL: return os << "LITERAL";
        case Scanner::TokenType::NUMBER: return os << "NUMBER";
        case Scanner::TokenType::INVALID: return os << "INVALID";
        case Scanner::TokenType::SEMANTIC_DEPENDANT: return os << "SEMANTIC_DEPENDANT";
        case Scanner::TokenType::COMMENT: return os << "COMMENT";
        case Scanner::TokenType::WORD: return os << "WORD";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Scanner::Token& token)
{
    return os << "{" << token.type << ", \"" << (token.string == "\n" ? "\\n" : token.string) << "\", line: " << token.line << ", column: " << token.column << "}";
}