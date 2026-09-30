C++ GUI Calculator
==================

This is a simple desktop calculator written in C++ using the native
Windows GUI API.

Features:
- Addition
- Subtraction
- Multiplication
- Division
- Modulus
- Power
- Square (x²)
- Decimal numbers
- Clear
- Delete
- Full expression evaluation
- Operator precedence
- Stack-based expression evaluation

DSA:
The calculator uses two custom stacks:
1. Stack<double> for numbers
2. Stack<char> for operators

Example:
2 + 3 * 4

The operator stack keeps + and * and the calculator applies * first,
giving 14.

Build with MinGW g++:
g++ main.cpp Stack.cpp Calculator.cpp -o calculator.exe -mwindows

Run:
.\calculator.exe
