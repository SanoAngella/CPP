@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"

echo ==========================================================
echo    DSA CALCULATOR - AUTOMATED BUILD ^& TEST SCRIPT
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

:: 1. Compile and Run Unit Tests
echo.
echo [1/2] Compiling Unit Test Suite (tests/test_calculator.cpp)...
%GXX% -std=c++17 -O2 tests\test_calculator.cpp src\dsa\HistoryList.cpp src\core\ExpressionParser.cpp src\core\Calculator.cpp -Isrc -o run_tests.exe
if %errorlevel% neq 0 (
    echo [ERROR] Test suite compilation failed!
    pause
    exit /b 1
)
echo [INFO] Test suite compiled successfully -> run_tests.exe

echo.
echo [INFO] Executing Unit Tests...
run_tests.exe
if %errorlevel% neq 0 (
    echo [ERROR] Unit tests failed!
    pause
    exit /b 1
)

:: 2. Compile Desktop GUI Application
echo.
echo [2/2] Compiling Desktop GUI Application (calculator.exe)...
%GXX% -std=c++17 -O2 src\main.cpp src\ui\MainWindow.cpp src\dsa\HistoryList.cpp src\core\ExpressionParser.cpp src\core\Calculator.cpp -Isrc -o calculator.exe -mwindows -lgdi32 -lcomctl32
if %errorlevel% neq 0 (
    echo [ERROR] Desktop GUI compilation failed!
    pause
    exit /b 1
)
echo [INFO] Desktop GUI compiled successfully -> calculator.exe

echo.
echo ==========================================================
echo    BUILD COMPLETE!
echo    - run_tests.exe  (automated DSA test runner)
echo    - calculator.exe (modern native desktop GUI)
echo ==========================================================
echo.
echo Launching calculator.exe...
start "" calculator.exe
pause
