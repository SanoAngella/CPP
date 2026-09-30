#include <iostream>

using namespace std;

int main() {
    int numbers[5]= {10,20,30,40};
    int size = 4;
    int position= 2;
    int value = 25;

    for (int i= size; i>position; i--){
        numbers[i] = numbers[i-1];
    }

    numbers[position] = value;
    size++;

    for (int i =0; i<size; i++){
        cout <<numbers[i] <<" ";
    }

    return 0;
}