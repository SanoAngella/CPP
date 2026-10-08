#include <iostream>
#include <cctype>
using namespace std;

int main() {
    char c;

    cout << "Enter a character: ";
    cin >> c;

    if (isdigit(static_cast<unsigned char>(c))) {
        cout << "It is a digit." << endl;
    }
    else if (isalpha(static_cast<unsigned char>(c))) {
        cout << "It is an alphabet letter." << endl;
    }
    else {
        cout << "It is neither a digit nor an alphabet letter." << endl;
    }

    return 0;
}
