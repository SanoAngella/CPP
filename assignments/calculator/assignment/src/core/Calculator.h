#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <vector>
#include "ExpressionParser.h"
#include "../dsa/HistoryList.h"

/**
 * @brief Encapsulates the complete result of a calculation attempt.
 */
struct CalculationResult {
    bool success;                   ///< true if evaluation succeeded without error
    double value;                   ///< Numerical answer
    std::string formattedResult;    ///< Clean user-facing string (e.g. "14", "3.14")
    std::string errorMessage;       ///< Descriptive error if success == false
};

/**
 * @brief High-level facade and state coordinator for the calculator engine.
 * 
 * Separates UI concerns from calculation logic. The UI only talks to Calculator.
 */
class Calculator {
private:
    ExpressionParser parser;
    HistoryList history;
    double lastAnswer;
    bool hasLastAnswerFlag;

    /**
     * @brief Formats a floating point number to a clean display string
     * (removes trailing decimal zeroes, handles integer values cleanly).
     */
    std::string formatNumber(double value) const;

    /**
     * @brief Preprocesses the expression before parsing (e.g., replaces "ANS" with the last answer).
     */
    std::string preprocess(const std::string& expression) const;

public:
    Calculator();

    /**
     * @brief Evaluates an expression string, records history on success,
     * and returns structured result or error message.
     */
    CalculationResult calculate(const std::string& expression);

    /**
     * @brief Returns the previous calculation answer.
     */
    double getLastAnswer() const;

    /**
     * @brief Returns true if at least one successful calculation has occurred.
     */
    bool hasLastAnswer() const;

    /**
     * @brief Configures angle measurement mode for trigonometric functions.
     */
    void setAngleMode(ExpressionParser::AngleMode mode);
    ExpressionParser::AngleMode getAngleMode() const;

    /**
     * @brief Access the calculation history list.
     */
    const HistoryList& getHistory() const;

    /**
     * @brief Clears all calculation history entries.
     */
    void clearHistory();
};

#endif // CALCULATOR_H
