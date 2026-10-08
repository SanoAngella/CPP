#include <iostream>
using namespace std;

void removeConsecutiveDuplicates(char input[]) {
    if (input[0] == '\0' || input[1] == '\0')
        return;

    if (input[0] == input[1]) {
        int i = 0;

        while (input[i] != '\0') {
            input[i] = input[i + 1];
            i++;
        }

        removeConsecutiveDuplicates(input);
    } else {
        removeConsecutiveDuplicates(input + 1);
    }
}

int main() {
    char input[100];

    cout << "Enter string: ";
    cin >> input;

    removeConsecutiveDuplicates(input);

    cout << "Result: " << input << endl;
    return 0;
}
