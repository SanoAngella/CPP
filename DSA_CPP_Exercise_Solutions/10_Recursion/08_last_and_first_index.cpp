#include <iostream>
using namespace std;

int lastIndex(int arr[], int n, int element) {
    if (n == 0)
        return -1;

    if (arr[n - 1] == element)
        return n - 1;

    return lastIndex(arr, n - 1, element);
}

int firstIndex(int arr[], int n, int element, int index = 0) {
    if (index == n)
        return -1;

    if (arr[index] == element)
        return index;

    return firstIndex(arr, n, element, index + 1);
}

int main() {
    int arr[] = {5, 5, 6, 20, 5, 6};
    int n = 6;
    int x = 5;

    cout << "Last index of " << x << ": "
         << lastIndex(arr, n, x) << endl;

    cout << "First index of " << x << ": "
         << firstIndex(arr, n, x) << endl;

    return 0;
}
