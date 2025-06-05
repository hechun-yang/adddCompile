// AstNode.h
#ifndef ASTNODE_H
#define ASTNODE_H

#include "Token.h" // We'll need Token for storing lexemes, types, positions
#include <vector>
#include <string>
#include <memory> // For std::unique_ptr

// Forward declaration for a potential Visitor pattern (good for later)
class AstVisitor;

// Base class for all AST nodes
class AstNode {
public:
    virtual ~AstNode() = default; // Virtual destructor for proper cleanup

    // Pure virtual function for printing the AST (for debugging)
    // 'indent' is for pretty printing.
    virtual void print(int indentLevel = 0) const = 0;

    // Optional: Accept method for the Visitor pattern
    // virtual void accept(AstVisitor* visitor) = 0;

    // Optional: Store line/column info for error reporting from AST
    // int line;
    // int column;
    // AstNode(int l, int c) : line(l), column(c) {}
    // AstNode() : line(0), column(0) {} // Default
};

// --- Expression Nodes ---
class ExpressionNode : public AstNode {
    // Common properties or methods for all expressions can go here
};

class IntegerLiteralNode : public ExpressionNode {
public:
    Token token; // The INTEGER_LITERAL token itself

    IntegerLiteralNode(Token t) : token(std::move(t)) {
        // Assert or check that t.type is indeed INTEGER_LITERAL
    }

    long long getValue() const { // Helper to get the integer value
        return std::stoll(token.lexeme); // std::stoll for long long
    }

    void print(int indentLevel = 0) const override;
    // void accept(AstVisitor* visitor) override { visitor->visit(this); }
};

// --- Statement Nodes ---
class StatementNode : public AstNode {
    // Common properties or methods for all statements
};

class ReturnStatementNode : public StatementNode {
public:
    Token keywordToken; // The 'return' keyword token
    std::unique_ptr<ExpressionNode> returnValue; // The expression being returned

    ReturnStatementNode(Token keyword, std::unique_ptr<ExpressionNode> expr)
        : keywordToken(std::move(keyword)), returnValue(std::move(expr)) {}

    void print(int indentLevel = 0) const override;
    // void accept(AstVisitor* visitor) override { visitor->visit(this); }
};


// --- Definition Nodes (like function definitions) ---
class FunctionDefinitionNode : public AstNode { // Could also be a type of Statement or a top-level declaration
public:
    Token returnTypeToken; // Token for the return type (e.g., "int")
    Token identifierToken; // Token for the function name
    // Parameters will be added later: std::vector<std::unique_ptr<VariableDeclarationNode>> parameters;
    std::vector<std::unique_ptr<StatementNode>> body; // Statements in the function body

    FunctionDefinitionNode(Token retType, Token id, std::vector<std::unique_ptr<StatementNode>> bodyStmts)
        : returnTypeToken(std::move(retType)),
        identifierToken(std::move(id)),
        body(std::move(bodyStmts)) {}

    void print(int indentLevel = 0) const override;
    // void accept(AstVisitor* visitor) override { visitor->visit(this); }
};


// --- Program Node (Root of the AST) ---
class ProgramNode : public AstNode {
public:
    // For now, a program is just a list of function definitions.
    // Later, it could include global variables, class definitions, etc.
    std::vector<std::unique_ptr<FunctionDefinitionNode>> functions;

    void print(int indentLevel = 0) const override;
    // void accept(AstVisitor* visitor) override { visitor->visit(this); }
};

// Helper for indentation in print methods
inline std::string indent(int level) {
    return std::string(level * 2, ' '); // 2 spaces per indent level
}

#endif // ASTNODE_H