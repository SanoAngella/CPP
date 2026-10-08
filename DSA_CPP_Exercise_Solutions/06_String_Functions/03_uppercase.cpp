#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int main() {
    string text;

    cout << "Enter a string: ";
    getline(cin, text);

    for (char &c : text) {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }

    cout << "Uppercase: " << text << endl;
    return 0;
}
