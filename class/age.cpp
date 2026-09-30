#include <iostream>
#include <stdio.h>
using namespace std;

int main(){
    int age;

   // printf("Enter your age: "); 
   cout << "Enter your age: \n";
   //>> is extraction operator

   //scanf("%d", &age);
    cin >> age;

    // << is insertion operator
    cout << "Your age is: " << age<< "years old"<< endl;
    return 0;
}