#include <iostream>
using namespace std;

// Template function for adding two values
template <typename T>
T addition(T a, T b)
{
    return a + b;
}

// Overloaded function for adding three integers
int addition(int a, int b, int c)
{
    return a + b + c;
}

// Overloaded function for adding three doubles
double addition(double a, double b, double c)
{
    return a + b + c;
}

int main()
{
    int x, y, z;

    cout << "Enter three integers: ";
    cin >> x >> y >> z;

    cout << "The sum of the three integers is: "
         << addition(x, y, z) << endl;

    double p, q, r;

    cout << "Enter three doubles: ";
    cin >> p >> q >> r;

    cout << "The sum of the three doubles is: "
         << addition(p, q, r) << endl;

    return 0;
}