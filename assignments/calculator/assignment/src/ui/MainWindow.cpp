#include "MainWindow.h"
#include <windowsx.h>
#include <sstream>

// Control IDs
enum ControlIds {
    ID_DISPLAY_EXPR = 1001,
    ID_DISPLAY_RESULT,

    // Button IDs
    ID_BTN_DEG_RAD = 1100,
    ID_BTN_HIST,
    ID_BTN_SIN,
    ID_BTN_COS,
    ID_BTN_TAN,
    ID_BTN_SQRT,

    ID_BTN_AC,
    ID_BTN_C,
    ID_BTN_DEL,
    ID_BTN_LPAREN,
    ID_BTN_RPAREN,
    ID_BTN_POW,

    ID_BTN_7,
    ID_BTN_8,
    ID_BTN_9,
    ID_BTN_DIV,
    ID_BTN_MOD,
    ID_BTN_LOG,

    ID_BTN_4,
    ID_BTN_5,
    ID_BTN_6,
    ID_BTN_MUL,
    ID_BTN_ANS,
    ID_BTN_LN,

    ID_BTN_1,
    ID_BTN_2,
    ID_BTN_3,
    ID_BTN_SUB,
    ID_BTN_SIGN,
    ID_BTN_PI,

    ID_BTN_0,
    ID_BTN_DOT,
    ID_BTN_E,
    ID_BTN_ADD,
    ID_BTN_EQUALS,

    // History dialog controls
    ID_HIST_LIST = 2001,
    ID_HIST_RECALL,
    ID_HIST_CLEAR,
    ID_HIST_CLOSE
};

// Color Palette
static const COLORREF COLOR_BG          = RGB(32, 33, 36);    // #202124 (Dark gray background)
static const COLORREF COLOR_DISPLAY_BG  = RGB(45, 47, 52);    // #2d2f34 (Display card background)
static const COLORREF COLOR_TEXT_EXPR   = RGB(180, 186, 194); // #b4bac2 (Muted expression text)
static const COLORREF COLOR_TEXT_RES    = RGB(255, 255, 255); // White result
static const COLORREF COLOR_TEXT_ERR    = RGB(242, 139, 130); // Soft red for errors

static const COLORREF COLOR_BTN_DIGIT   = RGB(60, 64, 67);    // Digit button
static const COLORREF COLOR_BTN_OP      = RGB(80, 85, 92);    // Operator button
static const COLORREF COLOR_BTN_FUNC    = RGB(48, 51, 56);    // Function button
static const COLORREF COLOR_BTN_CLEAR   = RGB(179, 38, 30);   // Clear / Del (Red)
static const COLORREF COLOR_BTN_EQUALS  = RGB(26, 115, 232);  // Equals (Google Blue)
static const COLORREF COLOR_BTN_UTIL    = RGB(55, 60, 68);    // Utility / Ans / Hist
static const COLORREF COLOR_BTN_PRESSED = RGB(100, 105, 115); // Pressed feedback

MainWindow::MainWindow()
    : hInstance(nullptr), hwndMain(nullptr), hwndDisplayExpr(nullptr),
      hwndDisplayResult(nullptr), isResultEvaluated(false), isErrorState(false) {

    currentExpression = "";
    currentResultText = "0";

    // Create GDI Brushes
    hBrushBg        = CreateSolidBrush(COLOR_BG);
    hBrushDisplay   = CreateSolidBrush(COLOR_DISPLAY_BG);
    hBrushBtnDigit  = CreateSolidBrush(COLOR_BTN_DIGIT);
    hBrushBtnOp     = CreateSolidBrush(COLOR_BTN_OP);
    hBrushBtnFunc   = CreateSolidBrush(COLOR_BTN_FUNC);
    hBrushBtnClear  = CreateSolidBrush(COLOR_BTN_CLEAR);
    hBrushBtnEquals = CreateSolidBrush(COLOR_BTN_EQUALS);
    hBrushBtnUtil   = CreateSolidBrush(COLOR_BTN_UTIL);

    // Create Typography Fonts (Segoe UI or default sans-serif)
    hFontExpr   = CreateFontA(-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hFontResult = CreateFontA(-28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hFontButton = CreateFontA(-18, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hFontSmall  = CreateFontA(-13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");

    initializeButtons();
}

MainWindow::~MainWindow() {
    DeleteObject(hBrushBg);
    DeleteObject(hBrushDisplay);
    DeleteObject(hBrushBtnDigit);
    DeleteObject(hBrushBtnOp);
    DeleteObject(hBrushBtnFunc);
    DeleteObject(hBrushBtnClear);
    DeleteObject(hBrushBtnEquals);
    DeleteObject(hBrushBtnUtil);

    DeleteObject(hFontExpr);
    DeleteObject(hFontResult);
    DeleteObject(hFontButton);
    DeleteObject(hFontSmall);
}

void MainWindow::initializeButtons() {
    buttons = {
        // Row 0: Scientific & Utilities
        { ID_BTN_DEG_RAD, "RAD", "",       ButtonCategory::Utility,     0, 0, 1, nullptr },
        { ID_BTN_HIST,    "HIST", "",      ButtonCategory::Utility,     0, 1, 1, nullptr },
        { ID_BTN_SIN,     "sin",  "sin(",  ButtonCategory::Function,    0, 2, 1, nullptr },
        { ID_BTN_COS,     "cos",  "cos(",  ButtonCategory::Function,    0, 3, 1, nullptr },
        { ID_BTN_TAN,     "tan",  "tan(",  ButtonCategory::Function,    0, 4, 1, nullptr },
        { ID_BTN_SQRT,    "√",    "sqrt(", ButtonCategory::Function,    0, 5, 1, nullptr },

        // Row 1: Clear & Parentheses & Powers
        { ID_BTN_AC,      "AC",   "",      ButtonCategory::ActionClear, 1, 0, 1, nullptr },
        { ID_BTN_C,       "C",    "",      ButtonCategory::ActionClear, 1, 1, 1, nullptr },
        { ID_BTN_DEL,     "DEL",  "",      ButtonCategory::ActionClear, 1, 2, 1, nullptr },
        { ID_BTN_LPAREN,  "(",    "(",     ButtonCategory::Function,    1, 3, 1, nullptr },
        { ID_BTN_RPAREN,  ")",    ")",     ButtonCategory::Function,    1, 4, 1, nullptr },
        { ID_BTN_POW,     "^",    "^",     ButtonCategory::Operator,    1, 5, 1, nullptr },

        // Row 2: 7, 8, 9, ÷, %, log
        { ID_BTN_7,       "7",    "7",     ButtonCategory::Digit,       2, 0, 1, nullptr },
        { ID_BTN_8,       "8",    "8",     ButtonCategory::Digit,       2, 1, 1, nullptr },
        { ID_BTN_9,       "9",    "9",     ButtonCategory::Digit,       2, 2, 1, nullptr },
        { ID_BTN_DIV,     "÷",    " / ",   ButtonCategory::Operator,    2, 3, 1, nullptr },
        { ID_BTN_MOD,     "%",    " % ",   ButtonCategory::Operator,    2, 4, 1, nullptr },
        { ID_BTN_LOG,     "log",  "log(",  ButtonCategory::Function,    2, 5, 1, nullptr },

        // Row 3: 4, 5, 6, ×, ANS, ln
        { ID_BTN_4,       "4",    "4",     ButtonCategory::Digit,       3, 0, 1, nullptr },
        { ID_BTN_5,       "5",    "5",     ButtonCategory::Digit,       3, 1, 1, nullptr },
        { ID_BTN_6,       "6",    "6",     ButtonCategory::Digit,       3, 2, 1, nullptr },
        { ID_BTN_MUL,     "×",    " * ",   ButtonCategory::Operator,    3, 3, 1, nullptr },
        { ID_BTN_ANS,     "ANS",  "ANS",   ButtonCategory::Utility,     3, 4, 1, nullptr },
        { ID_BTN_LN,      "ln",   "ln(",   ButtonCategory::Function,    3, 5, 1, nullptr },

        // Row 4: 1, 2, 3, -, +/-, pi
        { ID_BTN_1,       "1",    "1",     ButtonCategory::Digit,       4, 0, 1, nullptr },
        { ID_BTN_2,       "2",    "2",     ButtonCategory::Digit,       4, 1, 1, nullptr },
        { ID_BTN_3,       "3",    "3",     ButtonCategory::Digit,       4, 2, 1, nullptr },
        { ID_BTN_SUB,     "-",    " - ",   ButtonCategory::Operator,    4, 3, 1, nullptr },
        { ID_BTN_SIGN,    "+/-",  "",      ButtonCategory::Utility,     4, 4, 1, nullptr },
        { ID_BTN_PI,      "π",    "pi",    ButtonCategory::Function,    4, 5, 1, nullptr },

        // Row 5: 0, ., e, +, = (spans 2 columns)
        { ID_BTN_0,       "0",    "0",     ButtonCategory::Digit,       5, 0, 1, nullptr },
        { ID_BTN_DOT,     ".",    ".",     ButtonCategory::Digit,       5, 1, 1, nullptr },
        { ID_BTN_E,       "e",    "e",     ButtonCategory::Function,    5, 2, 1, nullptr },
        { ID_BTN_ADD,     "+",    " + ",   ButtonCategory::Operator,    5, 3, 1, nullptr },
        { ID_BTN_EQUALS,  "=",    "",      ButtonCategory::ActionEquals,5, 4, 2, nullptr }
    };
}

bool MainWindow::registerAndCreate(HINSTANCE hInst, int nCmdShow) {
    hInstance = hInst;

    WNDCLASSEXA wc = { sizeof(WNDCLASSEXA) };
    wc.lpfnWndProc   = MainWindow::WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "DsaCalculatorWindowClass";
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = hBrushBg;
    wc.style         = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassExA(&wc)) {
        return false;
    }

    hwndMain = CreateWindowExA(
        0,
        wc.lpszClassName,
        "DSA Scientific Calculator (C++)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 460, 640,
        nullptr, nullptr, hInstance, this
    );

    if (!hwndMain) {
        return false;
    }

    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);
    return true;
}

void MainWindow::createControls(HWND hwnd) {
    // Upper expression display (read-only static)
    hwndDisplayExpr = CreateWindowExA(
        0, "STATIC", "",
        WS_CHILD | WS_VISIBLE | SS_RIGHT,
        15, 15, 415, 25,
        hwnd, (HMENU)(INT_PTR)ID_DISPLAY_EXPR, hInstance, nullptr
    );
    SendMessageA(hwndDisplayExpr, WM_SETFONT, (WPARAM)hFontExpr, TRUE);

    // Lower result display (large bold static)
    hwndDisplayResult = CreateWindowExA(
        0, "STATIC", "0",
        WS_CHILD | WS_VISIBLE | SS_RIGHT,
        15, 40, 415, 50,
        hwnd, (HMENU)(INT_PTR)ID_DISPLAY_RESULT, hInstance, nullptr
    );
    SendMessageA(hwndDisplayResult, WM_SETFONT, (WPARAM)hFontResult, TRUE);

    // Create owner-drawn buttons
    for (size_t i = 0; i < buttons.size(); ++i) {
        CalcButton& btn = buttons[i];
        btn.hwnd = CreateWindowExA(
            0, "BUTTON", btn.label.c_str(),
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_TABSTOP,
            0, 0, 0, 0,
            hwnd, (HMENU)(INT_PTR)btn.id, hInstance, nullptr
        );
        SendMessageA(btn.hwnd, WM_SETFONT, (WPARAM)hFontButton, TRUE);
    }
}

void MainWindow::layoutControls(int width, int height) {
    int margin = 12;
    int displayHeight = 90;

    // Position display box
    int displayWidth = width - (margin * 2);
    if (hwndDisplayExpr && hwndDisplayResult) {
        MoveWindow(hwndDisplayExpr, margin + 10, margin + 8, displayWidth - 20, 24, TRUE);
        MoveWindow(hwndDisplayResult, margin + 10, margin + 35, displayWidth - 20, 45, TRUE);
    }

    // Grid layout
    int gridTop = margin + displayHeight + 10;
    int gridHeight = height - gridTop - margin;
    int gridWidth = width - (margin * 2);

    int rows = 6;
    int cols = 6;
    int spacing = 8;

    int cellWidth = (gridWidth - (spacing * (cols - 1))) / cols;
    int cellHeight = (gridHeight - (spacing * (rows - 1))) / rows;

    for (size_t i = 0; i < buttons.size(); ++i) {
        CalcButton& btn = buttons[i];
        if (!btn.hwnd) continue;

        int x = margin + btn.col * (cellWidth + spacing);
        int y = gridTop + btn.row * (cellHeight + spacing);
        int w = cellWidth * btn.colSpan + spacing * (btn.colSpan - 1);
        int h = cellHeight;

        MoveWindow(btn.hwnd, x, y, w, h, TRUE);
    }
}

void MainWindow::drawCustomButton(LPDRAWITEMSTRUCT pDIS) {
    CalcButton* targetBtn = nullptr;
    for (size_t i = 0; i < buttons.size(); ++i) {
        if (buttons[i].id == static_cast<int>(pDIS->CtlID)) {
            targetBtn = &buttons[i];
            break;
        }
    }
    if (!targetBtn) return;

    HDC hdc = pDIS->hDC;
    RECT rc = pDIS->rcItem;
    bool isPressed = (pDIS->itemState & ODS_SELECTED);

    // Pick button background brush & text color
    HBRUSH fillBrush = hBrushBtnDigit;
    COLORREF textColor = COLOR_TEXT_RES;

    if (isPressed) {
        fillBrush = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    } else {
        switch (targetBtn->category) {
            case ButtonCategory::Digit:
                fillBrush = hBrushBtnDigit;
                textColor = RGB(245, 245, 245);
                break;
            case ButtonCategory::Operator:
                fillBrush = hBrushBtnOp;
                textColor = RGB(255, 214, 102); // Warm accent for operators
                break;
            case ButtonCategory::Function:
                fillBrush = hBrushBtnFunc;
                textColor = RGB(140, 200, 255); // Soft cyan/blue for functions
                break;
            case ButtonCategory::ActionClear:
                fillBrush = hBrushBtnClear;
                textColor = RGB(255, 255, 255);
                break;
            case ButtonCategory::ActionEquals:
                fillBrush = hBrushBtnEquals;
                textColor = RGB(255, 255, 255);
                break;
            case ButtonCategory::Utility:
                fillBrush = hBrushBtnUtil;
                textColor = RGB(168, 218, 220);
                break;
        }
    }

    // Draw rounded rectangle
    HPEN hPen = CreatePen(PS_SOLID, 1, isPressed ? RGB(120, 120, 120) : RGB(50, 53, 58));
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    HGDIOBJ oldBrush = SelectObject(hdc, fillBrush);

    RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 10, 10);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hPen);

    // Draw text centered
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, textColor);

    HFONT fontToUse = (targetBtn->label.length() > 3) ? hFontSmall : hFontButton;
    HGDIOBJ oldFont = SelectObject(hdc, fontToUse);

    DrawTextA(hdc, targetBtn->label.c_str(), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, oldFont);
}

void MainWindow::updateDisplay() {
    if (hwndDisplayExpr) {
        SetWindowTextA(hwndDisplayExpr, currentExpression.c_str());
    }
    if (hwndDisplayResult) {
        SetWindowTextA(hwndDisplayResult, currentResultText.c_str());
    }
    // Repaint display box
    InvalidateRect(hwndMain, nullptr, FALSE);
}

void MainWindow::appendToExpression(const std::string& text) {
    if (isResultEvaluated) {
        // If an operator is pressed after evaluation, chain calculation from last result
        if (text.find('+') != std::string::npos || text.find('-') != std::string::npos ||
            text.find('*') != std::string::npos || text.find('/') != std::string::npos ||
            text.find('^') != std::string::npos || text.find('%') != std::string::npos) {
            currentExpression = currentResultText + text;
        } else {
            // New calculation starting with number/function
            currentExpression = text;
        }
        isResultEvaluated = false;
        isErrorState = false;
    } else {
        currentExpression += text;
    }
    updateDisplay();
}

void MainWindow::onCalculate() {
    if (currentExpression.empty()) {
        return;
    }

    CalculationResult res = calculator.calculate(currentExpression);
    if (res.success) {
        currentResultText = res.formattedResult;
        isErrorState = false;
    } else {
        currentResultText = res.errorMessage;
        isErrorState = true;
    }
    isResultEvaluated = true;
    updateDisplay();
}

void MainWindow::onClear(bool allClear) {
    currentExpression = "";
    currentResultText = "0";
    isResultEvaluated = false;
    isErrorState = false;
    if (allClear) {
        // Keeps history intact, resets active state
    }
    updateDisplay();
}

void MainWindow::onDelete() {
    if (isResultEvaluated) {
        currentExpression = "";
        currentResultText = "0";
        isResultEvaluated = false;
        updateDisplay();
        return;
    }

    if (!currentExpression.empty()) {
        // If trailing space operator " + ", pop all 3 characters
        if (currentExpression.size() >= 3 && currentExpression.back() == ' ') {
            currentExpression.pop_back();
            currentExpression.pop_back();
            currentExpression.pop_back();
        } else {
            currentExpression.pop_back();
        }
        updateDisplay();
    }
}

void MainWindow::onToggleSign() {
    if (currentExpression.empty()) {
        currentExpression = "-";
        updateDisplay();
        return;
    }

    // If result was evaluated, toggle sign of result
    if (isResultEvaluated && !isErrorState) {
        if (!currentResultText.empty() && currentResultText[0] == '-') {
            currentResultText.erase(0, 1);
        } else {
            currentResultText = "-" + currentResultText;
        }
        currentExpression = currentResultText;
        isResultEvaluated = false;
        updateDisplay();
        return;
    }

    // Wrap current expression in negation
    if (currentExpression.rfind("-(", 0) == 0 && currentExpression.back() == ')') {
        // Remove - ( ... )
        currentExpression = currentExpression.substr(2, currentExpression.length() - 3);
    } else {
        currentExpression = "-(" + currentExpression + ")";
    }
    updateDisplay();
}

void MainWindow::onToggleAngleMode() {
    if (calculator.getAngleMode() == ExpressionParser::AngleMode::Radians) {
        calculator.setAngleMode(ExpressionParser::AngleMode::Degrees);
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].id == ID_BTN_DEG_RAD) {
                buttons[i].label = "DEG";
                InvalidateRect(buttons[i].hwnd, nullptr, TRUE);
                break;
            }
        }
    } else {
        calculator.setAngleMode(ExpressionParser::AngleMode::Radians);
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].id == ID_BTN_DEG_RAD) {
                buttons[i].label = "RAD";
                InvalidateRect(buttons[i].hwnd, nullptr, TRUE);
                break;
            }
        }
    }
}

// History Dialog Procedure
LRESULT CALLBACK MainWindow::HistoryDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static MainWindow* pThis = nullptr;
    switch (uMsg) {
        case WM_INITDIALOG:
            pThis = reinterpret_cast<MainWindow*>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
            return TRUE;

        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            HWND hList = GetDlgItem(hwnd, ID_HIST_LIST);

            if (wmId == ID_HIST_CLEAR && pThis) {
                pThis->calculator.clearHistory();
                SendMessageA(hList, LB_RESETCONTENT, 0, 0);
            } else if (wmId == ID_HIST_RECALL && pThis) {
                int sel = (int)SendMessageA(hList, LB_GETCURSEL, 0, 0);
                if (sel != LB_ERR) {
                    std::string expr, res;
                    if (pThis->calculator.getHistory().getEntry(sel, expr, res)) {
                        pThis->appendToExpression(res);
                        DestroyWindow(hwnd);
                    }
                }
            } else if (wmId == ID_HIST_CLOSE || wmId == IDCANCEL) {
                DestroyWindow(hwnd);
            }
            break;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            return TRUE;
    }
    return FALSE;
}

void MainWindow::onShowHistory() {
    std::vector<std::string> entries = calculator.getHistory().getFormattedEntries();

    HWND hwndDlg = CreateWindowExA(
        WS_EX_DLGMODALFRAME,
        "STATIC", "Calculation History",
        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        300, 200, 360, 420,
        hwndMain, nullptr, hInstance, nullptr
    );

    // Create Listbox inside
    HWND hList = CreateWindowExA(
        WS_EX_CLIENTEDGE, "LISTBOX", "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        15, 15, 315, 290,
        hwndDlg, (HMENU)(INT_PTR)ID_HIST_LIST, hInstance, nullptr
    );
    SendMessageA(hList, WM_SETFONT, (WPARAM)hFontExpr, TRUE);

    if (entries.empty()) {
        SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)"No calculation history yet.");
    } else {
        for (size_t i = 0; i < entries.size(); ++i) {
            SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)entries[i].c_str());
        }
    }

    // Buttons: Recall, Clear, Close
    HWND btnRecall = CreateWindowA("BUTTON", "Recall Result", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                   15, 320, 100, 35, hwndDlg, (HMENU)(INT_PTR)ID_HIST_RECALL, hInstance, nullptr);
    HWND btnClear = CreateWindowA("BUTTON", "Clear All", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                  122, 320, 95, 35, hwndDlg, (HMENU)(INT_PTR)ID_HIST_CLEAR, hInstance, nullptr);
    HWND btnClose = CreateWindowA("BUTTON", "Close", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                  224, 320, 105, 35, hwndDlg, (HMENU)(INT_PTR)ID_HIST_CLOSE, hInstance, nullptr);

    SendMessageA(btnRecall, WM_SETFONT, (WPARAM)hFontSmall, TRUE);
    SendMessageA(btnClear, WM_SETFONT, (WPARAM)hFontSmall, TRUE);
    SendMessageA(btnClose, WM_SETFONT, (WPARAM)hFontSmall, TRUE);

    // Modal-like wait loop
    EnableWindow(hwndMain, FALSE);
    MSG msg;
    while (IsWindow(hwndDlg) && GetMessageA(&msg, nullptr, 0, 0)) {
        if (msg.message == WM_COMMAND) {
            int cmd = LOWORD(msg.wParam);
            if (cmd == ID_HIST_CLEAR) {
                calculator.clearHistory();
                SendMessageA(hList, LB_RESETCONTENT, 0, 0);
                SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)"History cleared.");
            } else if (cmd == ID_HIST_RECALL) {
                int sel = (int)SendMessageA(hList, LB_GETCURSEL, 0, 0);
                std::string expr, res;
                if (sel != LB_ERR && calculator.getHistory().getEntry(sel, expr, res)) {
                    appendToExpression(res);
                    DestroyWindow(hwndDlg);
                }
            } else if (cmd == ID_HIST_CLOSE) {
                DestroyWindow(hwndDlg);
            }
        }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    EnableWindow(hwndMain, TRUE);
    SetForegroundWindow(hwndMain);
}

void MainWindow::onButtonClick(int buttonId) {
    switch (buttonId) {
        case ID_BTN_EQUALS:
            onCalculate();
            break;
        case ID_BTN_AC:
            onClear(true);
            break;
        case ID_BTN_C:
            onClear(false);
            break;
        case ID_BTN_DEL:
            onDelete();
            break;
        case ID_BTN_SIGN:
            onToggleSign();
            break;
        case ID_BTN_DEG_RAD:
            onToggleAngleMode();
            break;
        case ID_BTN_HIST:
            onShowHistory();
            break;
        default: {
            for (size_t i = 0; i < buttons.size(); ++i) {
                if (buttons[i].id == buttonId) {
                    appendToExpression(buttons[i].insertText);
                    break;
                }
            }
            break;
        }
    }
}

LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    MainWindow* pThis = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<MainWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->hwndMain = hwnd;
    } else {
        pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        switch (uMsg) {
            case WM_CREATE:
                pThis->createControls(hwnd);
                return 0;

            case WM_SIZE: {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);
                pThis->layoutControls(width, height);
                return 0;
            }

            case WM_GETMINMAXINFO: {
                MINMAXINFO* pmmi = reinterpret_cast<MINMAXINFO*>(lParam);
                pmmi->ptMinTrackSize.x = 400;
                pmmi->ptMinTrackSize.y = 560;
                return 0;
            }

            case WM_DRAWITEM: {
                LPDRAWITEMSTRUCT pDIS = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
                pThis->drawCustomButton(pDIS);
                return TRUE;
            }

            case WM_CTLCOLORSTATIC: {
                HDC hdcStatic = (HDC)wParam;
                HWND hStatic = (HWND)lParam;

                if (hStatic == pThis->hwndDisplayExpr) {
                    SetBkMode(hdcStatic, TRANSPARENT);
                    SetTextColor(hdcStatic, COLOR_TEXT_EXPR);
                    return (LRESULT)pThis->hBrushDisplay;
                } else if (hStatic == pThis->hwndDisplayResult) {
                    SetBkMode(hdcStatic, TRANSPARENT);
                    SetTextColor(hdcStatic, pThis->isErrorState ? COLOR_TEXT_ERR : COLOR_TEXT_RES);
                    return (LRESULT)pThis->hBrushDisplay;
                }
                SetBkMode(hdcStatic, TRANSPARENT);
                return (LRESULT)pThis->hBrushBg;
            }

            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                RECT clientRc;
                GetClientRect(hwnd, &clientRc);

                // Draw rounded display container card
                RECT displayRc = { 12, 12, clientRc.right - 12, 12 + 90 };
                HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(55, 58, 64));
                HGDIOBJ oldPen = SelectObject(hdc, hBorderPen);
                HGDIOBJ oldBrush = SelectObject(hdc, pThis->hBrushDisplay);

                RoundRect(hdc, displayRc.left, displayRc.top, displayRc.right, displayRc.bottom, 12, 12);

                SelectObject(hdc, oldBrush);
                SelectObject(hdc, oldPen);
                DeleteObject(hBorderPen);

                EndPaint(hwnd, &ps);
                return 0;
            }

            case WM_COMMAND: {
                int wmId = LOWORD(wParam);
                pThis->onButtonClick(wmId);
                return 0;
            }

            case WM_CHAR: {
                char ch = static_cast<char>(wParam);
                if (ch >= '0' && ch <= '9') {
                    pThis->appendToExpression(std::string(1, ch));
                } else if (ch == '.') {
                    pThis->appendToExpression(".");
                } else if (ch == '+') {
                    pThis->appendToExpression(" + ");
                } else if (ch == '-') {
                    pThis->appendToExpression(" - ");
                } else if (ch == '*') {
                    pThis->appendToExpression(" * ");
                } else if (ch == '/') {
                    pThis->appendToExpression(" / ");
                } else if (ch == '%') {
                    pThis->appendToExpression(" % ");
                } else if (ch == '^') {
                    pThis->appendToExpression("^");
                } else if (ch == '(') {
                    pThis->appendToExpression("(");
                } else if (ch == ')') {
                    pThis->appendToExpression(")");
                } else if (ch == '=') {
                    pThis->onCalculate();
                } else if (ch == 13) { // Enter key
                    pThis->onCalculate();
                } else if (ch == 8) {  // Backspace key
                    pThis->onDelete();
                } else if (ch == 27) { // Escape key
                    pThis->onClear(true);
                }
                return 0;
            }

            case WM_KEYDOWN: {
                if (wParam == VK_RETURN) {
                    pThis->onCalculate();
                    return 0;
                } else if (wParam == VK_BACK) {
                    pThis->onDelete();
                    return 0;
                } else if (wParam == VK_ESCAPE) {
                    pThis->onClear(true);
                    return 0;
                } else if (wParam == VK_DELETE) {
                    pThis->onClear(false);
                    return 0;
                }
                break;
            }

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }
    }

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

int MainWindow::runMessageLoop() {
    MSG msg;
    while (GetMessageA(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return static_cast<int>(msg.wParam);
}
