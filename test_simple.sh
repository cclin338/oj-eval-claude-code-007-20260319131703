#!/bin/bash

echo "Running basic tests..."

# Test 1: QUIT
echo "Test 1: QUIT"
echo "Expected output: nothing"
echo -n "Actual output: "
echo "QUIT" | ./code
echo ""

# Test 2: PRINT
echo "Test 2: PRINT 42"
echo "Expected output: 42"
echo -n "Actual output: "
echo -e "PRINT 42\nQUIT" | ./code
echo ""

# Test 3: Program with LIST
echo "Test 3: LIST after adding program"
echo "Expected output: program listing"
echo -n "Actual output: "
echo -e "10 PRINT 123\nLIST\nQUIT" | ./code
echo ""

# Test 4: RUN program
echo "Test 4: RUN a simple program"
echo "Expected output: 5 then 10"
echo -n "Actual output: "
echo -e "10 LET X = 5\n20 PRINT X\n30 LET X = 10\n40 PRINT X\n50 END\nRUN" | ./code
echo ""

# Test 5: GOTO
echo "Test 5: GOTO statement"
echo "Expected output: 1 2 3"
echo -n "Actual output: "
echo -e "10 PRINT 1\n20 GOTO 40\n30 PRINT 2\n40 PRINT 3\n50 END\nRUN" | ./code