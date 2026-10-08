#include <iostream>
#include <iomanip>
using namespace std;

const double PI = 3.14159265;

double circleArea(double radius) {
    return PI * radius * radius;
}

double circumference(double radius) {
    return 2 * PI * radius;
}

int main() {
    double radius;

    cout << "Enter radius: ";
    cin >> radius;

    cout << fixed << setprecision(2);
    cout << "Area = " << circleArea(radius) << endl;
    cout << "Circumference = " << circumference(radius) << endl;

    return 0;
}
