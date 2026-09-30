#include "Calculator.h"
#include "Stack.h"

#include <cmath>
#include <cctype>
#include <stdexcept>

int Calculator::precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;

    if (op == '*' || op == '/' || op == '%')
        return 2;

    if (op == '^')
        return 3;

    return 0;
}

bool Calculator::isOperator(char c)
{
    return c == '+' || c == '-' ||
           c == '*' || c == '/' ||
           c == '%' || c == '^';
}

double Calculator::applyOperation(double a, double b, char op)
{
    switch (op)
    {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;

        case '/':
            if (b == 0)
                throw std::runtime_error("Cannot divide by zero");
            return a / b;

        case '%':
            if (b == 0)
                throw std::runtime_error("Cannot divide by zero");
            return std::fmod(a, b);

        case '^':
            return std::pow(a, b);

        default:
            throw std::runtime_error("Invalid operator");
    }
}

double Calculator::calculate(const std::string& expression)
{
    Stack<double> numbers;
    Stack<char> operators;

    for (size_t i = 0; i < expression.length();)
    {
        if (std::isspace(static_cast<unsigned char>(expression[i])))
        {
            i++;
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(expression[i])) ||
            expression[i] == '.')
        {
            std::string number;
            int dots = 0;

            while (i < expression.length() &&
                   (std::isdigit(static_cast<unsigned char>(expression[i])) ||
                    expression[i] == '.'))
            {
                if (expression[i] == '.')
                    dots++;

                if (dots > 1)
                    throw std::runtime_error("Invalid number");

                number += expression[i];
                i++;
            }

            numbers.push(std::stod(number));
        }
        else if (expression[i] == '(')
        {
            operators.push('(');
            i++;
        }
        else if (expression[i] == ')')
        {
            while (!operators.isEmpty() && operators.top() != '(')
            {
                double b = numbers.top(); numbers.pop();
                double a = numbers.top(); numbers.pop();
                char op = operators.top(); operators.pop();

                numbers.push(applyOperation(a, b, op));
            }

            if (operators.isEmpty())
                throw std::runtime_error("Mismatched parentheses");

            operators.pop();
            i++;
        }
        else if (isOperator(expression[i]))
        {
            char currentOperator = expression[i];

            while (!operators.isEmpty() &&
                   operators.top() != '(' &&
                   precedence(operators.top()) >= precedence(currentOperator))
            {
                double b = numbers.top(); numbers.pop();
                double a = numbers.top(); numbers.pop();
                char op = operators.top(); operators.pop();

                numbers.push(applyOperation(a, b, op));
            }

            operators.push(currentOperator);
            i++;
        }
        else
        {
            throw std::runtime_error("Invalid character");
        }
    }

    while (!operators.isEmpty())
    {
        if (operators.top() == '(')
            throw std::runtime_error("Mismatched parentheses");

        double b = numbers.top(); numbers.pop();
        double a = numbers.top(); numbers.pop();
        char op = operators.top(); operators.pop();

        numbers.push(applyOperation(a, b, op));
    }

    if (numbers.isEmpty())
        throw std::runtime_error("Invalid expression");

    double result = numbers.top();
    numbers.pop();

    if (!numbers.isEmpty())
        throw std::runtime_error("Invalid expression");

    return result;
}
