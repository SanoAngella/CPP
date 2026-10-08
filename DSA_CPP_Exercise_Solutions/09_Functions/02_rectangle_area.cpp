#include <iostream>
using namespace std;

double rect_area(double length, double width) {
    return length * width;
}

int main() {
    double length, width;

    cout << "Enter length: ";
    cin >> length;

    cout << "Enter width: ";
    cin >> width;

    cout << "Area = " << rect_area(length, width) << endl;

    return 0;
}
