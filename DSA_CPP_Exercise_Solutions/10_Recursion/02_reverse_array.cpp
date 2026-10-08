#include <iostream>
using namespace std;

void reversePrint(char input[]) {
    if (input[0] == '\0')
        return;

    reversePrint(input + 1);
    cout << input[0];
}

int main() {
    char input[100];

    cout << "Enter characters: ";
    cin >> input;

    reversePrint(input);
    cout << endl;

    return 0;
}
