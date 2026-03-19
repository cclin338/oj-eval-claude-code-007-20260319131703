/*
 * File: evalstate.cpp
 * -------------------
 * This file implements the EvalState class, which defines a symbol
 * table for keeping track of the value of identifiers.  The public
 * methods are simple enough that they need no individual documentation.
 */


#include "evalstate.hpp"


//using namespace std;

/* Implementation of the EvalState class */

EvalState::EvalState() : currentLine(-1), jumpRequested(false), jumpTarget(-1) {
    /* Empty */
}

EvalState::~EvalState() {
    /* Empty */
}

void EvalState::setValue(std::string var, int value) {
    if(isDefined(var)) symbolTable[var] = value;
    else symbolTable.emplace(var,value);
}

int EvalState::getValue(std::string var) {
    if(isDefined(var)) return symbolTable[var];
    else return 0;
}

bool EvalState::isDefined(std::string var) {
    return symbolTable.find(var)!=symbolTable.end();
}

void EvalState::clear() {
    symbolTable.clear();
    currentLine = -1;
    jumpRequested = false;
    jumpTarget = -1;
}

void EvalState::setCurrentLine(int lineNumber) {
    if (lineNumber >= 0) {
        jumpRequested = true;
        jumpTarget = lineNumber;
    } else {
        jumpRequested = false;
        jumpTarget = -1;
    }
}

int EvalState::getCurrentLine() const {
    return jumpRequested ? jumpTarget : currentLine;
}

bool EvalState::isJumpRequested() const {
    return jumpRequested;
}

void EvalState::clearJump() {
    jumpRequested = false;
    jumpTarget = -1;
}