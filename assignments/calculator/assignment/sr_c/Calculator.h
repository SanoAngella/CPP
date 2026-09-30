#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>

class Calculator
{
public:
    double calculate(const std::string& expression);

private:
    int precedence(char op);
    double applyOperation(double a, double b, char op);
    bool isOperator(char c);
};

#endif