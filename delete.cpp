#include <iostream>

using namespace std;

int main() {
    int numbers[5]= {10,20,30,40,50};
    int size = 5;
    int position = 2;

    for (int i=position; i<size-1; i++){
        numbers[i] = numbers[i+1];
    }
    size--;

    for (int i =0; i<size; i++){
        cout <<numbers[i] <<" ";
    }

    return 0;

}