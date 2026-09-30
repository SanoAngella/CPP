#ifndef TOKEN_H
#define TOKEN_H

#include <string>

/**
 * @brief Categorization of lexical tokens in a mathematical expression.
 */
enum class TokenType {
    Number,
    Operator,
    UnaryOp,
    Function,
    LeftParen,
    RightParen,
    End
};

/**
 * @brief Operator associativity direction for Shunting-Yard algorithm.
 */
enum class Associativity {
    Left,
    Right,
    None
};

/**
 * @brief Represents a single token in an expression.
 */
struct Token {
    TokenType type;
    std::string text;           ///< String representation (e.g., "+", "sqrt", "3.14")
    double numberValue;         ///< Numerical value if type == TokenType::Number
    int precedence;             ///< Precedence level (higher number = higher binding)
    Associativity associativity;///< Left or Right associative

    Token()
        : type(TokenType::End), text(""), numberValue(0.0), precedence(0), associativity(Associativity::None) {}

    Token(TokenType t, const std::string& txt, double val = 0.0, int prec = 0, Associativity assoc = Associativity::None)
        : type(t), text(txt), numberValue(val), precedence(prec), associativity(assoc) {}
};

#endif // TOKEN_H
