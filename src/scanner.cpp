#include <cstddef>
#include <scanner.hpp>
#include <cctype>
#include <ostream>
#include <regex>

std::vector<Scanner::Token> Scanner::tokenizeLine(
    std::string_view lineToBeScanned,
    std::size_t lineNumber
) noexcept
{
    std::string line {lineToBeScanned};
    std::vector<Scanner::Token> tokens {};
    while(not line.empty())
    {
        bool didMatch {false};
        for(auto& it : Scanner::regexesAndTypes)
        {
            std::smatch match;
            if(std::regex_search(line, match, it.first))
            {
                if(it.second == TokenType::COMMENT)
                {
                    line = line.substr(match.length());
                    break;
                }
                tokens.push_back({it.second, match.str(), lineNumber});
                line = line.substr(match.length());
                didMatch = true;
            }
        }
        if(not didMatch)
        {
            tokens.push_back({Scanner::TokenType::INVALID, "", lineNumber});
            //std::cout << "Error: line = [" << line << "]\n";
            break;
        }
    }
    for(auto& token : tokens)
    {
        if(token.type == Scanner::TokenType::WORD)
        {
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
    return tokens;
}

static std::string tokenTypeToString(Scanner::TokenType type)
{
    switch (type)
    {
        case Scanner::TokenType::KEYWORD: return "KEYWORD";
        case Scanner::TokenType::DEFAULT_IDENTIFIER: return "DEFAULT_IDENTIFIER";
        case Scanner::TokenType::USER_DEFINED_IDENTIFIER: return "USER_DEFINED_IDENTIFIER";
        case Scanner::TokenType::UNARY_OPERATOR: return "UNARY_OPERATOR";
        case Scanner::TokenType::BINARY_OPERATOR: return "BINARY_OPERATOR";
        case Scanner::TokenType::NAMED_OPERATOR: return "NAMED_OPERATOR";
        case Scanner::TokenType::AMBIGUOUS_OPERATOR: return "AMBIGUOUS_OPERATOR";
        case Scanner::TokenType::SEPARATOR: return "SEPARATOR";
        case Scanner::TokenType::BLOCK_START: return "BLOCK_START";
        case Scanner::TokenType::BLOCK_END: return "BLOCK_END";
        case Scanner::TokenType::LITERAL: return "LITERAL";
        case Scanner::TokenType::NUMBER: return "NUMBER";
        case Scanner::TokenType::INVALID: return "INVALID";
        case Scanner::TokenType::SEMANTIC_DEPENDANT: return "SEMANTIC_DEPENDANT";
        default: return "";
    }
}

std::ostream& operator<<(std::ostream& os, const Scanner::Token& token)
{
    return os << "Token("
        << tokenTypeToString(token.type) << ", \""
        << (token.string == "\n" ? "\\n" : token.string) << "\", "
        << token.line
        << ")\n";
}