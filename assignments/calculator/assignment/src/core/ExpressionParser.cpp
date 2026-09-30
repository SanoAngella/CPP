#include "ExpressionParser.h"
#include <cmath>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

ExpressionParser::ExpressionParser()
    : angleMode(AngleMode::Radians) {}

void ExpressionParser::setAngleMode(AngleMode mode) {
    angleMode = mode;
}

ExpressionParser::AngleMode ExpressionParser::getAngleMode() const {
    return angleMode;
}

std::string ExpressionParser::normalizeString(const std::string& input) const {
    std::string result;
    result.reserve(input.size());

    for (size_t i = 0; i < input.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(input[i]);

        // Replace Unicode multiplication symbol '×' (UTF-8: 0xC3 0x97 or Windows-1252: 0xD7)
        if (c == 0xC3 && i + 1 < input.size() && static_cast<unsigned char>(input[i + 1]) == 0x97) {
            result += '*';
            i++;
        } else if (c == 0xD7) {
            result += '*';
        }
        // Replace Unicode division symbol '÷' (UTF-8: 0xC3 0xB7 or Windows-1252: 0xF7)
        else if (c == 0xC3 && i + 1 < input.size() && static_cast<unsigned char>(input[i + 1]) == 0xB7) {
            result += '/';
            i++;
        } else if (c == 0xF7) {
            result += '/';
        }
        // Replace Unicode square root '√' (UTF-8: 0xE2 0x88 0x9A)
        else if (c == 0xE2 && i + 2 < input.size() &&
                 static_cast<unsigned char>(input[i + 1]) == 0x88 &&
                 static_cast<unsigned char>(input[i + 2]) == 0x9A) {
            result += "sqrt";
            i += 2;
        } else {
            result += input[i];
        }
    }
    return result;
}

bool ExpressionParser::isUnaryOperator(const std::vector<Token>& tokensSoFar) const {
    if (tokensSoFar.empty()) {
        return true;
    }
    const Token& last = tokensSoFar.back();
    return (last.type == TokenType::Operator ||
            last.type == TokenType::UnaryOp ||
            last.type == TokenType::LeftParen ||
            last.type == TokenType::Function);
}

std::vector<Token> ExpressionParser::tokenize(const std::string& rawExpression) const {
    std::string expression = normalizeString(rawExpression);
    std::vector<Token> tokens;
    size_t i = 0;
    size_t len = expression.length();

    // Skip leading spaces
    while (i < len && std::isspace(static_cast<unsigned char>(expression[i]))) {
        i++;
    }

    if (i >= len) {
        throw std::runtime_error("Empty expression");
    }

    while (i < len) {
        char c = expression[i];

        if (std::isspace(static_cast<unsigned char>(c))) {
            i++;
            continue;
        }

        // Numbers: digits or '.' starting a float like ".5"
        if (std::isdigit(static_cast<unsigned char>(c)) || (c == '.' && i + 1 < len && std::isdigit(static_cast<unsigned char>(expression[i + 1])))) {
            // Implicit multiplication check: e.g. 5(3+4) or )5
            if (!tokens.empty()) {
                const Token& prev = tokens.back();
                if (prev.type == TokenType::Number || prev.type == TokenType::RightParen) {
                    tokens.push_back(Token(TokenType::Operator, "*", 0.0, 2, Associativity::Left));
                }
            }

            size_t start = i;
            bool hasDecimal = (c == '.');
            i++;
            while (i < len) {
                char nextChar = expression[i];
                if (std::isdigit(static_cast<unsigned char>(nextChar))) {
                    i++;
                } else if (nextChar == '.') {
                    if (hasDecimal) {
                        throw std::runtime_error("Invalid number format: multiple decimal points");
                    }
                    hasDecimal = true;
                    i++;
                } else {
                    break;
                }
            }

            std::string numStr = expression.substr(start, i - start);
            double val = std::stod(numStr);
            tokens.push_back(Token(TokenType::Number, numStr, val));
            continue;
        }

        // Alphabetic identifiers: functions (sqrt, sin, cos, tan, log, ln) or constants (pi, e)
        if (std::isalpha(static_cast<unsigned char>(c))) {
            size_t start = i;
            while (i < len && std::isalpha(static_cast<unsigned char>(expression[i]))) {
                i++;
            }
            std::string id = expression.substr(start, i - start);

            // Convert to lowercase
            for (char& ch : id) {
                ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
            }

            // Check constants
            if (id == "pi") {
                if (!tokens.empty() && (tokens.back().type == TokenType::Number || tokens.back().type == TokenType::RightParen)) {
                    tokens.push_back(Token(TokenType::Operator, "*", 0.0, 2, Associativity::Left));
                }
                tokens.push_back(Token(TokenType::Number, "pi", M_PI));
            } else if (id == "e") {
                if (!tokens.empty() && (tokens.back().type == TokenType::Number || tokens.back().type == TokenType::RightParen)) {
                    tokens.push_back(Token(TokenType::Operator, "*", 0.0, 2, Associativity::Left));
                }
                tokens.push_back(Token(TokenType::Number, "e", M_E));
            } else if (id == "sqrt" || id == "sin" || id == "cos" || id == "tan" || id == "log" || id == "ln") {
                if (!tokens.empty() && (tokens.back().type == TokenType::Number || tokens.back().type == TokenType::RightParen)) {
                    tokens.push_back(Token(TokenType::Operator, "*", 0.0, 2, Associativity::Left));
                }
                // Functions have highest precedence (5) and Right associativity
                tokens.push_back(Token(TokenType::Function, id, 0.0, 5, Associativity::Right));
            } else {
                throw std::runtime_error("Unknown function or identifier: '" + id + "'");
            }
            continue;
        }

        // Parentheses
        if (c == '(') {
            // Implicit multiplication: 2(5) -> 2*(5) or )( -> )*(
            if (!tokens.empty()) {
                const Token& prev = tokens.back();
                if (prev.type == TokenType::Number || prev.type == TokenType::RightParen) {
                    tokens.push_back(Token(TokenType::Operator, "*", 0.0, 2, Associativity::Left));
                }
            }
            tokens.push_back(Token(TokenType::LeftParen, "("));
            i++;
            continue;
        }

        if (c == ')') {
            if (tokens.empty() || tokens.back().type == TokenType::LeftParen) {
                throw std::runtime_error("Empty parentheses ()");
            }
            tokens.push_back(Token(TokenType::RightParen, ")"));
            i++;
            continue;
        }

        // Operators: +, -, *, /, %, ^
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^') {
            std::string op(1, c);

            // Handle unary signs (+ or -)
            if (c == '-' && isUnaryOperator(tokens)) {
                // Unary negation NEG
                tokens.push_back(Token(TokenType::UnaryOp, "NEG", 0.0, 4, Associativity::Right));
                i++;
                continue;
            } else if (c == '+' && isUnaryOperator(tokens)) {
                // Discard unary plus
                i++;
                continue;
            }

            // Consecutive binary operators check (e.g. 5 + * 4 or 2 ** 3)
            if (!tokens.empty() && tokens.back().type == TokenType::Operator) {
                throw std::runtime_error("Invalid syntax: consecutive operators '" + tokens.back().text + "' and '" + op + "'");
            }

            if (tokens.empty() || tokens.back().type == TokenType::LeftParen || tokens.back().type == TokenType::UnaryOp) {
                throw std::runtime_error("Misplaced operator: '" + op + "'");
            }

            int prec = 1;
            Associativity assoc = Associativity::Left;
            if (c == '+' || c == '-') {
                prec = 1;
                assoc = Associativity::Left;
            } else if (c == '*' || c == '/' || c == '%') {
                prec = 2;
                assoc = Associativity::Left;
            } else if (c == '^') {
                prec = 3;
                assoc = Associativity::Right;
            }

            tokens.push_back(Token(TokenType::Operator, op, 0.0, prec, assoc));
            i++;
            continue;
        }

        // If unrecognized character
        throw std::runtime_error(std::string("Unrecognized character in expression: '") + c + "'");
    }

    // Post-tokenization validation: expression cannot end with an operator or unary negation
    if (!tokens.empty()) {
        const Token& last = tokens.back();
        if (last.type == TokenType::Operator || last.type == TokenType::UnaryOp) {
            throw std::runtime_error("Incomplete expression: ends with operator '" + last.text + "'");
        }
        if (last.type == TokenType::Function) {
            throw std::runtime_error("Incomplete expression: missing argument for function '" + last.text + "'");
        }
    }

    return tokens;
}

std::vector<Token> ExpressionParser::infixToPostfix(const std::vector<Token>& infixTokens) const {
    std::vector<Token> postfix;
    Stack<Token> opStack(32);

    for (size_t i = 0; i < infixTokens.size(); ++i) {
        const Token& token = infixTokens[i];

        switch (token.type) {
            case TokenType::Number:
                postfix.push_back(token);
                break;

            case TokenType::Function:
                opStack.push(token);
                break;

            case TokenType::UnaryOp:
            case TokenType::Operator: {
                // Pop operators from stack with higher precedence, or same precedence if left-associative
                while (!opStack.isEmpty() && opStack.top().type != TokenType::LeftParen) {
                    const Token& topOp = opStack.top();
                    bool shouldPop = false;

                    if (topOp.precedence > token.precedence) {
                        shouldPop = true;
                    } else if (topOp.precedence == token.precedence && token.associativity == Associativity::Left) {
                        shouldPop = true;
                    }

                    if (shouldPop) {
                        postfix.push_back(topOp);
                        opStack.pop();
                    } else {
                        break;
                    }
                }
                opStack.push(token);
                break;
            }

            case TokenType::LeftParen:
                opStack.push(token);
                break;

            case TokenType::RightParen: {
                bool foundMatchingLeft = false;
                while (!opStack.isEmpty()) {
                    if (opStack.top().type == TokenType::LeftParen) {
                        foundMatchingLeft = true;
                        opStack.pop(); // Remove '('
                        break;
                    }
                    postfix.push_back(opStack.top());
                    opStack.pop();
                }

                if (!foundMatchingLeft) {
                    throw std::runtime_error("Mismatched parentheses: unexpected ')'");
                }

                // If a function call was before '(', pop it to postfix
                if (!opStack.isEmpty() && opStack.top().type == TokenType::Function) {
                    postfix.push_back(opStack.top());
                    opStack.pop();
                }
                break;
            }

            default:
                break;
        }
    }

    // Pop any remaining operators from the stack
    while (!opStack.isEmpty()) {
        if (opStack.top().type == TokenType::LeftParen || opStack.top().type == TokenType::RightParen) {
            throw std::runtime_error("Mismatched parentheses: unclosed '('");
        }
        postfix.push_back(opStack.top());
        opStack.pop();
    }

    return postfix;
}

double ExpressionParser::applyBinaryOperator(const std::string& op, double a, double b) const {
    if (op == "+") {
        return a + b;
    } else if (op == "-") {
        return a - b;
    } else if (op == "*") {
        return a * b;
    } else if (op == "/") {
        if (std::abs(b) < 1e-15) {
            throw std::runtime_error("Cannot divide by zero");
        }
        return a / b;
    } else if (op == "%") {
        if (std::abs(b) < 1e-15) {
            throw std::runtime_error("Modulo by zero");
        }
        return std::fmod(a, b);
    } else if (op == "^") {
        if (a == 0.0 && b < 0.0) {
            throw std::runtime_error("Zero cannot be raised to a negative power");
        }
        if (a < 0.0 && std::floor(b) != b) {
            throw std::runtime_error("Negative base cannot be raised to non-integer power");
        }
        return std::pow(a, b);
    }
    throw std::runtime_error("Unknown operator: '" + op + "'");
}

double ExpressionParser::applyFunction(const std::string& func, double arg) const {
    if (func == "sqrt") {
        if (arg < 0.0) {
            throw std::runtime_error("Negative square root is undefined for real numbers");
        }
        return std::sqrt(arg);
    } else if (func == "sin") {
        double rad = (angleMode == AngleMode::Degrees) ? (arg * M_PI / 180.0) : arg;
        return std::sin(rad);
    } else if (func == "cos") {
        double rad = (angleMode == AngleMode::Degrees) ? (arg * M_PI / 180.0) : arg;
        return std::cos(rad);
    } else if (func == "tan") {
        double rad = (angleMode == AngleMode::Degrees) ? (arg * M_PI / 180.0) : arg;
        if (std::abs(std::cos(rad)) < 1e-15) {
            throw std::runtime_error("Tangent is undefined (asymptote)");
        }
        return std::tan(rad);
    } else if (func == "log") {
        if (arg <= 0.0) {
            throw std::runtime_error("Logarithm argument must be strictly positive");
        }
        return std::log10(arg);
    } else if (func == "ln") {
        if (arg <= 0.0) {
            throw std::runtime_error("Natural log argument must be strictly positive");
        }
        return std::log(arg);
    }
    throw std::runtime_error("Unknown function: '" + func + "'");
}

double ExpressionParser::evaluatePostfix(const std::vector<Token>& postfixTokens) const {
    if (postfixTokens.empty()) {
        throw std::runtime_error("Empty postfix expression");
    }

    Stack<double> valStack(32);

    for (size_t i = 0; i < postfixTokens.size(); ++i) {
        const Token& token = postfixTokens[i];

        if (token.type == TokenType::Number) {
            valStack.push(token.numberValue);
        } else if (token.type == TokenType::UnaryOp) {
            if (valStack.isEmpty()) {
                throw std::runtime_error("Malformed expression: missing operand for unary negation");
            }
            double val = valStack.top();
            valStack.pop();
            valStack.push(-val);
        } else if (token.type == TokenType::Function) {
            if (valStack.isEmpty()) {
                throw std::runtime_error("Malformed expression: missing argument for function '" + token.text + "'");
            }
            double arg = valStack.top();
            valStack.pop();
            double res = applyFunction(token.text, arg);
            valStack.push(res);
        } else if (token.type == TokenType::Operator) {
            if (valStack.size() < 2) {
                throw std::runtime_error("Malformed expression: insufficient operands for operator '" + token.text + "'");
            }
            double b = valStack.top();
            valStack.pop();
            double a = valStack.top();
            valStack.pop();

            double res = applyBinaryOperator(token.text, a, b);
            valStack.push(res);
        }
    }

    if (valStack.size() != 1) {
        throw std::runtime_error("Malformed expression: extra unconsumed operands");
    }

    return valStack.top();
}

double ExpressionParser::evaluate(const std::string& expression) const {
    std::vector<Token> tokens = tokenize(expression);
    std::vector<Token> postfix = infixToPostfix(tokens);
    return evaluatePostfix(postfix);
}
