#include <iostream>
using namespace std;

int convertStringToInt(char str[], int n) {
    if (n == 0)
        return 0;

    int smallAnswer = convertStringToInt(str, n - 1);
    int lastDigit = str[n - 1] - '0';

    return smallAnswer * 10 + lastDigit;
}

int main() {
    char str[100];

    cout << "Enter digits: ";
    cin >> str;

    int n = 0;
    while (str[n] != '\0')
        n++;

    cout << "Integer = " << convertStringToInt(str, n) << endl;
    return 0;
}
