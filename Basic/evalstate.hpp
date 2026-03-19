/*
 * File: evalstate.h
 * -----------------
 * This interface exports a class called EvalState, which keeps track
 * of additional information required by the evaluator, most notably
 * the values of variables.
 */

#ifndef _evalstate_h
#define _evalstate_h

#include <string>
#include <map>

/*
 * Class: EvalState
 * ----------------
 * This class is passed by reference through the recursive levels
 * of the evaluator and contains information from the evaluation
 * environment that the evaluator may need to know.  In this
 * version, the only information maintained by the EvalState class
 * is a symbol table that maps variable names into their values.
 * In your implementation, you may include additional information
 * in the EvalState class.
 */

class EvalState {

public:

/*
 * Constructor: EvalState
 * Usage: EvalState state;
 * -----------------------
 * Creates a new EvalState object with no variable bindings.
 */

    EvalState();

/*
 * Destructor: ~EvalState
 * Usage: usually implicit
 * -----------------------
 * Frees all heap storage associated with this object.
 */

    ~EvalState();

/*
 * Method: setValue
 * Usage: state.setValue(var, value);
 * ----------------------------------
 * Sets the value associated with the specified var.
 */

    void setValue(std::string var, int value);

/*
 * Method: getValue
 * Usage: int value = state.getValue(var);
 * ---------------------------------------
 * Returns the value associated with the specified variable.
 */

    int getValue(std::string var);

/*
 * Method: isDefined
 * Usage: if (state.isDefined(var)) . . .
 * --------------------------------------
 * Returns true if the specified variable is defined.
 */

    bool isDefined(std::string var);

    void clear();

/*
 * Method: setCurrentLine
 * Usage: state.setCurrentLine(lineNumber);
 * -----------------------------------------
 * Sets the current line number for program execution.
 */

    void setCurrentLine(int lineNumber);

/*
 * Method: getCurrentLine
 * Usage: int line = state.getCurrentLine();
 * ------------------------------------------
 * Gets the current line number for program execution.
 */

    int getCurrentLine() const;

/*
 * Method: isJumpRequested
 * Usage: if (state.isJumpRequested()) ...
 * ---------------------------------------
 * Returns true if a jump to a specific line has been requested.
 */

    bool isJumpRequested() const;

/*
 * Method: clearJump
 * Usage: state.clearJump();
 * --------------------------
 * Clears the jump request flag.
 */

    void clearJump();

private:

    std::map<std::string, int> symbolTable;
    int currentLine;
    bool jumpRequested;
    int jumpTarget;

};

#endif
