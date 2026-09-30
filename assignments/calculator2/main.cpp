#include <windows.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "Calculator.h"

#define ID_DISPLAY 100
#define ID_BUTTON_BASE 200

Calculator calculator;
HWND display;

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

        // x² button: turn 5 into 5² by appending ^2.
        case ID_BUTTON_BASE + 20: appendText("^2"); break;
    }
}

LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (uMsg)
    {
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
    const char CLASS_NAME[] = "CppCalculatorWindow";

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(245, 247, 250));

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "C++ Calculator",
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
        WS_EX_CLIENTEDGE,
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
        30, 0, 0, 0, FW_NORMAL,
        FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
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
    int buttonHeight = 58;
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
            // x² gets its own final row spanning the first button position.
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
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x,
            y,
            width,
            buttonHeight,
            hwnd,
            (HMENU)(ID_BUTTON_BASE + i),
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
