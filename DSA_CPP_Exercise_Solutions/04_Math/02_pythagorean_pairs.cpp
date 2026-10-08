#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int count = 0;

    for (int a = 2; a < 100; a++) {
        for (int b = a + 1; b < 100; b++) {
            double h = sqrt(pow(a, 2) + pow(b, 2));
            int integerH = static_cast<int>(h);

            if (integerH * integerH == a * a + b * b) {
                cout << "(" << a << ", " << b << ") -> " << integerH << endl;
                count++;
            }
        }
    }

    cout << "Number of pairs found: " << count << endl;
    return 0;
}
