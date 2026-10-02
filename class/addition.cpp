#include <iostream>
using namespace std;
template<Typename T>
T addition(T a, T b){
    return a+b;
}

//overload addition add integer and double
int addition(int a, int b, int c) {
    return a + b + c;
}

int addition(double a, double b, double c) {
    freturn a + b + c;
}

int main() {
   int x, y, z;
   cout << "Enter three integers: ";
   cin >> x >> y>> z;
   cout << "The sum of the three integers is: " << addition<int>(x, y, z) << endl;

double p, q, r;
   cout << "Enter three doubles: ";
   cin >> p >> q >> r;
   cout << "The sum of the three doubles is: " << addition<double>(p, q, r) << endl;
}