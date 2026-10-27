#include <iostream>
using namespace std;

int main() {
    // SET scores[9] = {...} and SET len = 9
    int scores[9] = {52, 78, 75, 68, 88, 63, 75, 90, 78};
    int len = 9;
    
    int index = 0;
    while (index < len) {
        cout << scores[index] << " ";
        
        // index = index + 1
        index = index + 1;
    }
    cout << endl;

    return 0;
}