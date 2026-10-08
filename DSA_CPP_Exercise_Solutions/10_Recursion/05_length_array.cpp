#include <iostream>
using namespace std;

int length(char input[]) {
    if (input[0] == '\0')
        return 0;

    return 1 + length(input + 1);
}

int main() {
    char input[100];

    cout << "Enter string: ";
    cin >> input;

    cout << "Length = " << length(input) << endl;
    return 0;
}
