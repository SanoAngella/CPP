#include <iostream>
using namespace std;

double maximum(double x, double y, double z) {
    double maxValue = x;

    if (y > maxValue)
        maxValue = y;

    if (z > maxValue)
        maxValue = z;

    return maxValue;
}

int main() {
    double a, b, c;

    cout << "Enter three numbers: ";
    cin >> a >> b >> c;

    cout << "Maximum = " << maximum(a, b, c) << endl;

    return 0;
}
