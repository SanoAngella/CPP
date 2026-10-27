#include <iostream>
using namespace std;

int main() {
    // 1-3. Initialize variables and the array 
    // (Using size 15 to safely accommodate shifting and printing up to len + 1)
    int scores[15] = {52, 78, 75, 68, 88, 63, 75, 90, 78};
    int item = 10;
    int k = 3;
    int len = 9;
    int j = len - 1; // j starts at 8

    // 4-6. Shift elements to the right from the end down to index k
    while (j >= k) {
        scores[j + 1] = scores[j];
        j = j - 1;
    }

    // 7. Insert the item at index k
    scores[k] = item;

    // 8-11. Print the array elements up to len + 1
    // (Note: Line 10 in your pseudocode "index=index=1" appears to be a typo 
    // for a manual increment like "index = index + 1")
    int index = 0;
    while (index <= len + 1) {
        cout << scores[index] << " ";
        index = index + 1; // Manual increment matching your pattern
    }
    cout << endl;

    return 0;
}