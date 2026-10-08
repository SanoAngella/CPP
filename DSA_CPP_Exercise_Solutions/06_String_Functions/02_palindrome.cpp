#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

bool palindromeWithoutBuiltIn(string text) {
    int left = 0;
    int right = static_cast<int>(text.length()) - 1;

    while (left < right) {
        if (text[left] != text[right]) {
            return false;
        }
        left++;
        right--;
    }

    return true;
}

bool palindromeWithBuiltIn(string text) {
    string reversed = text;
    reverse(reversed.begin(), reversed.end());
    return text == reversed;
}

int main() {
    string text;

    cout << "Enter a string: ";
    getline(cin, text);

    cout << "Without built-in reverse: "
         << (palindromeWithoutBuiltIn(text) ? "Palindrome" : "Not palindrome")
         << endl;

    cout << "With built-in reverse: "
         << (palindromeWithBuiltIn(text) ? "Palindrome" : "Not palindrome")
         << endl;

    return 0;
}
