#include <iostream>
using namespace std;

// Selection Sort function
void selectionSort(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {

        int minIndex = i;

        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[minIndex]) {
                minIndex = j;
            }
        }

        // Swap
        int temp = a[i];
        a[i] = a[minIndex];
        a[minIndex] = temp;
    }
}

int main() {
    int n;

    cout << "Enter the number of elements: " << endl;
    cin >> n;

    int a[100];

    cout << "Enter " << n << " elements: " << endl;

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    selectionSort(a, n);

    cout << "Sorted array: ";

    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }

    return 0;
}