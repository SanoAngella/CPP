#include <iostream>
#include <string>
using namespace std;

int main() {
    string fullName;
    double initialAmount, rate, years;

    cout << "Enter full name: ";
    getline(cin, fullName);

    cout << "Enter initial amount: ";
    cin >> initialAmount;

    cout << "Enter interest rate per year (%): ";
    cin >> rate;

    cout << "Enter payment time in years: ";
    cin >> years;

    double interest = initialAmount * (rate / 100.0) * years;

    cout << "\nName: " << fullName << endl;
    cout << "Total interest: " << interest << endl;

    return 0;
}
