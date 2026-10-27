#include <iostream>
using namespace std;

//fibonacci series

int fibonacci(int n){
    if (n<2){
        return n;
    }
    return fibonacci(n-1) + fibonacci(n-2);
}

int main(){
    cout << "Fibonacci of 10: " << fibonacci(10) << endl;
    return 0;
}