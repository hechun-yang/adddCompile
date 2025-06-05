// main.cpp
#include "Lexer.h"
#include "Parser.h" // Include Parser
#include "AstNode.h"  // Include AstNode for ProgramNode
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
    std::string sourceCode;
    std::string inputFileName = "stdin";

    if (argc >= 2) {
        inputFileName = argv[1];
        std::ifstream file(inputFileName);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << inputFileName << std::endl;
            return 1;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        sourceCode = buffer.str();
        file.close();
    }
    else {
        std::cout << "No input file provided. Using a default test string for V0.1 Parser.\n";
        sourceCode = "int main() {\n"
            "  return 123;\n"
            // "  return 456;\n" // Can add more return statements
            "}\n";
        inputFileName = "default test string (Parser V0.1)";
    }

    if (sourceCode.empty()) {
        std::cerr << "Error: No source code to parse." << std::endl;
        return 1;
    }

    std::cout << "Source Code from: " << inputFileName << "\n--- Start --- \n" << sourceCode << "\n--- End ---" << std::endl;

    Lexer lexer(sourceCode);
    Parser parser(lexer);

    try {
        std::cout << "\nParsing program..." << std::endl;
        std::unique_ptr<ProgramNode> astRoot = parser.parseProgram();
        std::cout << "Parsing successful!" << std::endl;

        if (astRoot) {
            std::cout << "\n--- Abstract Syntax Tree ---" << std::endl;
            astRoot->print(0);
            std::cout << "--- End of AST ---" << std::endl;
        }
        else {
            std::cout << "AST parsing resulted in a null root." << std::endl;
        }

    }
    catch (const ParseError& e) {
        std::cerr << "Parser Error: " << e.what() << std::endl;
        // Optionally print the token queue if you want to debug
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}