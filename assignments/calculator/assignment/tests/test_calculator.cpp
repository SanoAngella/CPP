#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include "../src/dsa/Stack.h"
#include "../src/dsa/HistoryList.h"
#include "../src/core/Calculator.h"
#include "../src/core/ExpressionParser.h"

static int totalTests = 0;
static int passedTests = 0;
static int failedTests = 0;

void assertEqual(const std::string& testName, double actual, double expected, double epsilon = 1e-6) {
    totalTests++;
    if (std::abs(actual - expected) <= epsilon) {
        std::cout << "  [PASS] " << testName << " -> Expected: " << expected << ", Got: " << actual << "\n";
        passedTests++;
    } else {
        std::cout << "  [FAIL] " << testName << " -> Expected: " << expected << ", Got: " << actual << "\n";
        failedTests++;
    }
}

void assertStringEqual(const std::string& testName, const std::string& actual, const std::string& expected) {
    totalTests++;
    if (actual == expected) {
        std::cout << "  [PASS] " << testName << " -> \"" << actual << "\"\n";
        passedTests++;
    } else {
        std::cout << "  [FAIL] " << testName << " -> Expected: \"" << expected << "\", Got: \"" << actual << "\"\n";
        failedTests++;
    }
}

void assertError(const std::string& testName, const std::string& expression, Calculator& calc) {
    totalTests++;
    CalculationResult res = calc.calculate(expression);
    if (!res.success) {
        std::cout << "  [PASS] Error Caught: \"" << expression << "\" -> " << res.errorMessage << "\n";
        passedTests++;
    } else {
        std::cout << "  [FAIL] Expected error for \"" << expression << "\", but got success: " << res.formattedResult << "\n";
        failedTests++;
    }
}

void testStackDSA() {
    std::cout << "\n========================================\n";
    std::cout << "  RUNNING DSA STACK UNIT TESTS\n";
    std::cout << "========================================\n";

    Stack<int> s(4);
    totalTests++;
    if (s.isEmpty() && s.size() == 0) {
        std::cout << "  [PASS] Stack initial state is empty\n";
        passedTests++;
    } else {
        std::cout << "  [FAIL] Stack initial state is not empty\n";
        failedTests++;
    }

    s.push(10);
    s.push(20);
    s.push(30);
    s.push(40);

    totalTests++;
    if (s.isFull() && s.size() == 4 && s.top() == 30 + 10) {
        std::cout << "  [PASS] Stack push and top correct, reached initial capacity (4)\n";
        passedTests++;
    } else {
        std::cout << "  [FAIL] Stack push/isFull failed\n";
        failedTests++;
    }

    // Test dynamic resizing: 5th push should double capacity
    s.push(50);
    totalTests++;
    if (s.size() == 5 && s.getCapacity() == 8 && s.top() == 50) {
        std::cout << "  [PASS] Stack dynamic resize auto-expanded capacity from 4 to 8\n";
        passedTests++;
    } else {
        std::cout << "  [FAIL] Dynamic resize failed\n";
        failedTests++;
    }

    // Test LIFO order
    int pop1 = s.top(); s.pop();
    int pop2 = s.top(); s.pop();
    totalTests++;
    if (pop1 == 50 && pop2 == 40 && s.size() == 3) {
        std::cout << "  [PASS] Stack LIFO popping order validated (50, then 40)\n";
        passedTests++;
    } else {
        std::cout << "  [FAIL] LIFO popping order failed\n";
        failedTests++;
    }

    // Test copy constructor (Rule of Three deep copy)
    Stack<int> copyStack = s;
    copyStack.push(999);
    totalTests++;
    if (s.top() == 30 && copyStack.top() == 999 && copyStack.size() == 4 && s.size() == 3) {
        std::cout << "  [PASS] Deep copy constructor test verified independent memory\n";
        passedTests++;
    } else {
        std::cout << "  [FAIL] Deep copy constructor failed\n";
        failedTests++;
    }

    // Test underflow exception
    s.clear();
    bool underflowCaught = false;
    try {
        s.pop();
    } catch (const std::underflow_error&) {
        underflowCaught = true;
    }
    totalTests++;
    if (underflowCaught) {
        std::cout << "  [PASS] Underflow exception properly thrown on pop() from empty stack\n";
        passedTests++;
    } else {
        std::cout << "  [FAIL] Underflow exception not thrown\n";
        failedTests++;
    }
}

void testHistoryDSA() {
    std::cout << "\n========================================\n";
    std::cout << "  RUNNING DSA HISTORY LIST TESTS\n";
    std::cout << "========================================\n";

    HistoryList hist(5);
    hist.add("2 + 3", "5");
    hist.add("10 * 4", "40");
    hist.add("2 ^ 3", "8");

    totalTests++;
    if (hist.size() == 3) {
        std::cout << "  [PASS] History list size is 3\n";
        passedTests++;
    } else {
        std::cout << "  [FAIL] History list size incorrect\n";
        failedTests++;
    }

    std::vector<std::string> entries = hist.getFormattedEntries();
    totalTests++;
    // Newest entry should be first in list
    if (entries.size() == 3 && entries[0] == "2 ^ 3 = 8" && entries[2] == "2 + 3 = 5") {
        std::cout << "  [PASS] History ordered newest-to-oldest verified\n";
        passedTests++;
    } else {
        std::cout << "  [FAIL] History order incorrect\n";
        failedTests++;
    }

    hist.clear();
    totalTests++;
    if (hist.isEmpty() && hist.size() == 0) {
        std::cout << "  [PASS] History clear() successfully emptied list\n";
        passedTests++;
    } else {
        std::cout << "  [FAIL] History clear() failed\n";
        failedTests++;
    }
}

void testArithmeticCalculations() {
    std::cout << "\n========================================\n";
    std::cout << "  RUNNING ARITHMETIC & PRECEDENCE TESTS\n";
    std::cout << "========================================\n";

    Calculator calc;

    // Basic arithmetic
    assertEqual("2 + 3", calc.calculate("2 + 3").value, 5.0);
    assertEqual("10 - 4", calc.calculate("10 - 4").value, 6.0);
    assertEqual("5 * 6", calc.calculate("5 * 6").value, 30.0);
    assertEqual("20 / 4", calc.calculate("20 / 4").value, 5.0);
    assertEqual("10 % 3", calc.calculate("10 % 3").value, 1.0);
    assertEqual("2 ^ 3", calc.calculate("2 ^ 3").value, 8.0);

    // Precedence and parentheses
    assertEqual("2 + 3 * 4", calc.calculate("2 + 3 * 4").value, 14.0);
    assertEqual("(2 + 3) * 4", calc.calculate("(2 + 3) * 4").value, 20.0);
    assertEqual("10 + 2 * 3 - 4", calc.calculate("10 + 2 * 3 - 4").value, 12.0);
    assertEqual("2 * (3 + 4)", calc.calculate("2 * (3 + 4)").value, 14.0);
    assertEqual("10 + 5 * 2", calc.calculate("10 + 5 * 2").value, 20.0);
    assertEqual("(10 + 5) * 2", calc.calculate("(10 + 5) * 2").value, 30.0);
    assertEqual("12 + 5 * 3 - 8 / 2", calc.calculate("12 + 5 * 3 - 8 / 2").value, 23.0);
    assertEqual("2 * (5 + 3)", calc.calculate("2 * (5 + 3)").value, 16.0);
    assertEqual("2 ^ 3 + 4 * 5", calc.calculate("2 ^ 3 + 4 * 5").value, 28.0);

    // Negative numbers and decimals
    assertEqual("-5 + 3", calc.calculate("-5 + 3").value, -2.0);
    assertEqual("3.5 * 2", calc.calculate("3.5 * 2").value, 7.0);
    assertEqual("3.14 * 2", calc.calculate("3.14 * 2").value, 6.28);
    assertEqual("2 * -4", calc.calculate("2 * -4").value, -8.0);
    assertEqual("-2 ^ 2", calc.calculate("(-2) ^ 2").value, 4.0);

    // Power right-associativity: 2 ^ 3 ^ 2 should be 2 ^ (3 ^ 2) = 2 ^ 9 = 512
    assertEqual("2 ^ 3 ^ 2 (Right Associativity)", calc.calculate("2 ^ 3 ^ 2").value, 512.0);

    // Functions
    assertEqual("sqrt(16)", calc.calculate("sqrt(16)").value, 4.0);
    assertEqual("sqrt(9) + 5", calc.calculate("sqrt(9) + 5").value, 8.0);
    assertEqual("sin(0)", calc.calculate("sin(0)").value, 0.0);
    assertEqual("cos(0)", calc.calculate("cos(0)").value, 1.0);
    assertEqual("log(100)", calc.calculate("log(100)").value, 2.0);

    // Formatting check
    assertStringEqual("Formatted 14.0 -> \"14\"", calc.calculate("2 + 3 * 4").formattedResult, "14");
    assertStringEqual("Formatted 3.14 * 2 -> \"6.28\"", calc.calculate("3.14 * 2").formattedResult, "6.28");

    // ANS reuse check
    calc.calculate("5 + 5"); // 10
    assertEqual("ANS * 3", calc.calculate("ANS * 3").value, 30.0);
}

void testErrorHandling() {
    std::cout << "\n========================================\n";
    std::cout << "  RUNNING ERROR HANDLING TESTS\n";
    std::cout << "========================================\n";

    Calculator calc;

    assertError("Division by Zero", "10 / 0", calc);
    assertError("Modulo by Zero", "10 % 0", calc);
    assertError("Incomplete expression (operator at end)", "5 +", calc);
    assertError("Mismatched unclosed parenthesis", "(2 + 3", calc);
    assertError("Mismatched unexpected parenthesis", "2 + 3)", calc);
    assertError("Consecutive operators", "2 ** 3", calc);
    assertError("Consecutive operators 5 + * 4", "5 + * 4", calc);
    assertError("Empty expression", "", calc);
    assertError("Only whitespace", "    ", calc);
    assertError("Negative square root", "sqrt(-4)", calc);
    assertError("Negative logarithm", "log(-10)", calc);
    assertError("Zero logarithm", "ln(0)", calc);
    assertError("Multiple decimal points", "3.14.15 + 2", calc);
}

int main() {
    std::cout << "====================================================\n";
    std::cout << "   DSA CALCULATOR TEST SUITE EXECUTION\n";
    std::cout << "====================================================\n";

    testStackDSA();
    testHistoryDSA();
    testArithmeticCalculations();
    testErrorHandling();

    std::cout << "\n====================================================\n";
    std::cout << "   TEST EXECUTION SUMMARY\n";
    std::cout << "====================================================\n";
    std::cout << "  Total Tests Executed : " << totalTests << "\n";
    std::cout << "  Passed               : " << passedTests << "\n";
    std::cout << "  Failed               : " << failedTests << "\n";

    if (failedTests == 0) {
        std::cout << "\n  >>> ALL TESTS PASSED SUCCESSFULLY! <<<\n\n";
        return 0;
    } else {
        std::cout << "\n  >>> SOME TESTS FAILED! <<<\n\n";
        return 1;
    }
}
