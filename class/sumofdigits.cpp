#include <iostream>
using namespace std;

//sum of digits of a number

int sumDigits(int n){
    //base case
    if(n%10==n) return n; //if n is a single digit number, return n

    //recursive case
    return (n % 10) + sumDigits(n / 10);
}
