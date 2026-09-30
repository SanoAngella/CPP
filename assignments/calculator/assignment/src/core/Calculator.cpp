#include "Calculator.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

Calculator::Calculator()
    : lastAnswer(0.0), hasLastAnswerFlag(false) {}

std::string Calculator::formatNumber(double value) const {
    if (std::isnan(value)) return "Error: Result is NaN";
    if (std::isinf(value)) return (value > 0) ? "Error: Overflow (+Infinity)" : "Error: Overflow (-Infinity)";

    // Very close to 0
    if (std::abs(value) < 1e-12) {
        return "0";
    }

    // Check if it's effectively an integer
    double intPart;
    if (std::modf(value, &intPart) == 0.0 && std::abs(value) < 1e15) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(0) << value;
        return ss.str();
    }

    // Standard floating point output
    std::ostringstream ss;
    ss << std::setprecision(10) << value;
    std::string str = ss.str();

    // If there is a decimal point and no scientific exponent 'e', trim trailing zeros
    if (str.find('.') != std::string::npos && str.find('e') == std::string::npos && str.find('E') == std::string::npos) {
        while (!str.empty() && str.back() == '0') {
            str.pop_back();
        }
        if (!str.empty() && str.back() == '.') {
            str.pop_back();
        }
    }

    return str;
}

std::string Calculator::preprocess(const std::string& expression) const {
    std::string result = expression;

    // Replace "ANS" (case-insensitive) with lastAnswer value
    std::string ansPattern = "ANS";
    std::string ansReplacement = hasLastAnswerFlag ? formatNumber(lastAnswer) : "0";

    size_t pos = 0;
    while (true) {
        // Look for ANS or ans
        size_t foundUpper = result.find("ANS", pos);
        size_t foundLower = result.find("ans", pos);
        size_t matchPos = std::string::npos;

        if (foundUpper != std::string::npos && foundLower != std::string::npos) {
            matchPos = std::min(foundUpper, foundLower);
        } else if (foundUpper != std::string::npos) {
            matchPos = foundUpper;
        } else if (foundLower != std::string::npos) {
            matchPos = foundLower;
        }

        if (matchPos == std::string::npos) break;

        result.replace(matchPos, 3, ansReplacement);
        pos = matchPos + ansReplacement.length();
    }

    return result;
}

CalculationResult Calculator::calculate(const std::string& expression) {
    CalculationResult res;
    res.success = false;
    res.value = 0.0;

    try {
        std::string processed = preprocess(expression);
        double val = parser.evaluate(processed);

        res.success = true;
        res.value = val;
        res.formattedResult = formatNumber(val);
        res.errorMessage = "";

        // Record in state
        lastAnswer = val;
        hasLastAnswerFlag = true;

        // Record in history list
        history.add(expression, res.formattedResult);

    } catch (const std::exception& e) {
        res.success = false;
        res.value = 0.0;
        res.formattedResult = "";
        std::string msg = e.what();
        if (msg.rfind("Error:", 0) != 0) {
            res.errorMessage = "Error: " + msg;
        } else {
            res.errorMessage = msg;
        }
    }

    return res;
}

double Calculator::getLastAnswer() const {
    return lastAnswer;
}

bool Calculator::hasLastAnswer() const {
    return hasLastAnswerFlag;
}

void Calculator::setAngleMode(ExpressionParser::AngleMode mode) {
    parser.setAngleMode(mode);
}

ExpressionParser::AngleMode Calculator::getAngleMode() const {
    return parser.getAngleMode();
}

const HistoryList& Calculator::getHistory() const {
    return history;
}

void Calculator::clearHistory() {
    history.clear();
}
