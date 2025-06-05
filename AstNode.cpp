// AstNode.cpp
#include "AstNode.h"
#include <iostream>

// --- Implementations of print methods for each AST node ---

void IntegerLiteralNode::print(int indentLevel) const {
    std::cout << indent(indentLevel) << "IntegerLiteralNode: " << token.lexeme << " (Value: " << getValue() << ")" << std::endl;
}

void ReturnStatementNode::print(int indentLevel) const {
    std::cout << indent(indentLevel) << "ReturnStatementNode (" << keywordToken.lexeme << ")" << std::endl;
    if (returnValue) {
        returnValue->print(indentLevel + 1);
    }
    else {
        std::cout << indent(indentLevel + 1) << "<no return value>" << std::endl;
    }
}

void FunctionDefinitionNode::print(int indentLevel) const {
    std::cout << indent(indentLevel) << "FunctionDefinitionNode: " << returnTypeToken.lexeme << " " << identifierToken.lexeme << "()" << std::endl;
    std::cout << indent(indentLevel) << "Body:" << std::endl;
    if (body.empty()) {
        std::cout << indent(indentLevel + 1) << "<empty body>" << std::endl;
    }
    else {
        for (const auto& stmt : body) {
            stmt->print(indentLevel + 1);
        }
    }
}

void ProgramNode::print(int indentLevel) const {
    std::cout << indent(indentLevel) << "ProgramNode" << std::endl;
    for (const auto& func : functions) {
        func->print(indentLevel + 1);
    }
}