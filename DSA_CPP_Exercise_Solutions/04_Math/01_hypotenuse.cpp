#include <iostream>
#include <cmath>
using namespace std;

int main() {
    double a, b;

    cout << "Enter side a: ";
    cin >> a;

    cout << "Enter side b: ";
    cin >> b;

    double h = sqrt(pow(a, 2) + pow(b, 2));

    cout << "Hypotenuse = " << h << endl;
    return 0;
}
