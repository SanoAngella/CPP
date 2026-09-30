# DSA Scientific Desktop Calculator (C++)

A comprehensive, learning-focused scientific desktop calculator built in modern C++ with an explicit emphasis on **Data Structures and Algorithms (DSA)**.

The application features an ultra-responsive native Windows desktop graphical user interface (zero third-party dependencies), a custom dynamic-array-based **Stack**, a custom doubly-linked-list-based **HistoryList**, Dijkstra's **Shunting-Yard algorithm** for expression parsing, and a stack-based **Postfix Evaluator**.

---

## 1. Project Architecture & Structure

```text
assignment/
├── CMakeLists.txt                <- Cross-platform CMake build configuration
├── build.bat                     <- Automated build & test script for MinGW / GCC
├── README.md                     <- DSA & architectural guide (this file)
│
├── src/
│   ├── main.cpp                  <- Windows desktop GUI entry point (WinMain)
│   │
│   ├── core/                     <- Platform-independent calculation engine
│   │   ├── Token.h               <- Lexical token definitions (precedence & associativity)
│   │   ├── ExpressionParser.h    <- Shunting-Yard parser & postfix evaluator interface
│   │   ├── ExpressionParser.cpp  <- Tokenizer, Infix-to-Postfix & evaluator logic
│   │   ├── Calculator.h          <- High-level calculator facade & state manager
│   │   └── Calculator.cpp        <- Formatting, ANS recall, and history coordinator
│   │
│   ├── dsa/                      <- Custom Data Structures implemented from scratch
│   │   ├── Stack.h               <- Generic dynamic array Stack template declaration
│   │   ├── Stack.hpp             <- Stack template implementation with RAII
│   │   ├── HistoryList.h         <- Doubly Linked List for calculation history
│   │   └── HistoryList.cpp       <- HistoryList node management & traversal
│   │
│   └── ui/                       <- Native Windows Graphical User Interface
│       ├── MainWindow.h          <- Win32 window class declaration
│       └── MainWindow.cpp        <- Dark-mode styling, button grid & event handling
│
└── tests/
    └── test_calculator.cpp       <- Automated test suite verifying DSA & math engine
```

---

## 2. File Responsibilities

| File | Component | Responsibility |
| :--- | :--- | :--- |
| `src/dsa/Stack.h` & `Stack.hpp` | DSA Container | Custom generic Stack container using dynamic arrays. Provides $O(1)$ amortized `push()`, $O(1)$ `pop()`, `top()`, `isEmpty()`, `isFull()`, and `size()`. |
| `src/dsa/HistoryList.h` & `.cpp` | DSA Container | Custom Doubly Linked List storing calculation history. Provides $O(1)$ insertions and bidirectional traversal. |
| `src/core/Token.h` | Core Engine | Defines Token types (numbers, operators, functions, parentheses), precedence levels, and associativity. |
| `src/core/ExpressionParser.h` & `.cpp` | Core Engine | Tokenizes mathematical strings, executes Dijkstra's Shunting-Yard algorithm (Infix $\rightarrow$ Postfix), and evaluates Postfix tokens using custom Stacks. |
| `src/core/Calculator.h` & `.cpp` | Core Engine | Coordinates expression parsing, previous answer (`ANS`) substitution, number formatting, and history tracking. |
| `src/ui/MainWindow.h` & `.cpp` | Presentation | Native Win32 desktop window, owner-drawn dark theme buttons, display screen, keyboard input listener, and history dialog. |
| `src/main.cpp` | Entry Point | Win32 application initialization (`WinMain`) and standard Windows message loop. |
| `tests/test_calculator.cpp` | Quality Assurance | Automated test suite validating Stack invariants, operator precedence, parentheses, unary negatives, scientific functions, and error cases. |

---

## 3. UI ↔ C++ Engine Communication

To maintain clean separation of concerns, the Graphical User Interface (UI) does **not** perform any mathematical operations or parsing. It strictly serves as an interactive view layer:

```
[ User Action ] (Mouse click or Keyboard keystroke)
       │
       ▼
[ UI Layer (MainWindow) ]
  - Updates display buffer
  - Appends numbers, operators, or functions to expression string
  - User hits '=' or Enter
       │  Calls calculator.calculate(expression)
       ▼
[ Coordinator Layer (Calculator) ]
  - Normalizes input & replaces 'ANS' with previous result
  - Delegates mathematical resolution to ExpressionParser
       │
       ▼
[ Core Engine (ExpressionParser) ]
  - Step 1: Tokenizer splits string into structured Tokens
  - Step 2: Shunting-Yard converts Infix Tokens -> Postfix Tokens using Stack<Token>
  - Step 3: Postfix Evaluator computes numerical result using Stack<double>
       │
       ▼
[ CalculationResult Struct ]
  - success (bool)
  - value (double)
  - formattedResult (std::string, e.g. "14")
  - errorMessage (std::string, e.g. "Error: Cannot divide by zero")
       │
       ▼
[ UI Layer (MainWindow) ]
  - Displays formatted result on lower screen line
  - Or displays error message in red text without crashing
  - Stores result in calculation history
```

---

## 4. Custom Data Structures Explained

### A. Dynamic Array Stack (`Stack<T>`)
* **Concept:** A Stack is a linear LIFO (Last-In, First-Out) container. The last element pushed is the first to be popped.
* **Why used?** Expression evaluation requires backtracking and reversing order of operations:
  - In Shunting-Yard: The operator stack holds pending operators until their operands and precedence conditions are satisfied.
  - In Postfix Evaluation: The operand stack holds numbers until an operator is ready to consume them.
* **Why dynamic array instead of static array?** A static array has a fixed capacity that can overflow on complex nested expressions. The dynamic array automatically doubles its size upon reaching capacity, guaranteeing **amortized $O(1)$** push time while never overflowing.
* **Complexity:**
  - `push(item)`: $O(1)$ amortized time.
  - `pop()`: $O(1)$ time.
  - `top()`: $O(1)$ time.
  - `isEmpty()`, `isFull()`, `size()`: $O(1)$ time.
  - Space Complexity: $O(N)$ where $N$ is the capacity.

### B. Doubly Linked List (`HistoryList`)
* **Concept:** A sequential data structure where each node stores pointers to both its predecessor (`prev`) and successor (`next`).
* **Why used?** A calculator history requires frequent insertions of new calculations and fast iteration in both chronological and reverse-chronological order. Unlike an array, a linked list does not require expensive element shifts or reallocation copying when the list grows.
* **Complexity:**
  - `add(expr, res)`: $O(1)$ time (inserted at tail).
  - `removeOldest()`: $O(1)$ time (removed from head).
  - `clear()`: $O(N)$ time.
  - Space Complexity: $O(N)$ where $N$ is the number of saved calculations.

---

## 5. Expression Evaluation Algorithm

Evaluating expressions like `2 + 3 * 4` requires three distinct algorithmic phases:

### Phase 1: Tokenization (Lexical Analysis)
1. **What it does:** Scans the raw character stream and groups characters into typed `Token` objects (Numbers, Operators, Functions, Parentheses).
2. **Why it is needed:** Distinguishes multi-digit numbers (e.g. `123.45`), detects function names (`sqrt`, `sin`), and identifies whether a minus sign `-` is a binary subtraction (`5 - 3`) or a unary negative number (`-5 + 3` or `2 * -4`).
3. **How unary minus is handled:** A minus sign `-` is classified as unary (`NEG`) if:
   - It appears at the very beginning of the expression (`-5`).
   - It immediately follows an operator (`2 * -4`).
   - It immediately follows an opening parenthesis (`(-3)`).
4. **Complexity:** Time: $O(N)$ where $N$ is the character length. Space: $O(T)$ where $T$ is token count ($T \le N$).

### Phase 2: Shunting-Yard Algorithm (Infix to Postfix)
1. **What it does:** Reorders tokens from human-readable Infix notation (where operators sit between operands: `2 + 3 * 4`) into Postfix / Reverse Polish Notation (`2 3 4 * +`).
2. **Why it is needed:** Infix notation requires lookahead and parentheses to resolve precedence. Postfix notation is unambiguous and can be evaluated sequentially in a single pass without parentheses.
3. **How it works:**
   - Numbers are sent directly to the output.
   - Functions are pushed onto `opStack`.
   - Left parenthesis `(` is pushed onto `opStack`.
   - Right parenthesis `)` pops operators from `opStack` to output until `(` is reached.
   - Operators (`+`, `-`, `*`, `/`, `%`, `^`, `NEG`): While the top of `opStack` has greater precedence (or equal precedence and is left-associative), pop from `opStack` to output. Then push the current operator.
4. **Precedence Hierarchy:**
   1. Parentheses: `(` `)`
   2. Functions & Unary Negation: `sqrt`, `sin`, `cos`, `tan`, `log`, `ln`, `NEG` (Precedence 5/4, Right-associative)
   3. Exponentiation: `^` (Precedence 3, Right-associative: $2^{3^2} = 2^9 = 512$)
   4. Multiplication, Division, Modulo: `*`, `/`, `%` (Precedence 2, Left-associative)
   5. Addition, Subtraction: `+`, `-` (Precedence 1, Left-associative)
5. **Complexity:** Time: $O(T)$ where $T$ is token count. Space: $O(T)$ for output and stack.

### Phase 3: Postfix Evaluation
1. **What it does:** Computes the final numeric answer from the Postfix token stream using an operand stack (`Stack<double>`).
2. **Why it is needed:** Direct sequential evaluation on postfix requires zero backtracking.
3. **How it works:**
   - Scan tokens from left to right:
     - If number: push onto `valStack`.
     - If unary operator (`NEG`): pop 1 operand, negate, push result.
     - If function (`sqrt`, etc.): pop 1 operand, validate domain, compute, push result.
     - If binary operator (`+`, `*`, etc.): pop operand $b$, pop operand $a$, validate division/modulo by zero, compute $a \text{ op } b$, push result.
   - When finished, exactly one value remains on `valStack` — the final answer.
4. **Complexity:** Time: $O(T)$. Space: $O(T)$.

---

## 6. Step-by-Step Trace of `2 + 3 * 4`

Here is the exact trace demonstrating why the algorithm produces **`14`** instead of `20`:

### Step A: Shunting-Yard (Infix $\rightarrow$ Postfix)

| Step | Input Token | Action | Operator Stack (`opStack`) | Postfix Output |
| :---: | :---: | :--- | :---: | :--- |
| 1 | `2` | Number $\rightarrow$ append to output | `[ ]` | `2` |
| 2 | `+` | Operator (prec 1) $\rightarrow$ push to stack | `[ + ]` | `2` |
| 3 | `3` | Number $\rightarrow$ append to output | `[ + ]` | `2 3` |
| 4 | `*` | Operator (prec 2). Top is `+` (prec 1). Since $1 < 2$, no pop $\rightarrow$ push `*` | `[ +, * ]` | `2 3` |
| 5 | `4` | Number $\rightarrow$ append to output | `[ +, * ]` | `2 3 4` |
| 6 | *End* | Pop remaining operators: pop `*`, pop `+` | `[ ]` | `2 3 4 * +` |

Resulting Postfix sequence: **`2 3 4 * +`**

### Step B: Postfix Evaluation

| Step | Postfix Token | Action | Operand Stack (`valStack`) |
| :---: | :---: | :--- | :---: |
| 1 | `2` | Push `2` | `[ 2 ]` |
| 2 | `3` | Push `3` | `[ 2, 3 ]` |
| 3 | `4` | Push `4` | `[ 2, 3, 4 ]` |
| 4 | `*` | Pop $b = 4$, Pop $a = 3$. Compute $3 \times 4 = 12$. Push `12` | `[ 2, 12 ]` |
| 5 | `+` | Pop $b = 12$, Pop $a = 2$. Compute $2 + 12 = 14$. Push `14` | `[ 14 ]` |

Final Result: **`14`**.

### Why did it not produce 20?
In `2 + 3 * 4`, if evaluated strictly left-to-right without precedence, one would compute $(2 + 3) = 5$, then $5 \times 4 = 20$.
Because the `*` operator has higher precedence (2) than `+` (1), the Shunting-Yard algorithm deferred the addition by leaving `+` on the stack while pushing `*`. In the resulting postfix notation `2 3 4 * +`, the multiplication binds to `3` and `4` first ($3 \times 4 = 12$), and only then is the addition evaluated ($2 + 12 = 14$).

---

## 7. Complexity Analysis Summary

| Algorithm Component | Time Complexity | Space Complexity | Explanation |
| :--- | :---: | :---: | :--- |
| **Tokenization** | $O(N)$ | $O(N)$ | Single linear pass through the $N$-character string. |
| **Shunting-Yard Parsing** | $O(T)$ | $O(T)$ | Each token is pushed to and popped from the stack at most once ($T$ tokens). |
| **Postfix Evaluation** | $O(T)$ | $O(T)$ | Each token triggers an $O(1)$ arithmetic operation or stack manipulation. |
| **Total Pipeline** | **$O(N)$** | **$O(N)$** | Linear time and linear space proportional to expression length. |

---

## 8. Build and Run Guide

### Option 1: Using `build.bat` (Recommended for MinGW)
Double-click `build.bat` or run in terminal:
```cmd
cd assignments\calculator\assignment
build.bat
```
This script will:
1. Compile and execute `tests/test_calculator.cpp` (verifying all DSA tests pass).
2. Compile the native GUI application `calculator.exe`.
3. Launch `calculator.exe`.

### Option 2: Using VS Code Tasks
Open this folder in VS Code:
- Press `Ctrl + Shift + B` to execute **Build DSA Calculator GUI**.
- Or open Command Palette (`Ctrl + Shift + P`) $\rightarrow$ **Tasks: Run Task** $\rightarrow$ **Build and Run Calculator Tests**.

### Option 3: Manual Command Line Compilation
Compile and run the tests:
```cmd
C:\MinGW\bin\g++.exe -std=c++17 -O2 tests\test_calculator.cpp src\dsa\HistoryList.cpp src\core\ExpressionParser.cpp src\core\Calculator.cpp -Isrc -o run_tests.exe
run_tests.exe
```

Compile the desktop GUI:
```cmd
C:\MinGW\bin\g++.exe -std=c++17 -O2 src\main.cpp src\ui\MainWindow.cpp src\dsa\HistoryList.cpp src\core\ExpressionParser.cpp src\core\Calculator.cpp -Isrc -o calculator.exe -mwindows -lgdi32 -lcomctl32
calculator.exe
```

---

## 9. Keyboard Shortcuts

| Key | Calculator Action |
| :--- | :--- |
| `0` - `9` | Digits |
| `.` | Decimal point |
| `+`, `-`, `*`, `/` | Basic operators ($+$, $-$, $\times$, $\div$) |
| `%` | Modulo |
| `^` | Exponentiation / Power |
| `(` and `)` | Parentheses |
| `Enter` or `=` | Evaluate expression |
| `Backspace` | DEL (delete last character) |
| `Escape` | AC (All Clear) |
| `Delete` | C (Clear current expression) |
