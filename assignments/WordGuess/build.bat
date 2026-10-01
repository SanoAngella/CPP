@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"

echo ==========================================================
echo    WORD GUESSING GAME - BUILD SCRIPT
echo ==========================================================

:: Detect C++ Compiler
set GXX=g++
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    if exist "C:\MinGW\bin\g++.exe" (
        set GXX=C:\MinGW\bin\g++.exe
    ) else (
        echo [ERROR] g++ compiler not found in PATH or at C:\MinGW\bin\g++.exe
        pause
        exit /b 1
    )
)

echo [INFO] Using Compiler: %GXX%

:: Compile GUI Application
echo.
echo Compiling Desktop GUI Application (wordguess.exe)...
%GXX% -std=c++17 -O2 GUI.cpp WordGame.cpp GuessList.cpp -o wordguess.exe -mwindows -lgdi32
if %errorlevel% neq 0 (
    echo [ERROR] Desktop GUI compilation failed!
    pause
    exit /b 1
)

echo [INFO] Compilation successful -> wordguess.exe
echo.
echo Launching wordguess.exe...
start "" wordguess.exe
