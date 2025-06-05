#pragma onc// Token.h
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <variant> // C++17, for token values (optional, can start with just string lexeme)


// TokenType: Defines all possible types of tokens our lexer can produce.

// We'll add more as we go.
// Token.h (TokenType enum additions)
enum class TokenType {
    // Single-character tokens
    LPAREN, RPAREN,     // ( )
    LBRACE, RBRACE,     // { }
    LBRACKET, RBRACKET, // [ ] (Added)
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    QUESTION, COLON, TILDE, // Added

    // One or two (or three for <<=, >>=) character tokens
    BANG, BANG_EQUAL,             // ! !=
    EQUAL, EQUAL_EQUAL,           // = ==
    PLUS_PLUS, PLUS_EQUAL,       // ++, += (Added specific PLUS tokens)
    MINUS_MINUS, MINUS_EQUAL, ARROW, // --, -=, -> (Added specific MINUS tokens)
    STAR_EQUAL,                   // *= (Added specific STAR token)
    SLASH_EQUAL,                  // /= (Added specific SLASH token)
    PERCENT, PERCENT_EQUAL,       // %, %= (Added)
    DOUBLE_COLON,                 // :: (Added)


    GREATER, GREATER_EQUAL,       // > >=
    LESS, LESS_EQUAL,             // < <=

    LESS_LESS, LESS_LESS_EQUAL,               // <<, <<= (Added)
    GREATER_GREATER, GREATER_GREATER_EQUAL,   // >>, >>= (Added)

    AMPERSAND, AMPERSAND_AMPERSAND, AMPERSAND_EQUAL, // &, &&, &= (Added)
    PIPE, PIPE_PIPE, PIPE_EQUAL,                   // |, ||, |= (Added)
    CARET, CARET_EQUAL,                             // ^, ^= (Added)


    // Literals
    IDENTIFIER,
    STRING_LITERAL,
    INTEGER_LITERAL,
    CHAR_LITERAL,       
    FLOAT_LITERAL,      // 3.14f, 2.0
    DOUBLE_LITERAL,     // 3.14, 2.0 
    // DOUBLE_LITERAL,

    // Keywords
    KEYWORD_IF, KEYWORD_ELSE, KEYWORD_WHILE, KEYWORD_RETURN, KEYWORD_FOR,
    KEYWORD_INT, KEYWORD_VOID, KEYWORD_CHAR, 
    KEYWORD_STRUCT, KEYWORD_CLASS,
    KEYWORD_TRUE, KEYWORD_FALSE, KEYWORD_NULLPTR,
    KEYWORD_CONST, KEYWORD_STATIC,
    KEYWORD_PUBLIC, KEYWORD_PRIVATE, KEYWORD_PROTECTED,
    KEYWORD_AUTO, KEYWORD_BREAK, KEYWORD_CASE,
    KEYWORD_CONTINUE, KEYWORD_DEFAULT, KEYWORD_DO, KEYWORD_DOUBLE, 
    KEYWORD_ENUM, KEYWORD_EXTERN, KEYWORD_FLOAT,  KEYWORD_GOTO,
    KEYWORD_LONG, KEYWORD_REGISTER, 
    KEYWORD_SHORT, KEYWORD_SIGNED, KEYWORD_SIZEOF, 
    KEYWORD_SWITCH, KEYWORD_TYPEDEF, KEYWORD_UNION,
    KEYWORD_UNSIGNED,  KEYWORD_VOLATILE, 


    // Special tokens
    UNKNOWN,
    END_OF_FILE
};

// Helper function to convert TokenType to a printable string (useful for debugging)
std::string tokenTypeToString(TokenType type);

// Token: Represents a single lexical unit.
struct Token {
    TokenType type;         // The type of the token
    std::string lexeme;     // The actual character sequence from the source
    // std::variant<std::monostate, int, double, std::string> literal_value; // Optional: store parsed literal value
    int line;               // Line number where the token begins
    int column;             // Column number where the token begins

    // Constructor
    Token(TokenType type, std::string lexeme, int line, int column)
        : type(type), lexeme(std::move(lexeme)), line(line), column(column) {}

    // Default constructor for cases like uninitialized token
    Token() : type(TokenType::UNKNOWN), lexeme(""), line(0), column(0) {}
};

#endif // TOKEN_He
