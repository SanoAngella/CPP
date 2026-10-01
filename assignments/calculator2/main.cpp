#include <windows.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "Calculator.h"

#define ID_DISPLAY 100
#define ID_BUTTON_BASE 200

// Theme Colors (Black and Orange)
static const COLORREF COLOR_BG          = RGB(14, 14, 16);     // Dark background
static const COLORREF COLOR_DISPLAY_BG  = RGB(24, 24, 28);     // Display card
static const COLORREF COLOR_ORANGE      = RGB(255, 149, 0);    // Vibrant orange for operators/equals
static const COLORREF COLOR_ORANGE_DARK = RGB(216, 115, 0);    // Pressed orange
static const COLORREF COLOR_CLEAR_RED   = RGB(216, 67, 21);    // Burnt orange / clear
static const COLORREF COLOR_DIGIT_BG    = RGB(36, 36, 40);     // Digit button
static const COLORREF COLOR_TEXT_WHITE  = RGB(255, 255, 255);  // High contrast white

Calculator calculator;
HWND display;
HBRUSH hBrushBg = nullptr;
HBRUSH hBrushDisplay = nullptr;
HFONT hButtonFont = nullptr;

std::string formatResult(double value)
{
    std::ostringstream out;
    out << std::setprecision(12) << value;

    std::string result = out.str();

    if (result.find('.') != std::string::npos)
    {
        while (!result.empty() && result.back() == '0')
            result.pop_back();

        if (!result.empty() && result.back() == '.')
            result.pop_back();
    }

    return result;
}

void appendText(const char* text)
{
    int length = GetWindowTextLengthA(display);

    if (length > 0 && length < 900)
    {
        char buffer[1024];
        GetWindowTextA(display, buffer, sizeof(buffer));

        std::string current(buffer);

        if (current == "Error")
            current.clear();

        current += text;
        SetWindowTextA(display, current.c_str());
    }
    else
    {
        SetWindowTextA(display, text);
    }
}

void clearDisplay()
{
    SetWindowTextA(display, "");
}

void deleteLast()
{
    int length = GetWindowTextLengthA(display);

    if (length <= 0)
        return;

    char buffer[1024];
    GetWindowTextA(display, buffer, sizeof(buffer));

    std::string current(buffer);
    current.pop_back();

    SetWindowTextA(display, current.c_str());
}

void calculateResult()
{
    int length = GetWindowTextLengthA(display);

    if (length == 0)
        return;

    char buffer[1024];
    GetWindowTextA(display, buffer, sizeof(buffer));

    try
    {
        double result = calculator.calculate(buffer);
        SetWindowTextA(display, formatResult(result).c_str());
    }
    catch (const std::exception& e)
    {
        SetWindowTextA(display, "Error");
    }
}

void handleButton(int id)
{
    switch (id)
    {
        case ID_BUTTON_BASE + 0: clearDisplay(); break;
        case ID_BUTTON_BASE + 1: deleteLast(); break;
        case ID_BUTTON_BASE + 2: appendText("%"); break;
        case ID_BUTTON_BASE + 3: appendText("/"); break;

        case ID_BUTTON_BASE + 4: appendText("7"); break;
        case ID_BUTTON_BASE + 5: appendText("8"); break;
        case ID_BUTTON_BASE + 6: appendText("9"); break;
        case ID_BUTTON_BASE + 7: appendText("*"); break;

        case ID_BUTTON_BASE + 8: appendText("4"); break;
        case ID_BUTTON_BASE + 9: appendText("5"); break;
        case ID_BUTTON_BASE + 10: appendText("6"); break;
        case ID_BUTTON_BASE + 11: appendText("-"); break;

        case ID_BUTTON_BASE + 12: appendText("1"); break;
        case ID_BUTTON_BASE + 13: appendText("2"); break;
        case ID_BUTTON_BASE + 14: appendText("3"); break;
        case ID_BUTTON_BASE + 15: appendText("+"); break;

        case ID_BUTTON_BASE + 16: appendText("^"); break;
        case ID_BUTTON_BASE + 17: appendText("0"); break;
        case ID_BUTTON_BASE + 18: appendText("."); break;
        case ID_BUTTON_BASE + 19: calculateResult(); break;

        // x² button
        case ID_BUTTON_BASE + 20: appendText("^2"); break;
    }
}

void drawButton(LPDRAWITEMSTRUCT pDIS)
{
    HDC hdc = pDIS->hDC;
    RECT rc = pDIS->rcItem;
    int id = static_cast<int>(pDIS->CtlID) - ID_BUTTON_BASE;
    bool isPressed = (pDIS->itemState & ODS_SELECTED);

    HBRUSH fillBrush = nullptr;
    HPEN borderPen = nullptr;
    COLORREF textColor = COLOR_TEXT_WHITE;

    // Determine button style
    // Operators & Equals: 3(/), 7(*), 11(-), 15(+), 16(^), 19(=), 20(x²)
    // Clear & Del: 0(C), 1(DEL), 2(%)
    // Digits: 4,5,6, 8,9,10, 12,13,14, 17(0), 18(.)
    if (id == 19 || id == 3 || id == 7 || id == 11 || id == 15) // Main Orange Operators
    {
        if (isPressed) {
            fillBrush = CreateSolidBrush(COLOR_ORANGE_DARK);
            borderPen = CreatePen(PS_SOLID, 1, COLOR_ORANGE_DARK);
        } else {
            fillBrush = CreateSolidBrush(COLOR_ORANGE);
            borderPen = CreatePen(PS_SOLID, 1, RGB(255, 175, 45));
        }
        textColor = COLOR_TEXT_WHITE;
    }
    else if (id == 0 || id == 1) // Clear / Del
    {
        fillBrush = CreateSolidBrush(isPressed ? RGB(160, 45, 15) : COLOR_CLEAR_RED);
        borderPen = CreatePen(PS_SOLID, 1, RGB(235, 80, 30));
        textColor = COLOR_TEXT_WHITE;
    }
    else if (id == 2 || id == 16 || id == 20) // %, ^, x²
    {
        fillBrush = CreateSolidBrush(isPressed ? RGB(65, 68, 76) : RGB(28, 29, 34));
        borderPen = CreatePen(PS_SOLID, 1, RGB(50, 52, 60));
        textColor = RGB(255, 183, 77); // Amber
    }
    else // Digits & Dot
    {
        fillBrush = CreateSolidBrush(isPressed ? RGB(65, 68, 76) : COLOR_DIGIT_BG);
        borderPen = CreatePen(PS_SOLID, 1, RGB(50, 52, 58));
        textColor = COLOR_TEXT_WHITE;
    }

    HGDIOBJ oldBrush = SelectObject(hdc, fillBrush);
    HGDIOBJ oldPen = SelectObject(hdc, borderPen);

    RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 10, 10);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(fillBrush);
    DeleteObject(borderPen);

    char text[32];
    GetWindowTextA(pDIS->hwndItem, text, sizeof(text));

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, textColor);

    HGDIOBJ oldFont = SelectObject(hdc, hButtonFont);
    DrawTextA(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hdc, oldFont);
}

LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DRAWITEM:
        {
            LPDRAWITEMSTRUCT pDIS = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
            drawButton(pDIS);
            return TRUE;
        }

        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORSTATIC:
        {
            HDC hdcStatic = (HDC)wParam;
            SetBkMode(hdcStatic, TRANSPARENT);
            SetTextColor(hdcStatic, COLOR_TEXT_WHITE);
            return (LRESULT)hBrushDisplay;
        }

        case WM_COMMAND:
        {
            int id = LOWORD(wParam);

            if (id >= ID_BUTTON_BASE && id <= ID_BUTTON_BASE + 20)
                handleButton(id);

            return 0;
        }

        case WM_KEYDOWN:
        {
            if (wParam == VK_RETURN)
                calculateResult();
            else if (wParam == VK_BACK)
                deleteLast();
            else if (wParam == VK_ESCAPE)
                clearDisplay();

            return 0;
        }

        case WM_DESTROY:
            DeleteObject(hBrushBg);
            DeleteObject(hBrushDisplay);
            DeleteObject(hButtonFont);
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int nCmdShow)
{
    hBrushBg = CreateSolidBrush(COLOR_BG);
    hBrushDisplay = CreateSolidBrush(COLOR_DISPLAY_BG);

    const char CLASS_NAME[] = "CppCalculatorWindow";

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = hBrushBg;

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "C++ Calculator (Black & Orange)",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        390,
        570,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd)
        return 0;

    display = CreateWindowExA(
        0,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_AUTOHSCROLL,
        25,
        25,
        325,
        65,
        hwnd,
        (HMENU)ID_DISPLAY,
        hInstance,
        nullptr
    );

    HFONT displayFont = CreateFontA(
        32, 0, 0, 0, FW_BOLD,
        FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH,
        "Segoe UI"
    );

    hButtonFont = CreateFontA(
        18, 0, 0, 0, FW_SEMIBOLD,
        FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH,
        "Segoe UI"
    );

    SendMessageA(display, WM_SETFONT, (WPARAM)displayFont, TRUE);

    const char* labels[] =
    {
        "C", "DEL", "%", "/",
        "7", "8", "9", "*",
        "4", "5", "6", "-",
        "1", "2", "3", "+",
        "^", "0", ".", "=",
        "x²"
    };

    int buttonWidth = 72;
    int buttonHeight = 54;
    int gap = 8;
    int startX = 25;
    int startY = 110;

    for (int i = 0; i < 21; i++)
    {
        int row;
        int col;

        if (i < 20)
        {
            row = i / 4;
            col = i % 4;
        }
        else
        {
            row = 5;
            col = 0;
        }

        int x = startX + col * (buttonWidth + gap);
        int y = startY + row * (buttonHeight + gap);

        int width = buttonWidth;

        if (i == 20)
            width = buttonWidth * 2 + gap;

        CreateWindowExA(
            0,
            "BUTTON",
            labels[i],
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            x,
            y,
            width,
            buttonHeight,
            hwnd,
            (HMENU)(INT_PTR)(ID_BUTTON_BASE + i),
            hInstance,
            nullptr
        );
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};

    while (GetMessageA(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;
}
