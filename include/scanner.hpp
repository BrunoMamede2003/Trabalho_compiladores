#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <unordered_set>
#include <utility>
#include <regex>
#include <array>


using Size = std::size_t;
using Regex = std::regex;
using Match = std::smatch;
using String = std::string;
template<typename T, Size U>
using Array = std::array<T,U>;
template<typename T>
using UnorderedSet = std::unordered_set<T>;
template<typename T>
using Vector = std::vector<T>;
template<typename T, typename U>
using Pair = std::pair<T,U>;

namespace scanner
{
    enum class TokenType
    {
        //TYPING RELATED OPERATORS
        WORD,
        KEYWORD,                        //Done
        DEFAULT_IDENTIFIER,             //Done
        DEFAULT_TYPE,                   //Done
        USER_DEFINED_IDENTIFIER,        //Done
        LITERAL,                        //Done
        COMMENT,                        //Done
        SEPARATOR,                      //Done
        SEPARATOR_SPACE,                //Done
        NUMBER,                         //Done

        //BINARY OPERATORS
        BINARY_TYPE_OPERATOR,           //Done
        BINARY_BINARY_OPERATOR,         //Done
        BINARY_ARITHMETIC_OPERATOR,     //Done
        BINARY_ACCESS_OPERATOR,         //Done
        BINARY_LOGIC_OPERATOR,          //Done

        //UNARY OPERATORS
        UNARY_TYPE_OPERATOR,            //Done
        UNARY_BINARY_OPERATOR,          //Done
        UNARY_ACCESS_OPERATOR,          //Done
        UNARY_LOGIC_OPERATOR,           //Done
        AMBIGUOUS_ARITHMETIC_OPERATOR,  //Done

        //OTHER OPERATORS
        ASSIGNMENT_OPERATOR,            //Done
        COMPARISON_OPERATOR,            //Done
        POINTER_OPERATOR,               //Done

        //OTHER
        BLOCK_START,                    //Done
        BLOCK_END,                      //Done
        INVALID,                        //Done
        SEMANTIC_DEPENDANT             //Done
    };

    struct Token
    {
        TokenType type;
        String string;
        Size line {};
        Size column {};

        bool equals(Regex reg);
        bool equals(TokenType type);
        bool equals(String str);
        bool equals(Token& token);
    };
    
    using RegexRule = Pair<Regex, TokenType>;
    using WordRule = Pair<UnorderedSet<String>, TokenType>;

    inline const Array regexRules {
        RegexRule{"^\\/\\/.*\n|^\\/\\*[\\s\\S]*\\*\\/", TokenType::COMMENT},
        RegexRule{"^u8'([^']|\\\')*'|^'[^']*'", TokenType::LITERAL},
        RegexRule{"^[_a-zA-Z]+[\\d_a-zA-Z]*", TokenType::WORD},
        RegexRule{"^0x[\\da-fA-F]+|^0o[0-7]+|^0b[10]+|^\\d+\\.\\d+|^\\d+", TokenType::NUMBER},
        RegexRule{"^\\[ *\\d* *\\. *\\. *=? *\\d* *\\]", TokenType::UNARY_ACCESS_OPERATOR},
        RegexRule{"^\\[ *[_\\w]*[_\\w\\d]* *\\. *\\. *=? *[_\\w]*[_\\w\\d]* *\\]", TokenType::UNARY_ACCESS_OPERATOR},
        RegexRule{"^[=!><]=", TokenType::COMPARISON_OPERATOR},
        RegexRule{"^[+-\\/*%&|^]?=|^<<=|^>>=", TokenType::ASSIGNMENT_OPERATOR},
        RegexRule{"^[&|^]|^<<|^>>", TokenType::BINARY_BINARY_OPERATOR},
        RegexRule{"^::|^\\.", TokenType::BINARY_ACCESS_OPERATOR},
        RegexRule{"^->", TokenType::POINTER_OPERATOR},
        RegexRule{"^:", TokenType::BINARY_TYPE_OPERATOR},
        RegexRule{"^~", TokenType::UNARY_BINARY_OPERATOR},
        RegexRule{"^[\\/*%]", TokenType::BINARY_ARITHMETIC_OPERATOR},
        RegexRule{"^[+-]", TokenType::AMBIGUOUS_ARITHMETIC_OPERATOR},
        RegexRule{"^[\\(\\{\\[]", TokenType::BLOCK_START},
        RegexRule{"^[\\)\\}\\]]", TokenType::BLOCK_END},
        RegexRule{"^ +", TokenType::SEPARATOR_SPACE},
        RegexRule{"^[\n,;]", TokenType::SEPARATOR},
        RegexRule{"^<|^>", TokenType::SEMANTIC_DEPENDANT}
    };

    static const UnorderedSet<String> keywords {
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

    static const UnorderedSet<String> defaultTypes {
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
        "Size",
        "Pointer",
        "Reference",
        "Boolean",
        "Float",
        "Double",
        "Void",
        "Function",
        "Tuple"
    };

    static const UnorderedSet<String> values {
        "true",
        "false",
        "null"
    };

    static const UnorderedSet<String> binaryLogicOperators {
        "and",
        "or"
    };

    static const UnorderedSet<String> unaryLogicOperators {
        "not"
    };

    static const UnorderedSet<String> binaryBinaryOperators {
        "band",
        "bor",
        "bxor",
        "bleft",
        "bright"
    };

    static const UnorderedSet<String> unaryBinaryOperators {
        "bnot"
    };

    static const UnorderedSet<String> unaryTypeOperators {
        "cast",
        "typeof",
        "sizeof",
        "use"
    };

    static const UnorderedSet<String> pointerOperators {
        "address",
        "deref",
        "new",
        "delete"
    };

    inline const Array wordRules {
        WordRule{keywords, TokenType::KEYWORD},
        WordRule{defaultTypes, TokenType::DEFAULT_TYPE},
        WordRule{values, TokenType::DEFAULT_IDENTIFIER},
        WordRule{binaryLogicOperators, TokenType::BINARY_LOGIC_OPERATOR},
        WordRule{unaryLogicOperators, TokenType::UNARY_LOGIC_OPERATOR},
        WordRule{binaryBinaryOperators, TokenType::BINARY_BINARY_OPERATOR},
        WordRule{unaryBinaryOperators, TokenType::UNARY_BINARY_OPERATOR},
        WordRule{unaryTypeOperators, TokenType::UNARY_TYPE_OPERATOR},
        WordRule{pointerOperators, TokenType::POINTER_OPERATOR}
    };

    void resolveWords(Vector<Token>& tokens) noexcept;
    void trim(Vector<Token>& tokens) noexcept;
    Vector<Token> tokenize(String& code) noexcept;
}

std::ostream& operator<<(std::ostream& os, const scanner::Token& token);

std::ostream& operator<<(std::ostream& os, const scanner::TokenType& token);
