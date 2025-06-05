// Lexer.cpp
#include "Lexer.h"
#include <cctype>   
#include <iostream> 

// --- Static Keyword Map Definition ---
// This map stores all reserved keywords of our language and their corresponding TokenType.
const std::map<std::string, TokenType> Lexer::keywords_ = {
    {"if",     TokenType::KEYWORD_IF},
    {"else",   TokenType::KEYWORD_ELSE},
    {"return", TokenType::KEYWORD_RETURN},
    {"while",  TokenType::KEYWORD_WHILE},
    {"for",    TokenType::KEYWORD_FOR},     
    {"int",    TokenType::KEYWORD_INT},
    {"void",   TokenType::KEYWORD_VOID},
    {"char",   TokenType::KEYWORD_CHAR},    
    {"double", TokenType::KEYWORD_DOUBLE}, 
    {"struct", TokenType::KEYWORD_STRUCT},  
    {"class",  TokenType::KEYWORD_CLASS},   
    {"true",   TokenType::KEYWORD_TRUE},    
    {"false",  TokenType::KEYWORD_FALSE},   
    {"nullptr",TokenType::KEYWORD_NULLPTR},
    {"const",  TokenType::KEYWORD_CONST},
    {"static", TokenType::KEYWORD_STATIC},
    {"public", TokenType::KEYWORD_PUBLIC},
    {"private",TokenType::KEYWORD_PRIVATE},
    {"protected",TokenType::KEYWORD_PROTECTED}
    
};

// --- Constructor ---
Lexer::Lexer(const std::string& source):
    source_code_(source),
    current_pos_(0),
    start_pos_(0),
    line_of_current_pos_(1), 
    col_of_current_pos_(1), 
    token_start_line_(1),
    token_start_col_(1) {
}

// --- Main Public Method ---
Token Lexer::getNextToken() {
    skipWhitespaceAndComments();

    //mark start position
    start_pos_ = current_pos_;
    token_start_line_ = line_of_current_pos_;
    token_start_col_ = col_of_current_pos_;


    //check END
    if (isAtEnd()) {
        return Token(TokenType::END_OF_FILE, "", token_start_line_, token_start_col_);
    }


    char c = advance();

   //Wenn das c ist Identifer 
    if (std::isalpha(c) || c == '_') {
        return scanIdentifier(); 
    }

    //Wenn das c ist num
    if (std::isdigit(c)) {
        return scanNumber(); 
    }

    
    switch (c) {
        // Single-character tokens
    case '(': return makeToken(TokenType::LPAREN);
    case ')': return makeToken(TokenType::RPAREN);
    case '{': return makeToken(TokenType::LBRACE);
    case '}': return makeToken(TokenType::RBRACE);
    case '[': return makeToken(TokenType::LBRACKET); 
    case ']': return makeToken(TokenType::RBRACKET);
    case ',': return makeToken(TokenType::COMMA);
    case '.': return makeToken(TokenType::DOT); 
    case ';': return makeToken(TokenType::SEMICOLON);
    case '?': return makeToken(TokenType::QUESTION);
    case ':':
        if (match(':')) return makeToken(TokenType::DOUBLE_COLON); // ::
        return makeToken(TokenType::COLON);      // :
    case '~': return makeToken(TokenType::TILDE);      // ~ 

        // Operators that might be single or double character
    case '!':
        return match('=') ? makeToken(TokenType::BANG_EQUAL) : makeToken(TokenType::BANG); // != or !
    case '=':
        return match('=') ? makeToken(TokenType::EQUAL_EQUAL) : makeToken(TokenType::EQUAL); // == or =
    case '+':
        if (match('+')) return makeToken(TokenType::PLUS_PLUS);    // ++
        if (match('=')) return makeToken(TokenType::PLUS_EQUAL);   // +=
        return makeToken(TokenType::PLUS);                         // +
    case '-':
        if (match('-')) return makeToken(TokenType::MINUS_MINUS);  // --
        if (match('=')) return makeToken(TokenType::MINUS_EQUAL);  // -=
        if (match('>')) return makeToken(TokenType::ARROW);        // ->
        return makeToken(TokenType::MINUS);                        // -
    case '*':
        return match('=') ? makeToken(TokenType::STAR_EQUAL) : makeToken(TokenType::STAR); // *= or *
    case '/':
        return match('=') ? makeToken(TokenType::SLASH_EQUAL) : makeToken(TokenType::SLASH); // /= or /
    case '%':
        return match('=') ? makeToken(TokenType::PERCENT_EQUAL) : makeToken(TokenType::PERCENT); // %= or %
    case '<':
        if (match('<')) { // <<
            return match('=') ? makeToken(TokenType::LESS_LESS_EQUAL) : makeToken(TokenType::LESS_LESS); // <<= or <<
        }
        return match('=') ? makeToken(TokenType::LESS_EQUAL) : makeToken(TokenType::LESS); // <= or <
    case '>':
        if (match('>')) { // >>
            return match('=') ? makeToken(TokenType::GREATER_GREATER_EQUAL) : makeToken(TokenType::GREATER_GREATER); // >>= or >>
        }
        return match('=') ? makeToken(TokenType::GREATER_EQUAL) : makeToken(TokenType::GREATER); // >= or >
    case '&':
        if (match('&')) return makeToken(TokenType::AMPERSAND_AMPERSAND); // &&
        if (match('=')) return makeToken(TokenType::AMPERSAND_EQUAL);   // &=
        return makeToken(TokenType::AMPERSAND);                        // &
    case '|':
        if (match('|')) return makeToken(TokenType::PIPE_PIPE);          // ||
        if (match('=')) return makeToken(TokenType::PIPE_EQUAL);         // |=
        return makeToken(TokenType::PIPE);                             // |
    case '^':
        return match('=') ? makeToken(TokenType::CARET_EQUAL) : makeToken(TokenType::CARET); // ^= or ^

       
    case '"':
        return scanStringLiteral(); // `"` is the first char
    case '\'':
        return scanCharLiteral();   // `'` is the first char (Added)

    default:
       
        return errorToken("Unexpected character.");
    }
}

// --- Private Helper Methods ---

bool Lexer::isAtEnd() const {
    return current_pos_ >= source_code_.length();
}

char Lexer::advance() {
    
    if (current_pos_ >= source_code_.length()) return '\0';

    char current_char = source_code_[current_pos_];
    current_pos_++; 

    //deal line and col 's num 
    if (current_char == '\n') {
        line_of_current_pos_++;
        col_of_current_pos_ = 1; 
    }
    else {
        col_of_current_pos_++;
    }
    return current_char;
}

char Lexer::peek() const {
    if (current_pos_ >= source_code_.length()) return '\0'; // EOF character
    return source_code_[current_pos_];
}

char Lexer::peekNext() const {
    if (current_pos_ + 1 >= source_code_.length()) return '\0';
    return source_code_[current_pos_ + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source_code_[current_pos_] != expected) return false;

    
    current_pos_++; 
    if (expected == '\n') { 
        line_of_current_pos_++;
        col_of_current_pos_ = 1;
    }
    else {
        col_of_current_pos_++;
    }
    return true;
}

void Lexer::skipWhitespaceAndComments() {
    while (true) {
        if (isAtEnd()) break;
        char c = peek();

        switch (c) {
        case ' ':
        case '\r': 
        case '\t': 
            advance();
            break;
        case '\n': 
            advance();
            break;
        case '/':
            if (peekNext() == '/') {
                skipSingleLineComment();
            }
            else if (peekNext() == '*') {
                skipMultiLineComment();
            }
            else {
                return;//das ist division
            }
            break;
        default:
            return; 
        }
    }
}

void Lexer::skipSingleLineComment() {
    
    advance(); 
    advance(); 

    while (peek() != '\n' && !isAtEnd()) {
        advance();
    }
    
}

void Lexer::skipMultiLineComment() {
    // Assumes '/*' has been identified. Consume them.
    advance(); // Consume /
    advance(); // Consume *

    int nesting = 1; // Handle nested block comments (though C/C++ don't support them natively, some lexers do)
    // For standard C/C++, nesting can be ignored, first '*/' closes.
    // Let's implement standard C/C++ behavior (no nesting).

    while (!isAtEnd()) {
        if (peek() == '*' && peekNext() == '/') {
            advance(); // Consume *
            advance(); // Consume /
            return;    // End of comment
        }
        // For C/C++ standard, no need to check for nested /*
        // else if (peek() == '/' && peekNext() == '*') {
        //     advance(); advance(); nesting++;
        // }
        else {
            advance(); // Consume any other character within the comment
        }
    }
    // If we reach here, it's an unterminated multi-line comment (error)
    // We can't make an error token here directly as this is a void function.
    // The main getNextToken loop will eventually hit EOF.
    // A more robust lexer might flag this error earlier.
    // For now, it will just consume until EOF.
    // To report error:
    // if (isAtEnd()) {
    //     // Need a way to signal error from here, or let getNextToken produce an error token based on current state
    //     std::cerr << "Lexical Error [Line " << token_start_line_ // This might be tricky to get right from here
    //               << "]: Unterminated multi-line comment." << std::endl;
    // }
}


Token Lexer::scanIdentifier() {
    // The first character (which was alpha or '_') was already consumed by advance() in getNextToken()
    // and is part of the lexeme. `start_pos_` points to it.
    // `current_pos_` is now pointing to the character *after* the first one.
    while (std::isalnum(peek()) || peek() == '_') {
        advance();
    }

    std::string lexeme = source_code_.substr(start_pos_, current_pos_ - start_pos_);
    auto it = keywords_.find(lexeme);
    if (it != keywords_.end()) {
        return makeToken(it->second); // It's a keyword
    }
    return makeToken(TokenType::IDENTIFIER); // It's a user-defined identifier
}

Token Lexer::scanNumber() {
    // The first digit was already consumed by advance() in getNextToken().
    // `start_pos_` points to it. `current_pos_` is after it.
    while (std::isdigit(peek())) {
        advance();
    }

    // Look for a fractional part (for floating point numbers)
    // For a simple integer-only lexer, the above is enough.
    // If supporting floats:
    /*
    if (peek() == '.' && std::isdigit(peekNext())) {
        advance(); // Consume the '.'
        while (std::isdigit(peek())) {
            advance();
        }
        // Could also look for an exponent part (e.g., e+10, E-5)
        // if (std::tolower(peek()) == 'e') { ... }
        return makeToken(TokenType::DOUBLE_LITERAL); // Or FLOAT_LITERAL
    }
    */
    return makeToken(TokenType::INTEGER_LITERAL);
}

Token Lexer::scanStringLiteral() {
    // The opening quote `"` was consumed by advance() in getNextToken().
    // `start_pos_` points to the opening quote.
    // `current_pos_` is at the first character *inside* the string.
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\\') { // Handle escape sequences
            advance(); // Consume '\'
            if (!isAtEnd()) {
                // Consume the character being escaped (e.g., n, t, ", \)
                // A more robust lexer would check if it's a valid escape sequence.
                advance();
            }
        }
        else {
            advance();
        }
    }

    if (isAtEnd()) {
        // Unterminated string
        return errorToken("Unterminated string literal.");
    }

    // Found the closing quote.
    advance(); // Consume the closing quote.

    // The lexeme includes the quotes.
    // The actual string value (unescaped) could be stored in token.literal_value if needed.
    return makeToken(TokenType::STRING_LITERAL);
}

Token Lexer::scanCharLiteral() {
    // The opening quote `'` was consumed by advance() in getNextToken().
    // `start_pos_` points to the opening quote.
    // `current_pos_` is at the first character *inside* the char literal.
    if (isAtEnd()) return errorToken("Unterminated character literal.");

    if (peek() == '\\') { // Escape sequence
        advance(); // Consume '\'
        if (isAtEnd()) return errorToken("Unterminated escape sequence in char literal.");
        advance(); // Consume escaped character
    }
    else { // Regular character
        advance(); // Consume the character
    }

    if (peek() != '\'') {
        // If too many chars or no closing quote before newline etc.
        // This simple version assumes a char literal is 'c' or '\c'.
        // A C++ char literal can be longer for universal character names, e.g., '\u00C1', or wide chars L'a'.
        // We are keeping it simple for now.
        while (peek() != '\'' && peek() != '\n' && !isAtEnd()) advance(); // Consume until ' or newline or EOF
        return errorToken("Invalid or malformed character literal. Expected closing '.");
    }

    advance(); // Consume the closing quote.
    return makeToken(TokenType::CHAR_LITERAL);
}


// --- Token Creation Helpers ---
Token Lexer::makeToken(TokenType type) const {
    std::string lexeme = "";
    if (start_pos_ < current_pos_ && current_pos_ <= source_code_.length()) { // Ensure valid substring
        lexeme = source_code_.substr(start_pos_, current_pos_ - start_pos_);
    }
    return Token(type, lexeme, token_start_line_, token_start_col_);
}

Token Lexer::makeToken(TokenType type, const std::string& custom_lexeme) const {
    // This version is less used now that the primary makeToken correctly extracts the lexeme.
    // It's useful if the lexeme is not a direct substring, e.g. for EOF or if we were unescaping strings here.
    return Token(type, custom_lexeme, token_start_line_, token_start_col_);
}

// --- Error Handling ---
Token Lexer::errorToken(const std::string& message) const {
    // For now, just prints to cerr. A real compiler would collect errors.
    std::string problematic_lexeme = "";
    // Try to get the character(s) that caused the error for the lexeme
    if (start_pos_ < source_code_.length()) {
        // If current_pos_ advanced, use the range. Otherwise, just start_pos_ char.
        size_t length = (current_pos_ > start_pos_) ? (current_pos_ - start_pos_) : 1;
        if (start_pos_ + length <= source_code_.length()) {
            problematic_lexeme = source_code_.substr(start_pos_, length);
        }
        else if (start_pos_ < source_code_.length()) { // Fallback for single char if length calc is off
            problematic_lexeme = source_code_.substr(start_pos_, 1);
        }
    }


    std::cerr << "Lexical Error [Line " << token_start_line_
        << ", Col " << token_start_col_
        << " near '" << problematic_lexeme << "'"
        << "]: " << message << std::endl;

    return Token(TokenType::UNKNOWN, problematic_lexeme, token_start_line_, token_start_col_);
}

// Just a Helper .
std::string tokenTypeToString(TokenType type) {
    switch (type) {
    case TokenType::LPAREN: return "LPAREN";
    case TokenType::RPAREN: return "RPAREN";
    case TokenType::LBRACE: return "LBRACE";
    case TokenType::RBRACE: return "RBRACE";
    case TokenType::LBRACKET: return "LBRACKET";
    case TokenType::RBRACKET: return "RBRACKET";
    case TokenType::COMMA: return "COMMA";
    case TokenType::DOT: return "DOT";
    case TokenType::MINUS: return "MINUS";
    case TokenType::PLUS: return "PLUS";
    case TokenType::SEMICOLON: return "SEMICOLON";
    case TokenType::SLASH: return "SLASH";
    case TokenType::STAR: return "STAR";
    case TokenType::QUESTION: return "QUESTION";
    case TokenType::COLON: return "COLON";
    case TokenType::DOUBLE_COLON: return "DOUBLE_COLON";
    case TokenType::TILDE: return "TILDE";

    case TokenType::BANG: return "BANG";
    case TokenType::BANG_EQUAL: return "BANG_EQUAL";
    case TokenType::EQUAL: return "EQUAL";
    case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
    case TokenType::PLUS_PLUS: return "PLUS_PLUS";
    case TokenType::PLUS_EQUAL: return "PLUS_EQUAL";
    case TokenType::MINUS_MINUS: return "MINUS_MINUS";
    case TokenType::MINUS_EQUAL: return "MINUS_EQUAL";
    case TokenType::ARROW: return "ARROW";
    case TokenType::STAR_EQUAL: return "STAR_EQUAL";
    case TokenType::SLASH_EQUAL: return "SLASH_EQUAL";
    case TokenType::PERCENT: return "PERCENT";
    case TokenType::PERCENT_EQUAL: return "PERCENT_EQUAL";

    case TokenType::GREATER: return "GREATER";
    case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
    case TokenType::LESS: return "LESS";
    case TokenType::LESS_EQUAL: return "LESS_EQUAL";

    case TokenType::LESS_LESS: return "LESS_LESS";
    case TokenType::LESS_LESS_EQUAL: return "LESS_LESS_EQUAL";
    case TokenType::GREATER_GREATER: return "GREATER_GREATER";
    case TokenType::GREATER_GREATER_EQUAL: return "GREATER_GREATER_EQUAL";

    case TokenType::AMPERSAND: return "AMPERSAND";
    case TokenType::AMPERSAND_AMPERSAND: return "AMPERSAND_AMPERSAND";
    case TokenType::AMPERSAND_EQUAL: return "AMPERSAND_EQUAL";
    case TokenType::PIPE: return "PIPE";
    case TokenType::PIPE_PIPE: return "PIPE_PIPE";
    case TokenType::PIPE_EQUAL: return "PIPE_EQUAL";
    case TokenType::CARET: return "CARET";
    case TokenType::CARET_EQUAL: return "CARET_EQUAL";


    case TokenType::IDENTIFIER: return "IDENTIFIER";
    case TokenType::STRING_LITERAL: return "STRING_LITERAL";
    case TokenType::INTEGER_LITERAL: return "INTEGER_LITERAL";
    case TokenType::CHAR_LITERAL: return "CHAR_LITERAL";
        // case TokenType::DOUBLE_LITERAL: return "DOUBLE_LITERAL";


    case TokenType::KEYWORD_IF: return "KEYWORD_IF";
    case TokenType::KEYWORD_ELSE: return "KEYWORD_ELSE";
    case TokenType::KEYWORD_RETURN: return "KEYWORD_RETURN";
    case TokenType::KEYWORD_WHILE: return "KEYWORD_WHILE";
    case TokenType::KEYWORD_FOR: return "KEYWORD_FOR";
    case TokenType::KEYWORD_INT: return "KEYWORD_INT";
    case TokenType::KEYWORD_VOID: return "KEYWORD_VOID";
    case TokenType::KEYWORD_CHAR: return "KEYWORD_CHAR";
    case TokenType::KEYWORD_DOUBLE: return "KEYWORD_DOUBLE";
    case TokenType::KEYWORD_STRUCT: return "KEYWORD_STRUCT";
    case TokenType::KEYWORD_CLASS: return "KEYWORD_CLASS";
    case TokenType::KEYWORD_TRUE: return "KEYWORD_TRUE";
    case TokenType::KEYWORD_FALSE: return "KEYWORD_FALSE";
    case TokenType::KEYWORD_NULLPTR: return "KEYWORD_NULLPTR";
    case TokenType::KEYWORD_CONST: return "KEYWORD_CONST";
    case TokenType::KEYWORD_STATIC: return "KEYWORD_STATIC";
    case TokenType::KEYWORD_PUBLIC: return "KEYWORD_PUBLIC";
    case TokenType::KEYWORD_PRIVATE: return "KEYWORD_PRIVATE";
    case TokenType::KEYWORD_PROTECTED: return "KEYWORD_PROTECTED";


    case TokenType::UNKNOWN: return "UNKNOWN";
    case TokenType::END_OF_FILE: return "END_OF_FILE";
    default: return "!!! UNHANDLED TOKEN TYPE !!!";
    }
}