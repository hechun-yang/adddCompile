// Lexer.h
#ifndef LEXER_H
#define LEXER_H

#include "Token.h"  // Assumes Token.h now has all the new TokenTypes
#include <string>
#include <vector>
#include <map>

class Lexer {
public:
    explicit Lexer(const std::string& source);
    Token getNextToken();
    // std::vector<Token> getAllTokens(); // Optional

private:
    // --- Member Variables (State) ---
    const std::string& source_code_;
    size_t current_pos_;
    size_t start_pos_; // Marks beginning of current lexeme

    int line_of_current_pos_;      // Line number of the character at current_pos_
    int col_of_current_pos_;       // Column number of the character at current_pos_

    int token_start_line_;         // Line where the current token started
    int token_start_col_;          // Column where the current token started

    static const std::map<std::string, TokenType> keywords_;

    // --- Private Helper Methods ---

    // Character handling and advancement
    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);

    // Skipping utility
    void skipWhitespaceAndComments();
    void skipSingleLineComment();
    void skipMultiLineComment(); // Now fully implemented

    // Token scanning methods for different types
    Token scanIdentifier();
    Token scanNumber();
    Token scanStringLiteral();
    Token scanCharLiteral(); // Added declaration

    // Token creation helper
    Token makeToken(TokenType type) const;
    // The following overload is less critical now that the main makeToken is robust,
    // but can be kept if specific custom lexemes are needed (e.g. for EOF, or future processed literals)
    Token makeToken(TokenType type, const std::string& custom_lexeme) const;

    // Error handling
    Token errorToken(const std::string& message) const;
};

#endif // LEXER_H