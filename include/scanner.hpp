#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <unordered_set>
#include <utility>
#include <regex>
#include <array>

namespace Scanner
{
    enum class TokenType
    {
        KEYWORD,                    //Done
        DEFAULT_IDENTIFIER,         //Done
        USER_DEFINED_IDENTIFIER,    //Done
        UNARY_OPERATOR,             //Done
        BINARY_OPERATOR,            //Done
        NAMED_OPERATOR,             //Done
        AMBIGUOUS_OPERATOR,         //Done
        SEPARATOR,                  //Done
        BLOCK_START,                //Done
        BLOCK_END,                  //Done
        LITERAL,                    //Done
        NUMBER,                     //Done
        INVALID,                    //Done
        SEMANTIC_DEPENDANT,         //Done
        COMMENT,                    //ONLY USED IN SCANNER
        WORD                        //ONLY USED IN SCANNER
    };

    struct Token
    {
        TokenType type;
        std::string string;
        std::size_t line {};
        std::size_t column {};
    };

    using Rule = std::pair<std::regex, TokenType>;

    inline const std::array rules {
        Rule{"^\\/\\/.*\n|^\\/\\*[\\s\\S]*\\*\\/", TokenType::COMMENT},
        Rule{"^u8'([^']|\\\')*'|^'[^']*'", TokenType::LITERAL},
        Rule{"^[_a-zA-Z]+[\\d_a-zA-Z]*", TokenType::WORD},
        Rule{"^0x[\\da-fA-F]+|^0o[0-7]+|^0b[10]+|^\\d+\\.\\d+|^\\d+", TokenType::NUMBER},
        Rule{"^\\[[_\\w]*[_\\w\\d]+ ?\\.\\. ?[_\\w]*[_\\w\\d]*=?\\]|^\\[ ?\\.\\. ?[_\\w]*[_\\w\\d]+=?\\]", TokenType::UNARY_OPERATOR},
        Rule{"^[\\(\\{\\[]", TokenType::BLOCK_START},
        Rule{"^[\\)\\}\\]]", TokenType::BLOCK_END},
        Rule{"^[\n,;]|^ +", TokenType::SEPARATOR},
        Rule{"^[=!><+-\\/*%&|^]?=|^<<=|^>>=|^::|^->|^[:\\/*%&|^\\.]|^>>|^<<", TokenType::BINARY_OPERATOR},
        Rule{"^~", TokenType::UNARY_OPERATOR},
        Rule{"^[+-]", TokenType::AMBIGUOUS_OPERATOR},
        Rule{"^<|^>", TokenType::SEMANTIC_DEPENDANT}
    };

    namespace Words {
        inline const std::unordered_set<std::string> keywords {
            "var",
            "const",
            "private",
            "if",
            "else",
            "loop",
            "for",
            "while",
            "continue",
            "break",
            "defer",
            "function",
            "return",
            "type",
            "union",
            "enum",
            "self",
            "namespace",
            "import",
            "interface",
            "implement",
            "manual",
            "static",
            "initcode"
        };
    
        inline const std::unordered_set<std::string> defaultIdentifiers {
            "Integer",
            "Integer8",
            "Integer16",
            "Integer32",
            "Integer64",
            "Unsigned",
            "Unsigned8",
            "Unsigned16",
            "Unsigned32",
            "Unsigned64",
            "PointerSize",
            "Pointer",
            "Reference",
            "Boolean",
            "Float",
            "Double",
            "Void",
            "Function",
            "List",
            "Map",
            "Tuple",
            "Set",
            "null",
            "true",
            "false"
        };
    
        inline const std::unordered_set<std::string> namedOperators {
            "and",
            "or",
            "not",
            "band",
            "bor",
            "bxor",
            "bnot",
            "bleft",
            "bright",
            "sizeof",
            "typeof",
            "address",
            "deref",
            "cast",
            "safecast",
            "new",
            "delete",
            "use"
        };
    }

    std::vector<Token> tokenize(std::string& code) noexcept;
}

std::ostream& operator<<(std::ostream& os, const Scanner::Token& token);

std::ostream& operator<<(std::ostream& os, const Scanner::TokenType& token);
