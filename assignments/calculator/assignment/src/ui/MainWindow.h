#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include "../core/Calculator.h"

/**
 * @brief Represents metadata and layout for a calculator button.
 */
enum class ButtonCategory {
    Digit,
    Operator,
    Function,
    ActionClear,
    ActionEquals,
    Utility
};

struct CalcButton {
    int id;
    std::string label;
    std::string insertText;
    ButtonCategory category;
    int row;
    int col;
    int colSpan;
    HWND hwnd;
};

/**
 * @brief Main GUI Window for the C++ DSA Calculator.
 * 
 * Uses Native Windows Win32 API with Owner-Drawn controls to achieve a sleek,
 * modern, dark-themed user interface with Segoe UI typography and zero external dependencies.
 */
class MainWindow {
private:
    HINSTANCE hInstance;
    HWND hwndMain;
    HWND hwndDisplayExpr;
    HWND hwndDisplayResult;
    HFONT hFontExpr;
    HFONT hFontResult;
    HFONT hFontButton;
    HFONT hFontSmall;

    // Brushes and colors for dark theme
    HBRUSH hBrushBg;
    HBRUSH hBrushDisplay;
    HBRUSH hBrushBtnDigit;
    HBRUSH hBrushBtnOp;
    HBRUSH hBrushBtnFunc;
    HBRUSH hBrushBtnClear;
    HBRUSH hBrushBtnEquals;
    HBRUSH hBrushBtnUtil;

    // Calculator engine & UI state
    Calculator calculator;
    std::string currentExpression;
    std::string currentResultText;
    bool isResultEvaluated;
    bool isErrorState;

    std::vector<CalcButton> buttons;

    // Static window procedure dispatcher
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK HistoryDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void initializeButtons();
    void createControls(HWND hwnd);
    void layoutControls(int width, int height);
    void drawCustomButton(LPDRAWITEMSTRUCT pDIS);
    void updateDisplay();

    // Event handlers
    void onButtonClick(int buttonId);
    void onCalculate();
    void onClear(bool allClear);
    void onDelete();
    void onToggleSign();
    void onShowHistory();
    void onToggleAngleMode();
    void appendToExpression(const std::string& text);

public:
    MainWindow();
    ~MainWindow();

    bool registerAndCreate(HINSTANCE hInst, int nCmdShow);
    int runMessageLoop();
};

#endif // MAIN_WINDOW_H
