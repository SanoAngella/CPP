#include <windows.h>
#include <string>
#include <vector>
#include <cctype>
#include <ctime>

#include "WordGame.h"

// Control Identifiers
#define ID_WORD        100
#define ID_ATTEMPTS    101
#define ID_GUESSED     102
#define ID_MESSAGE     103

#define ID_LETTER_BASE 200
#define ID_NEW_GAME    300

// Theme Colors (Sleek Black and Orange Theme)
static const COLORREF COLOR_BG             = RGB(18, 18, 22);     // Pitch Dark Background (#121216)
static const COLORREF COLOR_CARD_BG        = RGB(28, 30, 36);     // Dark Charcoal Card (#1C1E24)
static const COLORREF COLOR_CARD_BORDER    = RGB(55, 60, 72);     // Card Border (#373C48)
static const COLORREF COLOR_ORANGE_ACCENT  = RGB(255, 149, 0);    // Radiant Orange (#FF9500)
static const COLORREF COLOR_ORANGE_LIGHT   = RGB(255, 183, 77);   // Warm Amber (#FFB74D)
static const COLORREF COLOR_ORANGE_DARK    = RGB(216, 115, 0);    // Deep Orange for pressed states
static const COLORREF COLOR_TEXT_MUTED     = RGB(170, 175, 185);  // Slate Gray text
static const COLORREF COLOR_TEXT_WHITE     = RGB(250, 250, 250);  // High contrast white
static const COLORREF COLOR_WIN_GREEN      = RGB(76, 217, 100);   // Emerald win text
static const COLORREF COLOR_LOSS_RED       = RGB(255, 99, 71);    // Tomato loss text

static const COLORREF COLOR_BTN_NORMAL     = RGB(38, 41, 50);     // Normal letter button
static const COLORREF COLOR_BTN_BORDER     = RGB(62, 66, 80);     // Normal button border
static const COLORREF COLOR_BTN_DISABLED   = RGB(24, 25, 30);     // Used / disabled button
static const COLORREF COLOR_BTN_DIS_TEXT   = RGB(80, 84, 96);     // Disabled button text

// Global State
HWND hMainWnd   = nullptr;
HWND hWord      = nullptr;
HWND hAttempts  = nullptr;
HWND hGuessed   = nullptr;
HWND hMessage   = nullptr;
HWND hNewGameBtn= nullptr;
HWND hLetterBtns[26] = { nullptr };

HBRUSH hBrushBg       = nullptr;
HBRUSH hBrushCard     = nullptr;
HBRUSH hBrushBtnNorm  = nullptr;
HBRUSH hBrushBtnDis   = nullptr;
HBRUSH hBrushOrange   = nullptr;

HFONT hFontTitle  = nullptr;
HFONT hFontWord   = nullptr;
HFONT hFontStatus = nullptr;
HFONT hFontButton = nullptr;

WordGame* game = nullptr;

// Curated Bank of Words
static const std::vector<std::string> WORD_BANK = {
    "COMPUTER",
    "ALGORITHM",
    "PROGRAMMING",
    "DEVELOPER",
    "KEYBOARD",
    "SOFTWARE",
    "DATABASE",
    "INTERNET",
    "FUNCTION",
    "VARIABLE",
    "POINTER",
    "STRUCTURE",
    "COMPILER",
    "ENGINEER",
    "INTERFACE",
    "NETWORK",
    "SECURITY",
    "GRAPHICS",
    "TERMINAL",
    "RECURSION"
};

static size_t currentWordIndex = 0;

std::string getNextSecretWord() {
    if (WORD_BANK.empty()) return "COMPUTER";
    std::string word = WORD_BANK[currentWordIndex % WORD_BANK.size()];
    currentWordIndex++;
    return word;
}

void updateDisplay()
{
    if (game == nullptr)
        return;

    // Display formatted hidden word with wide spacing
    std::string word = game->getHiddenWord();
    std::string formattedWord;
    for (char c : word)
    {
        formattedWord += c;
        formattedWord += ' ';
    }
    SetWindowTextA(hWord, formattedWord.c_str());

    // Display attempts remaining
    std::string attempts = "Attempts remaining: " + std::to_string(game->getAttempts()) + " / 6";
    SetWindowTextA(hAttempts, attempts.c_str());

    // Display guessed letters list
    std::string guessed = "Guessed letters: " + game->getGuessedLetters();
    SetWindowTextA(hGuessed, guessed.c_str());

    // Check game state and update message
    if (game->hasWon())
    {
        SetWindowTextA(hMessage, "CONGRATULATIONS! YOU WON!");
        // Disable all letter buttons when game is won
        for (int i = 0; i < 26; i++) {
            if (hLetterBtns[i]) {
                EnableWindow(hLetterBtns[i], FALSE);
            }
        }
    }
    else if (game->hasLost())
    {
        std::string lossMsg = "GAME OVER! The word was: " + game->getSecretWord();
        SetWindowTextA(hMessage, lossMsg.c_str());
        // Disable all letter buttons when game is lost
        for (int i = 0; i < 26; i++) {
            if (hLetterBtns[i]) {
                EnableWindow(hLetterBtns[i], FALSE);
            }
        }
    }
    else
    {
        SetWindowTextA(hMessage, "Guess a letter below or type on your keyboard:");
    }

    // Force redraw of status cards
    InvalidateRect(hMainWnd, nullptr, TRUE);
}

void startNewGame()
{
    delete game;
    game = new WordGame(getNextSecretWord());

    // Re-enable all 26 letter buttons
    for (int i = 0; i < 26; i++)
    {
        if (hLetterBtns[i])
        {
            EnableWindow(hLetterBtns[i], TRUE);
            InvalidateRect(hLetterBtns[i], nullptr, TRUE);
        }
    }

    updateDisplay();
}

void processGuess(char letter)
{
    letter = std::toupper(static_cast<unsigned char>(letter));
    if (letter < 'A' || letter > 'Z') return;
    if (game == nullptr || game->isGameOver()) return;

    int idx = letter - 'A';
    // If already disabled/guessed, do nothing
    if (hLetterBtns[idx] && !IsWindowEnabled(hLetterBtns[idx])) return;

    game->makeGuess(letter);

    // Disable the clicked button
    if (hLetterBtns[idx])
    {
        EnableWindow(hLetterBtns[idx], FALSE);
        InvalidateRect(hLetterBtns[idx], nullptr, TRUE);
    }

    updateDisplay();
}

void drawOwnerButton(LPDRAWITEMSTRUCT pDIS)
{
    HDC hdc = pDIS->hDC;
    RECT rc = pDIS->rcItem;
    int id = static_cast<int>(pDIS->CtlID);
    bool isPressed = (pDIS->itemState & ODS_SELECTED);
    bool isDisabled = (pDIS->itemState & ODS_DISABLED);

    HBRUSH fillBrush = nullptr;
    HPEN borderPen = nullptr;
    COLORREF textColor = COLOR_TEXT_WHITE;

    if (id == ID_NEW_GAME)
    {
        // New Game button - Radiant Orange styling
        if (isPressed) {
            fillBrush = CreateSolidBrush(COLOR_ORANGE_DARK);
            borderPen = CreatePen(PS_SOLID, 1, COLOR_ORANGE_DARK);
        } else {
            fillBrush = CreateSolidBrush(COLOR_ORANGE_ACCENT);
            borderPen = CreatePen(PS_SOLID, 1, COLOR_ORANGE_LIGHT);
        }
        textColor = RGB(16, 16, 20); // Deep dark text for maximum contrast on orange
    }
    else
    {
        // Letter Buttons
        if (isDisabled) {
            fillBrush = CreateSolidBrush(COLOR_BTN_DISABLED);
            borderPen = CreatePen(PS_SOLID, 1, RGB(38, 40, 48));
            textColor = COLOR_BTN_DIS_TEXT;
        } else if (isPressed) {
            fillBrush = CreateSolidBrush(COLOR_ORANGE_ACCENT);
            borderPen = CreatePen(PS_SOLID, 1, COLOR_ORANGE_LIGHT);
            textColor = RGB(16, 16, 20);
        } else {
            fillBrush = CreateSolidBrush(COLOR_BTN_NORMAL);
            borderPen = CreatePen(PS_SOLID, 1, COLOR_BTN_BORDER);
            textColor = COLOR_TEXT_WHITE;
        }
    }

    HGDIOBJ oldBrush = SelectObject(hdc, fillBrush);
    HGDIOBJ oldPen = SelectObject(hdc, borderPen);

    RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 8, 8);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(fillBrush);
    DeleteObject(borderPen);

    // Get Button text
    char text[64];
    GetWindowTextA(pDIS->hwndItem, text, sizeof(text));

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, textColor);

    HGDIOBJ oldFont = SelectObject(hdc, hFontButton);
    DrawTextA(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hdc, oldFont);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
        {
            // Upper word display banner
            hWord = CreateWindowA(
                "STATIC",
                "_ _ _ _",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                30, 20, 560, 55,
                hwnd, (HMENU)(INT_PTR)ID_WORD, nullptr, nullptr
            );
            SendMessageA(hWord, WM_SETFONT, (WPARAM)hFontWord, TRUE);

            // Attempts remaining label
            hAttempts = CreateWindowA(
                "STATIC",
                "Attempts remaining: 6 / 6",
                WS_VISIBLE | WS_CHILD,
                35, 88, 550, 26,
                hwnd, (HMENU)(INT_PTR)ID_ATTEMPTS, nullptr, nullptr
            );
            SendMessageA(hAttempts, WM_SETFONT, (WPARAM)hFontStatus, TRUE);

            // Guessed letters label
            hGuessed = CreateWindowA(
                "STATIC",
                "Guessed letters: None",
                WS_VISIBLE | WS_CHILD,
                35, 116, 550, 26,
                hwnd, (HMENU)(INT_PTR)ID_GUESSED, nullptr, nullptr
            );
            SendMessageA(hGuessed, WM_SETFONT, (WPARAM)hFontStatus, TRUE);

            // Instructional / Game State Message
            hMessage = CreateWindowA(
                "STATIC",
                "Guess a letter below or type on your keyboard:",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                30, 150, 560, 30,
                hwnd, (HMENU)(INT_PTR)ID_MESSAGE, nullptr, nullptr
            );
            SendMessageA(hMessage, WM_SETFONT, (WPARAM)hFontStatus, TRUE);

            // 26 Letter Buttons (A to Z) arranged in 3 ergonomic rows
            int startX = 35;
            int startY = 190;
            int btnW = 52;
            int btnH = 44;
            int gapX = 8;
            int gapY = 8;

            for (int i = 0; i < 26; i++)
            {
                char letter = 'A' + i;
                std::string text(1, letter);

                int row = 0;
                int col = 0;

                if (i < 9) { // Row 0: A-I (9 letters)
                    row = 0;
                    col = i;
                } else if (i < 18) { // Row 1: J-R (9 letters)
                    row = 1;
                    col = i - 9;
                } else { // Row 2: S-Z (8 letters centered)
                    row = 2;
                    col = i - 18;
                }

                int x = startX + col * (btnW + gapX);
                if (row == 2) {
                    x += (btnW + gapX) / 2; // Center 8 letters on row 3
                }
                int y = startY + row * (btnH + gapY);

                hLetterBtns[i] = CreateWindowA(
                    "BUTTON",
                    text.c_str(),
                    WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | WS_TABSTOP,
                    x, y, btnW, btnH,
                    hwnd, (HMENU)(INT_PTR)(ID_LETTER_BASE + i), nullptr, nullptr
                );
            }

            // Radiant Orange "New Game" Button
            hNewGameBtn = CreateWindowA(
                "BUTTON",
                "NEW GAME",
                WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | WS_TABSTOP,
                220, 355, 180, 45,
                hwnd, (HMENU)(INT_PTR)ID_NEW_GAME, nullptr, nullptr
            );

            break;
        }

        case WM_DRAWITEM:
        {
            LPDRAWITEMSTRUCT pDIS = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
            drawOwnerButton(pDIS);
            return TRUE;
        }

        case WM_CTLCOLORSTATIC:
        {
            HDC hdcStatic = (HDC)wParam;
            HWND hCtrl = (HWND)lParam;

            SetBkMode(hdcStatic, TRANSPARENT);

            if (hCtrl == hWord) {
                SetTextColor(hdcStatic, COLOR_ORANGE_ACCENT);
                return (LRESULT)hBrushCard;
            } else if (hCtrl == hAttempts) {
                SetTextColor(hdcStatic, COLOR_ORANGE_LIGHT);
                return (LRESULT)hBrushCard;
            } else if (hCtrl == hGuessed) {
                SetTextColor(hdcStatic, COLOR_TEXT_MUTED);
                return (LRESULT)hBrushCard;
            } else if (hCtrl == hMessage) {
                if (game && game->hasWon()) {
                    SetTextColor(hdcStatic, COLOR_WIN_GREEN);
                } else if (game && game->hasLost()) {
                    SetTextColor(hdcStatic, COLOR_LOSS_RED);
                } else {
                    SetTextColor(hdcStatic, COLOR_ORANGE_LIGHT);
                }
                return (LRESULT)hBrushCard;
            }

            SetTextColor(hdcStatic, COLOR_TEXT_WHITE);
            return (LRESULT)hBrushBg;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRc;
            GetClientRect(hwnd, &clientRc);

            // Fill window background
            FillRect(hdc, &clientRc, hBrushBg);

            // Draw rounded top display card behind word & status
            RECT cardRc = { 20, 12, clientRc.right - 20, 182 };
            HPEN hBorderPen = CreatePen(PS_SOLID, 1, COLOR_CARD_BORDER);
            HGDIOBJ oldPen = SelectObject(hdc, hBorderPen);
            HGDIOBJ oldBrush = SelectObject(hdc, hBrushCard);

            RoundRect(hdc, cardRc.left, cardRc.top, cardRc.right, cardRc.bottom, 12, 12);

            SelectObject(hdc, oldBrush);
            SelectObject(hdc, oldPen);
            DeleteObject(hBorderPen);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_COMMAND:
        {
            int id = LOWORD(wParam);

            if (id >= ID_LETTER_BASE && id < ID_LETTER_BASE + 26)
            {
                char letter = 'A' + (id - ID_LETTER_BASE);
                processGuess(letter);
            }
            else if (id == ID_NEW_GAME)
            {
                startNewGame();
            }
            break;
        }

        case WM_CHAR:
        {
            char ch = static_cast<char>(wParam);
            if (std::isalpha(static_cast<unsigned char>(ch)))
            {
                processGuess(ch);
            }
            else if (ch == 13 || ch == ' ') // Enter or Space triggers New Game when game over
            {
                if (game && game->isGameOver()) {
                    startNewGame();
                }
            }
            break;
        }

        case WM_DESTROY:
        {
            delete game;
            game = nullptr;

            DeleteObject(hBrushBg);
            DeleteObject(hBrushCard);
            DeleteObject(hBrushBtnNorm);
            DeleteObject(hBrushBtnDis);
            DeleteObject(hBrushOrange);

            DeleteObject(hFontTitle);
            DeleteObject(hFontWord);
            DeleteObject(hFontStatus);
            DeleteObject(hFontButton);

            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProcA(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    // Seed random generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    if (!WORD_BANK.empty()) {
        currentWordIndex = std::rand() % WORD_BANK.size();
    }

    // Initialize GDI Brushes
    hBrushBg       = CreateSolidBrush(COLOR_BG);
    hBrushCard     = CreateSolidBrush(COLOR_CARD_BG);
    hBrushBtnNorm  = CreateSolidBrush(COLOR_BTN_NORMAL);
    hBrushBtnDis   = CreateSolidBrush(COLOR_BTN_DISABLED);
    hBrushOrange   = CreateSolidBrush(COLOR_ORANGE_ACCENT);

    // Initialize Sleek Fonts (Segoe UI)
    hFontTitle  = CreateFontA(-18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hFontWord   = CreateFontA(-32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hFontStatus = CreateFontA(-15, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hFontButton = CreateFontA(-16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");

    const char CLASS_NAME[] = "WordGuessBlackOrangeWindow";

    WNDCLASSEXA wc = { sizeof(WNDCLASSEXA) };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = hBrushBg;
    wc.style         = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassExA(&wc))
        return 0;

    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "Word Guessing Game - Black & Orange Edition",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        635, 465,
        nullptr, nullptr, hInstance, nullptr
    );

    if (hwnd == nullptr)
        return 0;

    hMainWnd = hwnd;

    // Start initial game with random word
    game = new WordGame(getNextSecretWord());

    // Show window and immediately update display to show accurate blanks
    ShowWindow(hwnd, nCmdShow);
    updateDisplay();
    UpdateWindow(hwnd);

    // Message Loop
    MSG msg = {};
    while (GetMessageA(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;
}