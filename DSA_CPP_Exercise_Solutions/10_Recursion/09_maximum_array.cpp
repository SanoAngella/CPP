#include <iostream>
using namespace std;

int maximum(int arr[], int n) {
    if (n == 1)
        return arr[0];

    int smallMaximum = maximum(arr, n - 1);

    return (arr[n - 1] > smallMaximum) ? arr[n - 1] : smallMaximum;
}

int main() {
    int arr[] = {4, 9, 2, 15, 7, 3};

    cout << "Maximum = " << maximum(arr, 6) << endl;
    return 0;
}
