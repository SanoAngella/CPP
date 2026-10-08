#include <iostream>
#include <cctype>
using namespace std;

int main() {
    char letter;

    cout << "Enter a character: ";
    cin >> letter;

    cout << "Lowercase: " << static_cast<char>(tolower(letter)) << endl;
    return 0;
}
