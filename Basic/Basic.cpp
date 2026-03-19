/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include <cstdlib>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "statement.hpp"
#include "evalstate.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"

using namespace std;


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);
Statement* parseStatement(TokenScanner& scanner);
int parseLineNumber(TokenScanner& scanner);
int stringToInt(std::string str);

/* Main program */

int main() {
    EvalState state;
    Program* program = new Program();
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            if (!std::getline(std::cin, input)) {
                // EOF reached
                delete program;
                return 0;
            }
            if (input.empty())
                continue;
            processLine(input, *program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        } catch (...) {
            delete program;
            throw;
        }
    }
    delete program;
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    // Check if this is a line number (program line)
    if (scanner.hasMoreTokens()) {
        std::string firstToken = scanner.nextToken();
        scanner.saveToken(firstToken);

        // Check if first token is a number
        bool isNumber = true;
        for (char c : firstToken) {
            if (!isdigit(c)) {
                isNumber = false;
                break;
            }
        }

        if (isNumber && !firstToken.empty()) {
            // This is a program line
            int lineNumber = stringToInt(firstToken);
            scanner.nextToken(); // consume the line number

            std::string remainingLine;
            if (scanner.hasMoreTokens()) {
                // Get the rest of the line after the line number
                int pos = line.find(firstToken) + firstToken.length();
                remainingLine = trim(line.substr(pos));
            }

            if (remainingLine.empty()) {
                // Empty line - remove the line
                program.removeSourceLine(lineNumber);
            } else {
                // Add or update the line
                program.addSourceLine(lineNumber, line);

                // Parse the statement
                scanner.setInput(remainingLine);
                Statement *stmt = parseStatement(scanner);
                if (stmt != nullptr) {
                    program.setParsedStatement(lineNumber, stmt);
                }
            }
        } else {
            // This is a direct command
            scanner.nextToken(); // consume the first token
            std::string command = toUpperCase(firstToken);

            if (command == "RUN") {
                int currentLine = program.getFirstLineNumber();
                while (currentLine != -1) {
                    state.clearJump();
                    Statement *stmt = program.getParsedStatement(currentLine);
                    if (stmt != nullptr) {
                        stmt->execute(state, program);
                    }

                    // Check if a jump was requested
                    if (state.isJumpRequested()) {
                        // Find the target line
                        int targetLine = -1;
                        for (int line = program.getFirstLineNumber(); line != -1;
                             line = program.getNextLineNumber(line)) {
                            if (line >= state.getCurrentLine() || state.getCurrentLine() == -1) {
                                if (targetLine == -1 || (state.getCurrentLine() != -1 && line == state.getCurrentLine())) {
                                    targetLine = line;
                                }
                            }
                        }
                        currentLine = targetLine;
                        state.setCurrentLine(targetLine);
                    } else {
                        // Normal flow - go to next line
                        currentLine = program.getNextLineNumber(currentLine);
                    }
                }
            } else if (command == "LIST") {
                program.listAllLines();
            } else if (command == "CLEAR") {
                program.clear();
                state.clear();
            } else if (command == "QUIT") {
                std::exit(0);
            } else if (command == "HELP") {
                std::cout << "Available commands: RUN, LIST, CLEAR, QUIT, HELP" << std::endl;
            } else if (command == "REM") {
                // REM in direct mode - just ignore
            } else if (command == "LET") {
                // Parse LET statement directly
                scanner.nextToken(); // should be variable
                std::string var = scanner.nextToken();
                scanner.nextToken(); // should be =
                Expression *exp = parseExp(scanner);
                if (exp != nullptr) {
                    int value = exp->eval(state);
                    state.setValue(var, value);
                    delete exp;
                }
            } else if (command == "PRINT") {
                // Parse PRINT statement directly
                Expression *exp = parseExp(scanner);
                if (exp != nullptr) {
                    int value = exp->eval(state);
                    std::cout << value << std::endl;
                    delete exp;
                }
            } else if (command == "INPUT") {
                // Parse INPUT statement directly
                std::string var = scanner.nextToken();
                int value;
                std::cin >> value;
                state.setValue(var, value);
            } else if (command == "END") {
                std::exit(0);
            } else if (command == "GOTO") {
                // Direct GOTO - not in program mode
                error("GOTO can only be used in program mode");
            } else {
                error("Unknown command: " + command);
            }
        }
    }
}

/*
 * Function: parseStatement
 * Usage: Statement *stmt = parseStatement(scanner);
 * --------------------------------------------------
 * Parses a BASIC statement from the token scanner.
 */

Statement* parseStatement(TokenScanner& scanner) {
    if (!scanner.hasMoreTokens()) {
        return nullptr;
    }

    std::string keyword = toUpperCase(scanner.nextToken());

    if (keyword == "REM") {
        // REM statement - collect rest of line as comment
        std::string comment;
        while (scanner.hasMoreTokens()) {
            if (!comment.empty()) comment += " ";
            comment += scanner.nextToken();
        }
        return new RemStatement(comment);
    } else if (keyword == "LET") {
        // LET statement: LET var = expression
        std::string var = scanner.nextToken();
        scanner.nextToken(); // consume =
        Expression *exp = parseExp(scanner);
        return new LetStatement(var, exp);
    } else if (keyword == "PRINT") {
        // PRINT statement: PRINT expression
        Expression *exp = parseExp(scanner);
        return new PrintStatement(exp);
    } else if (keyword == "INPUT") {
        // INPUT statement: INPUT var
        std::string var = scanner.nextToken();
        return new InputStatement(var);
    } else if (keyword == "END") {
        // END statement
        return new EndStatement();
    } else if (keyword == "GOTO") {
        // GOTO statement: GOTO linenumber
        int lineNum = stringToInt(scanner.nextToken());
        return new GotoStatement(lineNum);
    } else if (keyword == "IF") {
        // IF statement: IF exp1 op exp2 THEN linenumber
        Expression *exp1 = parseExp(scanner);
        std::string op = scanner.nextToken();
        Expression *exp2 = parseExp(scanner);
        scanner.nextToken(); // consume THEN
        int lineNum = stringToInt(scanner.nextToken());
        return new IfStatement(exp1, op, exp2, lineNum);
    }

    return nullptr;
}

/*
 * Function: stringToInt
 * Usage: int n = stringToInt(str);
 * --------------------------------
 * Converts a string to an integer.  This implementation is incomplete;
 * it needs to check that the string is a valid integer representation.
 */

int stringToInt(std::string str) {
    return std::stoi(str);
}

