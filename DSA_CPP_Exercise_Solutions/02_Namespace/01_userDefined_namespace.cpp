#include <iostream>
using namespace std;

namespace userDefined {
    int insideNamespace = 10;

    int cout() {
        return insideNamespace;
    }
}

int myGlobal = 5;

int cout() {
    return myGlobal * myGlobal;
}

int main() {
    int cout = 20;

    cout << "The local variable cout in main is " << cout << endl;
    cout << "The variable in userDefined namespace is "
         << userDefined::insideNamespace << endl;
    cout << "The output of cout() in userDefined is "
         << userDefined::cout() << endl;
    cout << "The value of myGlobal is " << ::myGlobal << endl;
    cout << "The output of global cout() is " << ::cout() << endl;

    return 0;
}
