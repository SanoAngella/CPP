#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int main() {
    string text;
    int withoutSpaces = 0;
    int words = 0;
    bool insideWord = false;

    cout << "Enter a string: ";
    getline(cin, text);

    for (char c : text) {
        if (!isspace(static_cast<unsigned char>(c))) {
            withoutSpaces++;
        }

        if (!isspace(static_cast<unsigned char>(c))) {
            if (!insideWord) {
                words++;
                insideWord = true;
            }
        } else {
            insideWord = false;
        }
    }

    cout << "Characters without spaces: " << withoutSpaces << endl;
    cout << "Characters with spaces: " << text.length() << endl;
    cout << "Words: " << words << endl;

    return 0;
}
