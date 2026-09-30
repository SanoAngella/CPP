#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include <string>
#include <vector>
#include "Token.h"
#include "../dsa/Stack.h"

/**
 * @brief ExpressionParser handles tokenization, Shunting-Yard parsing,
 * and Postfix evaluation of mathematical expressions using custom Stacks.
 * 
 * DSA Core Concepts:
 * 1. Tokenization: Lexical scanning converting raw string characters into structured Tokens.
 * 2. Shunting-Yard Algorithm: Converts Infix notation (human-readable, e.g. 2 + 3 * 4)
 *    into Postfix notation (Reverse Polish Notation, e.g. 2 3 4 * +) using an Operator Stack.
 * 3. Postfix Evaluation: Computes the numerical value from Postfix tokens using an Operand Stack.
 */
class ExpressionParser {
public:
    enum class AngleMode {
        Radians,
        Degrees
    };

private:
    AngleMode angleMode;

    /**
     * @brief Normalizes input string (replaces unicode symbols like ×, ÷, √ with standard tokens).
     */
    std::string normalizeString(const std::string& input) const;

    /**
     * @brief Determines whether a '-' or '+' character at the given position is unary.
     */
    bool isUnaryOperator(const std::vector<Token>& tokensSoFar) const;

    /**
     * @brief Applies a binary operator (e.g. +, -, *, /, %, ^) to operands a and b.
     */
    double applyBinaryOperator(const std::string& op, double a, double b) const;

    /**
     * @brief Applies a unary operator or scientific function (e.g. NEG, sqrt, sin, cos).
     */
    double applyFunction(const std::string& func, double arg) const;

public:
    ExpressionParser();

    void setAngleMode(AngleMode mode);
    AngleMode getAngleMode() const;

    /**
     * @brief Tokenizes an expression string into a sequence of Tokens.
     * @throws std::runtime_error on invalid characters, consecutive operators, etc.
     */
    std::vector<Token> tokenize(const std::string& expression) const;

    /**
     * @brief Converts an Infix token stream to Postfix (RPN) using Dijkstra's Shunting-Yard
     * algorithm with our custom Stack<Token>.
     * @throws std::runtime_error on mismatched parentheses.
     */
    std::vector<Token> infixToPostfix(const std::vector<Token>& infixTokens) const;

    /**
     * @brief Evaluates a sequence of Postfix (RPN) tokens using our custom Stack<double>.
     * @throws std::runtime_error on division by zero, domain errors, malformed syntax.
     */
    double evaluatePostfix(const std::vector<Token>& postfixTokens) const;

    /**
     * @brief Complete end-to-end evaluation pipeline:
     * Tokenize -> InfixToPostfix -> EvaluatePostfix.
     */
    double evaluate(const std::string& expression) const;
};

#endif // EXPRESSION_PARSER_H
