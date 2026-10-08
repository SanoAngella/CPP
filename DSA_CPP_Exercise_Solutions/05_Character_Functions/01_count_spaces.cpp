#include <iostream>
#include <string>
using namespace std;

int main() {
    string text;
    int spaces = 0;

    cout << "Enter a string: ";
    getline(cin, text);

    for (char c : text) {
        if (c == ' ') {
            spaces++;
        }
    }

    cout << "Number of spaces: " << spaces << endl;
    return 0;
}
