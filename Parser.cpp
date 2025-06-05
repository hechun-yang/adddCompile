// Parser.cpp
#include "Parser.h"
#include <iostream> // For error messages (temporary)

Parser::Parser(Lexer& lexer) : lexer_(lexer) {
    // Initialize currentToken_ by consuming the first token from the lexer
    consumeToken();
}

void Parser::consumeToken() {
    currentToken_ = lexer_.getNextToken();
}

Token Parser::eat(TokenType expectedType, const std::string& errorMessage) {
    if (currentToken_.type == expectedType) {
        Token consumedToken = currentToken_;
        consumeToken(); // Move to the next token
        return consumedToken;
    }
    else {
        // Throw a more informative error using the custom ParseError
        throw ParseError(errorMessage + ". Expected " + tokenTypeToString(expectedType) +
            ", but got " + tokenTypeToString(currentToken_.type) +
            " ('" + currentToken_.lexeme + "')",
            currentToken_.line, currentToken_.column);
    }
}

void Parser::error(const std::string& message) {
    // More sophisticated error handling might involve collecting multiple errors
    // or attempting recovery. For now, we throw.
    throw ParseError(message, currentToken_.line, currentToken_.column);
}

// program ::= function_definition* EOF
std::unique_ptr<ProgramNode> Parser::parseProgram() {
    auto programNode = std::make_unique<ProgramNode>();
    // For V0.1, we expect exactly one function definition.
    // Later, this will be a loop: while (currentToken_.type != TokenType::END_OF_FILE && /* other top-level constructs */)
    if (currentToken_.type == TokenType::KEYWORD_INT) { // Assuming 'int' is the start of a function def for now
        programNode->functions.push_back(parseFunctionDefinition());
    }
    else if (currentToken_.type != TokenType::END_OF_FILE) {
        error("Expected function definition or EOF.");
    }


    eat(TokenType::END_OF_FILE, "Expected EOF at the end of the program");
    return programNode;
}

// function_definition ::= type IDENTIFIER "(" ")" "{" statement* "}"
std::unique_ptr<FunctionDefinitionNode> Parser::parseFunctionDefinition() {
    Token typeToken = parseType(); // Expects "int" for now
    Token idToken = eat(TokenType::IDENTIFIER, "Expected function name");
    eat(TokenType::LPAREN, "Expected '(' after function name");
    eat(TokenType::RPAREN, "Expected ')' after function parameters"); // Parameters later
    eat(TokenType::LBRACE, "Expected '{' before function body");

    std::vector<std::unique_ptr<StatementNode>> bodyStatements;
    while (currentToken_.type != TokenType::RBRACE && currentToken_.type != TokenType::END_OF_FILE) {
        bodyStatements.push_back(parseStatement());
    }

    eat(TokenType::RBRACE, "Expected '}' after function body");

    return std::make_unique<FunctionDefinitionNode>(typeToken, idToken, std::move(bodyStatements));
}

// type ::= "int"
Token Parser::parseType() {
    // For V0.1, we only support "int"
    // Later, this can be expanded: KEYWORD_VOID, KEYWORD_CHAR, IDENTIFIER (for custom types)
    return eat(TokenType::KEYWORD_INT, "Expected 'int' as return type");
}

// statement ::= return_statement
std::unique_ptr<StatementNode> Parser::parseStatement() {
    // Based on the current token, decide which kind of statement it is.
    // For V0.1, only return statements.
    if (currentToken_.type == TokenType::KEYWORD_RETURN) {
        return parseReturnStatement();
    }
    // else if (currentToken_.type == ...) { parseOtherStatement(); }
    else {
        error("Expected a statement (e.g., 'return')");
        return nullptr; // Should not be reached if error throws
    }
}

// return_statement ::= "return" expression ";"
std::unique_ptr<ReturnStatementNode> Parser::parseReturnStatement() {
    Token keywordToken = eat(TokenType::KEYWORD_RETURN, "Expected 'return' keyword");
    std::unique_ptr<ExpressionNode> expr = parseExpression();
    eat(TokenType::SEMICOLON, "Expected ';' after return statement");
    return std::make_unique<ReturnStatementNode>(keywordToken, std::move(expr));
}

// expression ::= INTEGER_LITERAL (for V0.1)
std::unique_ptr<ExpressionNode> Parser::parseExpression() {
    // For V0.1, expressions are just integer literals.
    // This will become the entry point for operator-precedence parsing later.
    return parsePrimaryExpression();
}

// primary_expression ::= INTEGER_LITERAL | ...
std::unique_ptr<ExpressionNode> Parser::parsePrimaryExpression() {
    if (currentToken_.type == TokenType::INTEGER_LITERAL) {
        Token intToken = currentToken_; // Copy before consuming
        consumeToken(); // or eat(TokenType::INTEGER_LITERAL)
        return std::make_unique<IntegerLiteralNode>(intToken);
    }
    // else if (currentToken_.type == TokenType::IDENTIFIER) { ... }
    // else if (currentToken_.type == TokenType::LPAREN) { parse parenthesized expression ... }
    else {
        error("Expected an integer literal or other primary expression");
        return nullptr; // Should not be reached
    }
}
