/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include <iostream>


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

/* Implementation of RemStatement */

RemStatement::RemStatement(const std::string& comment) : comment(comment) {}

void RemStatement::execute(EvalState &state, Program &program) {
    // REM statements do nothing
}

/* Implementation of LetStatement */

LetStatement::LetStatement(const std::string& var, Expression *exp)
    : variable(var), expression(exp) {}

LetStatement::~LetStatement() {
    delete expression;
}

void LetStatement::execute(EvalState &state, Program &program) {
    int value = expression->eval(state);
    state.setValue(variable, value);
}

/* Implementation of PrintStatement */

PrintStatement::PrintStatement(Expression *exp) : expression(exp) {}

PrintStatement::~PrintStatement() {
    delete expression;
}

void PrintStatement::execute(EvalState &state, Program &program) {
    int value = expression->eval(state);
    std::cout << value << std::endl;
}

/* Implementation of InputStatement */

InputStatement::InputStatement(const std::string& var) : variable(var) {}

void InputStatement::execute(EvalState &state, Program &program) {
    int value;
    std::cin >> value;
    state.setValue(variable, value);
}

/* Implementation of EndStatement */

EndStatement::EndStatement() {}

void EndStatement::execute(EvalState &state, Program &program) {
    exit(0);
}

/* Implementation of GotoStatement */

GotoStatement::GotoStatement(int lineNum) : targetLine(lineNum) {}

void GotoStatement::execute(EvalState &state, Program &program) {
    state.setCurrentLine(targetLine);
}

/* Implementation of IfStatement */

IfStatement::IfStatement(Expression *exp1, const std::string& op, Expression *exp2, int lineNum)
    : expr1(exp1), expr2(exp2), operation(op), targetLine(lineNum) {}

IfStatement::~IfStatement() {
    delete expr1;
    delete expr2;
}

void IfStatement::execute(EvalState &state, Program &program) {
    int val1 = expr1->eval(state);
    int val2 = expr2->eval(state);
    bool condition = false;

    if (operation == "=") {
        condition = (val1 == val2);
    } else if (operation == "<") {
        condition = (val1 < val2);
    } else if (operation == ">") {
        condition = (val1 > val2);
    } else if (operation == "<>") {
        condition = (val1 != val2);
    } else if (operation == "<=") {
        condition = (val1 <= val2);
    } else if (operation == ">=") {
        condition = (val1 >= val2);
    }

    if (condition) {
        state.setCurrentLine(targetLine);
    }
}
