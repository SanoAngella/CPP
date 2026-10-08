#include <iostream>
using namespace std;

void print(char input[]) {
    if (input[0] == '\0')
        return;

    cout << input[0];
    print(input + 1);
}

int main() {
    char input[100];

    cout << "Enter characters: ";
    cin >> input;

    print(input);
    cout << endl;

    return 0;
}
