#include <iostream>

using namespace std;
namespace myCustomNamespace {
    int value= 30;
    int square(){
        return value*value;
    }
}

int value= 21;

int main() {
    int value =20;
    // double value = 20.5; it won't work

    //:: scope resolution operator

    cout << "The local variable is " <<value<<endl;
    cout << "The global variable is " <<::value<<endl;
    cout << "The value in my custom namespace is " <<myCustomNamespace::value<<endl;
    cout << "The squared value in my custom namespace is " <<myCustomNamespace::square()<<endl;

    return 0;
}