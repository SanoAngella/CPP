#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int main() {
    string text;
    string result;

    cout << "Enter a string: ";
    getline(cin, text);

    for (char c : text) {
        if (!isspace(static_cast<unsigned char>(c))) {
            result += c;
        }
    }

    cout << "Without spaces: " << result << endl;
    return 0;
}
