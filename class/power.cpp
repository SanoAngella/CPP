#include <iostream>

using namespace std;

// Function to calculate power of a number
// base case

int powerNumber(int x, int n){
    if (n == 0) return 1; // Base case: any number raised to the power of 0 is 1
    if (n == 1) return x; // Base case: any number raised to the power of 1 is itself

    int smallerAnswer = powerNumber(x, n - 1); // Recursive call
    return x * smallerAnswer; // Multiply current number with result of (x, n-1)

}

int main(){
    int base, exponent;

    cout << "Enter base and exponent: ";
    cin >> base >> exponent;

    int result = powerNumber(base, exponent);
    cout << base << " raised to the power of " << exponent << " is: " << result << endl;

    return 0;
}