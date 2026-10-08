#include <iostream>
using namespace std;

void replaceCharacter(char input[], char oldChar, char newChar) {
    if (input[0] == '\0')
        return;

    if (input[0] == oldChar)
        input[0] = newChar;

    replaceCharacter(input + 1, oldChar, newChar);
}

int main() {
    char input[100];
    char oldChar, newChar;

    cout << "Enter string: ";
    cin >> input;

    cout << "Character to replace: ";
    cin >> oldChar;

    cout << "Replacement character: ";
    cin >> newChar;

    replaceCharacter(input, oldChar, newChar);

    cout << "Result: " << input << endl;
    return 0;
}
