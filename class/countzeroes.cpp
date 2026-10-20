#include <iostream>
using namespace std;

// use pmi to count the number of zeroes in a number

int countZeroes(int n){
    //base
    if(n==0){
        return 0;
    }

    //recursive case
    int smallAns= countZeroes(n/10);

    //calculation
    int last_digit= n%10;

    if(last_digit==0){
        return smallAns+1;
    }
    else{
        return smallAns;
    }
}

int main(){
    int number;

    cout << "Enter a number: ";
    cin >> number;

    int result = countZeroes(number);
    cout << "The number of zeroes in " << number << " is: " << result << endl;

    return 0;
}