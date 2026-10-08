#include <iostream>
#include <string>
using namespace std;

int main() {
    string firstName, lastName;
    int age;

    cout << "Enter first name: ";
    cin >> firstName;

    cout << "Enter last name: ";
    cin >> lastName;

    cout << "Enter age: ";
    cin >> age;

    cout << firstName << " " << lastName << " " << age << endl;
    return 0;
}
