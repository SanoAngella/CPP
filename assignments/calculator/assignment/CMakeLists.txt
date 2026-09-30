cmake_minimum_required(VERSION 3.10)
project(DsaCalculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(src)

# Core engine and DSA sources
set(CORE_SOURCES
    src/dsa/HistoryList.cpp
    src/core/ExpressionParser.cpp
    src/core/Calculator.cpp
)

# Desktop GUI Target
if(WIN32)
    add_executable(calculator
        WIN32
        src/main.cpp
        src/ui/MainWindow.cpp
        ${CORE_SOURCES}
    )
    target_link_libraries(calculator PRIVATE gdi32 comctl32)
endif()

# Automated Test Target
add_executable(run_tests
    tests/test_calculator.cpp
    ${CORE_SOURCES}
)
