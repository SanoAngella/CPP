#include <iostream>
using namespace std;

// Function to calculate factorial of a number

int factorial(int n){
    if (n<0) return -1; // Factorial is not defined for negative numbers
    if (n==0 || n==1) return 1; // Base case: factorial of 0 and 1 is 1

    int smallerAnswer = factorial(n-1); // Recursive call
    return n * smallerAnswer; // Multiply current number with factorial of (n-1)
}

int main(){
    int number;

    cout << "Enter a non-negative integer: ";
    cin >> number;

    int result = factorial(number);
    if (result == -1) {
        cout << "Factorial is not defined for negative numbers." << endl;
    } else {
        cout << "Factorial of " << number << " is: " << result << endl;
    }
}
