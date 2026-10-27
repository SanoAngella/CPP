#include <iostream>
using namespace std;

// program that stores arrays of 10 integers

int main(){
    int arr[10];
    int i;
    cout << "Enter 10 integers: ";
    for(i=0; i<10; i++){
        cin >> arr[i];
    }
    cout << "The integers you entered are: ";
    for(i=0; i<10; i++){
        cout << arr[i] << " ";
    }
    return 0;
}