# DSA C++ — Exercise Solutions

These solutions are based on the exercises explicitly contained in the
provided DSA_C++ @damascene10 PDF.

## Exercise inventory

1. User input:
   - first name, last name, age
   - full name, initial amount, yearly interest rate, years

2. Namespace:
   - `userDefined` namespace, namespace/global/local `cout`

3. Constants:
   - circle area and circumference using PI and a function

4. Math/functions:
   - hypotenuse using `sqrt()` and `pow()`
   - integer Pythagorean pairs
   - quadratic roots
   - Rock Paper Scissors using ternary operator

5. Character functions:
   - lowercase conversion activity
   - count spaces
   - digit or alphabet
   - word count
   - reverse string

6. String functions:
   - length without built-in functions
   - palindrome with and without built-in reverse
   - uppercase conversion
   - remove spaces

7. Word Guess Game:
   - categories
   - random word
   - hidden letters
   - limited attempts
   - `exit`
   - replay

8. GUI Calculator:
   - the PDF asks for +, -, *, / and a result textbox.
   - Our previously built calculator exceeds this specification.

9. User-defined functions:
   - maximum of three numbers
   - rectangle area

10. Recursion:
   - print character array
   - reverse character array
   - remove character
   - replace character
   - character-array length
   - string digits to integer
   - remove consecutive duplicates
   - last index
   - first index
   - maximum array element
   - Sudoku solver

## Important source limitation

The PDF contains links to external OOP and pointer exercises but does not
contain their actual questions. It also refers to a separate file for
"Exercises on Functions"; that file is not embedded in the PDF.

Those external exercises are intentionally not invented here.

## Compile

For a normal exercise:

g++ filename.cpp -o program.exe
.\program.exe

For the GUI calculator from the earlier project:

g++ main.cpp Stack.cpp Calculator.cpp -o calculator.exe -mwindows
