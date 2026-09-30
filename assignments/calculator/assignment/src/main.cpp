#include "ui/MainWindow.h"
#include <windows.h>
#include <commctrl.h>

/**
 * @brief Application Entry Point for Windows Desktop GUI.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize common control library
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icex);

    MainWindow app;
    if (!app.registerAndCreate(hInstance, nCmdShow)) {
        MessageBoxA(nullptr, "Failed to create Calculator window.", "Initialization Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    return app.runMessageLoop();
}

/**
 * @brief Fallback standard main entry point for console builds or test wrappers.
 */
int main(int argc, char* argv[]) {
    return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_SHOWDEFAULT);
}
