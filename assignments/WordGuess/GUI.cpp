#include <windows.h>
#include <string>
#include <vector>
#include <cctype>
#include <ctime>
#include <algorithm>

#include "WordGame.h"

// Control Identifiers
#define ID_CATEGORY_BASE 100
#define ID_LETTER_BASE   200
#define ID_NEW_GAME      300
#define ID_EXIT_GAME     301

// Wordle Color Palette
static const COLORREF COLOR_WORDLE_BG       = RGB(18, 18, 19);    // #121213 Dark Theme Background
static const COLORREF COLOR_CARD_BG         = RGB(26, 26, 27);    // #1A1A1B Display Card
static const COLORREF COLOR_TILE_EMPTY      = RGB(30, 30, 32);    // #1E1E20 Empty tile fill
static const COLORREF COLOR_TILE_BORDER     = RGB(58, 58, 60);    // #3A3A3C Unrevealed tile border
static const COLORREF COLOR_WORDLE_GREEN    = RGB(83, 141, 78);   // #538D4E Wordle Green (Correct letter)
static const COLORREF COLOR_WORDLE_ABSENT   = RGB(58, 58, 60);    // #3A3A3C Wordle Gray (Incorrect letter)
static const COLORREF COLOR_KEY_DEFAULT     = RGB(129, 131, 132); // #818384 Unused keyboard key
static const COLORREF COLOR_KEY_BORDER      = RGB(90, 92, 94);    // Key border
static const COLORREF COLOR_TEXT_WHITE      = RGB(255, 255, 255); // Crisp White text
static const COLORREF COLOR_TEXT_MUTED      = RGB(160, 164, 168); // Muted secondary text
static const COLORREF COLOR_ORANGE_ACCENT   = RGB(255, 149, 0);   // Vibrant Accent for categories
static const COLORREF COLOR_WIN_GOLD        = RGB(255, 215, 0);   // Victory gold
static const COLORREF COLOR_LOSS_RED        = RGB(235, 75, 75);   // Loss red

// Category Structure
struct CategoryData {
    std::string name;
    std::vector<std::string> words;
};

static const std::vector<CategoryData> CATEGORIES = {
    {
        "Animals",
        { "ELEPHANT", "GIRAFFE", "CHEETAH", "KANGAROO", "PENGUIN", "DOLPHIN", "LEOPARD", "OCTOPUS", "CROCODILE" }
    },
    {
        "Technology",
        { "COMPUTER", "ALGORITHM", "PROGRAMMING", "DEVELOPER", "KEYBOARD", "DATABASE", "INTERNET", "SECURITY" }
    },
    {
        "Films",
        { "INCEPTION", "GLADIATOR", "TITANIC", "AVATAR", "INTERSTELLAR", "MATRIX", "CASABLANCA" }
    },
    {
        "Countries",
        { "GERMANY", "BRAZIL", "AUSTRALIA", "CANADA", "JAPAN", "PORTUGAL", "RWANDA", "SWITZERLAND" }
    },
    {
        "Books",
        { "HAMLET", "ODYSSEY", "DRACULA", "MACBETH", "FRANKENSTEIN", "HOBBIT" }
    }
};

static int currentCategoryIndex = 0;

// Global State
HWND hMainWnd    = nullptr;
HWND hMsgLabel   = nullptr;
HWND hStatLabel  = nullptr;
HWND hCatButtons[5]  = { nullptr };
HWND hLetterBtns[26] = { nullptr };
HWND hNewGameBtn = nullptr;
HWND hExitBtn    = nullptr;

HBRUSH hBrushBg        = nullptr;
HBRUSH hBrushCard      = nullptr;
HBRUSH hBrushGreen     = nullptr;
HBRUSH hBrushTileEmpty = nullptr;

HFONT hFontTitle  = nullptr;
HFONT hFontTile   = nullptr;
HFONT hFontStatus = nullptr;
HFONT hFontButton = nullptr;
HFONT hFontSmall  = nullptr;

WordGame* game = nullptr;

std::string getRandomWordFromCategory(int catIdx)
{
    const CategoryData& cat = CATEGORIES[catIdx % CATEGORIES.size()];
    int idx = std::rand() % cat.words.size();
    return cat.words[idx];
}

void updateDisplay();

void startNewGame(int catIdx = -1)
{
    if (catIdx >= 0 && catIdx < static_cast<int>(CATEGORIES.size())) {
        currentCategoryIndex = catIdx;
    }

    delete game;
    std::string secret = getRandomWordFromCategory(currentCategoryIndex);
    game = new WordGame(secret, 6, CATEGORIES[currentCategoryIndex].name);

    // Re-enable and repaint all keyboard buttons
    for (int i = 0; i < 26; i++)
    {
        if (hLetterBtns[i])
        {
            EnableWindow(hLetterBtns[i], TRUE);
            InvalidateRect(hLetterBtns[i], nullptr, TRUE);
        }
    }

    // Update category buttons active state
    for (size_t i = 0; i < CATEGORIES.size(); ++i)
    {
        if (hCatButtons[i]) {
            InvalidateRect(hCatButtons[i], nullptr, TRUE);
        }
    }

    updateDisplay();
}

void updateDisplay()
{
    if (game == nullptr) return;

    // Repaint window to render Wordle tiles in WM_PAINT
    InvalidateRect(hMainWnd, nullptr, TRUE);

    // Update status text
    std::string attemptsStr = "Chances remaining: " + std::to_string(game->getAttempts()) + " / 6";
    std::string guessedStr  = "Guessed: " + (game->getGuessedLetters().empty() ? "None" : game->getGuessedLetters());

    std::string fullStatus = attemptsStr + "   |   " + guessedStr;
    if (hStatLabel) {
        SetWindowTextA(hStatLabel, fullStatus.c_str());
    }

    // Check Win / Loss state
    if (game->hasWon())
    {
        if (hMsgLabel) {
            SetWindowTextA(hMsgLabel, "CONGRATULATIONS! YOU WON!");
        }
        for (int i = 0; i < 26; i++) {
            if (hLetterBtns[i]) EnableWindow(hLetterBtns[i], FALSE);
        }
    }
    else if (game->hasLost())
    {
        if (hMsgLabel) {
            std::string lossMsg = "GAME OVER! The word was: " + game->getSecretWord();
            SetWindowTextA(hMsgLabel, lossMsg.c_str());
        }
        for (int i = 0; i < 26; i++) {
            if (hLetterBtns[i]) EnableWindow(hLetterBtns[i], FALSE);
        }
    }
    else
    {
        if (hMsgLabel) {
            std::string prompt = "Category: " + game->getCategory() + " — Choose a letter:";
            SetWindowTextA(hMsgLabel, prompt.c_str());
        }
    }
}

void processGuess(char letter)
{
    letter = std::toupper(static_cast<unsigned char>(letter));
    if (letter < 'A' || letter > 'Z') return;
    if (game == nullptr || game->isGameOver()) return;

    int idx = letter - 'A';
    if (hLetterBtns[idx] && !IsWindowEnabled(hLetterBtns[idx])) return;

    game->makeGuess(letter);

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
    HFONT fontToUse = hFontButton;

    if (id >= ID_CATEGORY_BASE && id < ID_CATEGORY_BASE + 5)
    {
        // Category Selection Button
        int catIdx = id - ID_CATEGORY_BASE;
        bool isActive = (catIdx == currentCategoryIndex);

        if (isActive) {
            fillBrush = CreateSolidBrush(COLOR_WORDLE_GREEN);
            borderPen = CreatePen(PS_SOLID, 1, RGB(115, 175, 110));
            textColor = COLOR_TEXT_WHITE;
        } else {
            fillBrush = CreateSolidBrush(isPressed ? RGB(45, 45, 48) : RGB(30, 30, 34));
            borderPen = CreatePen(PS_SOLID, 1, RGB(65, 65, 70));
            textColor = COLOR_TEXT_MUTED;
        }
        fontToUse = hFontSmall;
    }
    else if (id == ID_NEW_GAME)
    {
        // Play Again / New Game Button
        fillBrush = CreateSolidBrush(isPressed ? RGB(65, 115, 60) : COLOR_WORDLE_GREEN);
        borderPen = CreatePen(PS_SOLID, 1, RGB(110, 180, 105));
        textColor = COLOR_TEXT_WHITE;
    }
    else if (id == ID_EXIT_GAME)
    {
        // Exit Game Button
        fillBrush = CreateSolidBrush(isPressed ? RGB(50, 50, 55) : RGB(35, 35, 40));
        borderPen = CreatePen(PS_SOLID, 1, RGB(70, 70, 75));
        textColor = COLOR_TEXT_MUTED;
    }
    else
    {
        // Virtual Keyboard Letter Buttons
        int letterIdx = id - ID_LETTER_BASE;
        char letter = 'A' + letterIdx;

        if (isDisabled && game)
        {
            // Already guessed: check if letter is in secret word
            if (game->getSecretWord().find(letter) != std::string::npos) {
                fillBrush = CreateSolidBrush(COLOR_WORDLE_GREEN);
                borderPen = CreatePen(PS_SOLID, 1, RGB(100, 160, 95));
                textColor = COLOR_TEXT_WHITE;
            } else {
                fillBrush = CreateSolidBrush(COLOR_WORDLE_ABSENT);
                borderPen = CreatePen(PS_SOLID, 1, RGB(50, 50, 52));
                textColor = RGB(120, 124, 130);
            }
        }
        else if (isPressed)
        {
            fillBrush = CreateSolidBrush(RGB(150, 153, 155));
            borderPen = CreatePen(PS_SOLID, 1, RGB(170, 173, 175));
            textColor = RGB(18, 18, 19);
        }
        else
        {
            fillBrush = CreateSolidBrush(COLOR_KEY_DEFAULT);
            borderPen = CreatePen(PS_SOLID, 1, COLOR_KEY_BORDER);
            textColor = COLOR_TEXT_WHITE;
        }
    }

    HGDIOBJ oldBrush = SelectObject(hdc, fillBrush);
    HGDIOBJ oldPen = SelectObject(hdc, borderPen);

    RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 6, 6);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(fillBrush);
    DeleteObject(borderPen);

    char text[64];
    GetWindowTextA(pDIS->hwndItem, text, sizeof(text));

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, textColor);

    HGDIOBJ oldFont = SelectObject(hdc, fontToUse);
    DrawTextA(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hdc, oldFont);
}

void renderWordleTiles(HDC hdc, int startY)
{
    if (game == nullptr) return;

    std::string word = game->getHiddenWord();
    int numLetters = static_cast<int>(word.length());
    if (numLetters == 0) return;

    int tileSize = 48;
    int gap = 8;
    int totalWidth = numLetters * tileSize + (numLetters - 1) * gap;
    int startX = (660 - totalWidth) / 2;

    HGDIOBJ oldFont = SelectObject(hdc, hFontTile);
    SetBkMode(hdc, TRANSPARENT);

    for (int i = 0; i < numLetters; ++i)
    {
        char c = word[i];
        int x = startX + i * (tileSize + gap);
        int y = startY;

        RECT tileRc = { x, y, x + tileSize, y + tileSize };

        HBRUSH fillBrush = nullptr;
        HPEN borderPen = nullptr;
        COLORREF txtCol = COLOR_TEXT_WHITE;

        if (c != '_')
        {
            // Correctly revealed letter -> Green tile
            fillBrush = CreateSolidBrush(COLOR_WORDLE_GREEN);
            borderPen = CreatePen(PS_SOLID, 2, RGB(115, 175, 110));
        }
        else
        {
            // Hidden letter -> Dark tile with border
            fillBrush = CreateSolidBrush(COLOR_TILE_EMPTY);
            borderPen = CreatePen(PS_SOLID, 2, COLOR_TILE_BORDER);
        }

        HGDIOBJ oB = SelectObject(hdc, fillBrush);
        HGDIOBJ oP = SelectObject(hdc, borderPen);

        RoundRect(hdc, tileRc.left, tileRc.top, tileRc.right, tileRc.bottom, 8, 8);

        SelectObject(hdc, oB);
        SelectObject(hdc, oP);
        DeleteObject(fillBrush);
        DeleteObject(borderPen);

        if (c != '_')
        {
            std::string s(1, c);
            SetTextColor(hdc, txtCol);
            DrawTextA(hdc, s.c_str(), 1, &tileRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }

    SelectObject(hdc, oldFont);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
        {
            // Category Selector Tabs
            int catW = 114;
            int catH = 32;
            int catStartX = 30;
            int catGap = 8;

            for (size_t i = 0; i < CATEGORIES.size(); ++i)
            {
                int x = catStartX + i * (catW + catGap);
                hCatButtons[i] = CreateWindowA(
                    "BUTTON", CATEGORIES[i].name.c_str(),
                    WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | WS_TABSTOP,
                    x, 15, catW, catH,
                    hwnd, (HMENU)(INT_PTR)(ID_CATEGORY_BASE + i), nullptr, nullptr
                );
            }

            // Message Prompt Label
            hMsgLabel = CreateWindowA(
                "STATIC", "Choose a category or click a letter below:",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                30, 60, 600, 24,
                hwnd, (HMENU)1001, nullptr, nullptr
            );
            SendMessageA(hMsgLabel, WM_SETFONT, (WPARAM)hFontStatus, TRUE);

            // Status Label (Attempts & Guessed letters)
            hStatLabel = CreateWindowA(
                "STATIC", "Chances remaining: 6 / 6",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                30, 160, 600, 24,
                hwnd, (HMENU)1002, nullptr, nullptr
            );
            SendMessageA(hStatLabel, WM_SETFONT, (WPARAM)hFontSmall, TRUE);

            // Virtual Keyboard (QWERTY layout or 3 ergonomic rows)
            int startX = 35;
            int startY = 195;
            int btnW = 56;
            int btnH = 46;
            int gapX = 6;
            int gapY = 8;

            for (int i = 0; i < 26; i++)
            {
                char letter = 'A' + i;
                std::string text(1, letter);

                int row = (i < 9) ? 0 : ((i < 18) ? 1 : 2);
                int col = (i < 9) ? i : ((i < 18) ? (i - 9) : (i - 18));

                int x = startX + col * (btnW + gapX);
                if (row == 2) x += (btnW + gapX) / 2;
                int y = startY + row * (btnH + gapY);

                hLetterBtns[i] = CreateWindowA(
                    "BUTTON", text.c_str(),
                    WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | WS_TABSTOP,
                    x, y, btnW, btnH,
                    hwnd, (HMENU)(INT_PTR)(ID_LETTER_BASE + i), nullptr, nullptr
                );
            }

            // Play Again / New Game & Exit Buttons
            hNewGameBtn = CreateWindowA(
                "BUTTON", "PLAY AGAIN",
                WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | WS_TABSTOP,
                190, 365, 140, 40,
                hwnd, (HMENU)(INT_PTR)ID_NEW_GAME, nullptr, nullptr
            );

            hExitBtn = CreateWindowA(
                "BUTTON", "EXIT",
                WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | WS_TABSTOP,
                340, 365, 130, 40,
                hwnd, (HMENU)(INT_PTR)ID_EXIT_GAME, nullptr, nullptr
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

            if (hCtrl == hMsgLabel) {
                if (game && game->hasWon()) SetTextColor(hdcStatic, COLOR_WIN_GOLD);
                else if (game && game->hasLost()) SetTextColor(hdcStatic, COLOR_LOSS_RED);
                else SetTextColor(hdcStatic, COLOR_TEXT_WHITE);
            } else {
                SetTextColor(hdcStatic, COLOR_TEXT_MUTED);
            }

            return (LRESULT)hBrushBg;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRc;
            GetClientRect(hwnd, &clientRc);

            FillRect(hdc, &clientRc, hBrushBg);

            // Render Wordle Tile Boxes in the center
            renderWordleTiles(hdc, 96);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_COMMAND:
        {
            int id = LOWORD(wParam);

            if (id >= ID_CATEGORY_BASE && id < ID_CATEGORY_BASE + 5)
            {
                int catIdx = id - ID_CATEGORY_BASE;
                startNewGame(catIdx);
            }
            else if (id >= ID_LETTER_BASE && id < ID_LETTER_BASE + 26)
            {
                char letter = 'A' + (id - ID_LETTER_BASE);
                processGuess(letter);
            }
            else if (id == ID_NEW_GAME)
            {
                startNewGame();
            }
            else if (id == ID_EXIT_GAME)
            {
                DestroyWindow(hwnd);
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
            else if (ch == 27) // Escape key exits
            {
                DestroyWindow(hwnd);
            }
            break;
        }

        case WM_DESTROY:
        {
            delete game;
            game = nullptr;

            DeleteObject(hBrushBg);
            DeleteObject(hBrushCard);
            DeleteObject(hBrushGreen);
            DeleteObject(hBrushTileEmpty);

            DeleteObject(hFontTitle);
            DeleteObject(hFontTile);
            DeleteObject(hFontStatus);
            DeleteObject(hFontButton);
            DeleteObject(hFontSmall);

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
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    hBrushBg        = CreateSolidBrush(COLOR_WORDLE_BG);
    hBrushCard      = CreateSolidBrush(COLOR_CARD_BG);
    hBrushGreen     = CreateSolidBrush(COLOR_WORDLE_GREEN);
    hBrushTileEmpty = CreateSolidBrush(COLOR_TILE_EMPTY);

    hFontTitle  = CreateFontA(-18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hFontTile   = CreateFontA(-32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hFontStatus = CreateFontA(-15, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hFontButton = CreateFontA(-16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hFontSmall  = CreateFontA(-13, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");

    const char CLASS_NAME[] = "WordleWordGuessWindow";

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
        "Word Guessing Game (Wordle Edition)",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        675, 465,
        nullptr, nullptr, hInstance, nullptr
    );

    if (hwnd == nullptr) return 0;
    hMainWnd = hwnd;

    startNewGame(0);

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