/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"



#include <map>

Program::Program() = default;

Program::~Program() {
    clear();
}

void Program::clear() {
    for (auto& pair : lines) {
        if (pair.second.parsedStatement != nullptr) {
            delete pair.second.parsedStatement;
        }
    }
    lines.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    auto it = lines.find(lineNumber);
    if (it != lines.end()) {
        // Line exists - replace text and delete old parsed statement
        if (it->second.parsedStatement != nullptr) {
            delete it->second.parsedStatement;
            it->second.parsedStatement = nullptr;
        }
        it->second.sourceLine = line;
    } else {
        // New line - insert it
        lines[lineNumber] = LineInfo(line);
    }
}

void Program::removeSourceLine(int lineNumber) {
    auto it = lines.find(lineNumber);
    if (it != lines.end()) {
        if (it->second.parsedStatement != nullptr) {
            delete it->second.parsedStatement;
        }
        lines.erase(it);
    }
}

std::string Program::getSourceLine(int lineNumber) {
    auto it = lines.find(lineNumber);
    if (it != lines.end()) {
        return it->second.sourceLine;
    }
    return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    auto it = lines.find(lineNumber);
    if (it == lines.end()) {
        error("Line number does not exist in program");
    }
    if (it->second.parsedStatement != nullptr) {
        delete it->second.parsedStatement;
    }
    it->second.parsedStatement = stmt;
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
    auto it = lines.find(lineNumber);
    if (it != lines.end()) {
        return it->second.parsedStatement;
    }
    return nullptr;
}

int Program::getFirstLineNumber() {
    if (lines.empty()) {
        return -1;
    }
    return lines.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    auto it = lines.find(lineNumber);
    if (it == lines.end()) {
        return -1;
    }
    ++it;
    if (it != lines.end()) {
        return it->first;
    }
    return -1;
}

void Program::listAllLines() {
    for (const auto& pair : lines) {
        std::cout << pair.second.sourceLine << std::endl;
    }
}


