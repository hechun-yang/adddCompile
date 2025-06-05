// Parser.h
#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"    // Needs Lexer to get tokens
#include "AstNode.h"  // Needs AST node definitions
#include <vector>
#include <memory>     // For std::unique_ptr
#include <stdexcept>  // For std::runtime_error (or custom error class)

class Parser {
public:
    Parser(Lexer& lexer);

    // Top-level parsing function, returns the root of the AST
    std::unique_ptr<ProgramNode> parseProgram();

private:
    Lexer& lexer_;
    Token currentToken_;
    // Token peekToken_; // For LL(k) where k > 1, not needed for simple LL(1)

    // Helper to advance to the next token
    void consumeToken();

    // Helper to check current token type and consume it if it matches.
    // Throws an error (or logs and attempts recovery) if it doesn't match.
    Token eat(TokenType expectedType, const std::string& errorMessage = "Unexpected token");

    // --- Parsing methods for grammar rules (non-terminals) ---
    // Each of these will correspond to a rule in our grammar.

    // program ::= function_definition EOF
    // (parseProgram already declared)

    // function_definition ::= type IDENTIFIER "(" ")" "{" statement* "}"
    std::unique_ptr<FunctionDefinitionNode> parseFunctionDefinition();

    // type ::= "int"
    Token parseType(); // Returns the type token (e.g., "int")

    // statement ::= return_statement
    std::unique_ptr<StatementNode> parseStatement();

    // return_statement ::= "return" expression ";"
    std::unique_ptr<ReturnStatementNode> parseReturnStatement();

    // expression ::= INTEGER_LITERAL
    std::unique_ptr<ExpressionNode> parseExpression();
    std::unique_ptr<ExpressionNode> parsePrimaryExpression(); // For literals, parentheses, etc.

    // Error reporting utility
    void error(const std::string& message); // Throws a ParseError or std::runtime_error
};

// Custom exception for parsing errors (optional, can use std::runtime_error)
class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message, int line, int col)
        : std::runtime_error(message + " at line " + std::to_string(line) + " col " + std::to_string(col)),
        line_(line), col_(col) {}
    int getLine() const { return line_; }
    int getColumn() const { return col_; }
private:
    int line_;
    int col_;
};


#endif // PARSER_H
