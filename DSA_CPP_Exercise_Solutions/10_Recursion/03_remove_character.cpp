#include <iostream>
using namespace std;

void removeCharacter(char input[], char target, int index = 0) {
    if (input[index] == '\0')
        return;

    if (input[index] == target) {
        int j = index;
        while (input[j] != '\0') {
            input[j] = input[j + 1];
            j++;
        }
        removeCharacter(input, target, index);
    } else {
        removeCharacter(input, target, index + 1);
    }
}

int main() {
    char input[100];
    char target;

    cout << "Enter string: ";
    cin >> input;

    cout << "Character to remove: ";
    cin >> target;

    removeCharacter(input, target);

    cout << "Result: " << input << endl;
    return 0;
}
