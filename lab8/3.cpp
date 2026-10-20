#include <iostream>
using namespace std;

void printArray(int A[], int n) {
    for (int i = 0; i < n; i++) {
        cout << A[i] << " ";
    }
    cout << endl;
}

void DistributionCountingSort(int A[], int n) {
    if (n <= 1) return;

    // Find minimum and maximum values
    int minVal = A[0], maxVal = A[0];
    for (int i = 1; i < n; i++) {
        if (A[i] < minVal) minVal = A[i];
        if (A[i] > maxVal) maxVal = A[i];
    }

    int range = maxVal - minVal + 1;

    // Count array
    int* count = new int[range];
    for (int i = 0; i < range; i++) {
        count[i] = 0;
    }

    // Count occurrences
    for (int i = 0; i < n; i++) {
        count[A[i] - minVal]++;
    }

    // Prefix sums
    for (int i = 1; i < range; i++) {
        count[i] += count[i - 1];
    }

    // Output array
    int* output = new int[n];

    // Build sorted array from right to left (stable)
    for (int i = n - 1; i >= 0; i--) {
        output[count[A[i] - minVal] - 1] = A[i];
        count[A[i] - minVal]--;
    }

    // Copy back
    for (int i = 0; i < n; i++) {
        A[i] = output[i];
    }

    delete[] count;
    delete[] output;
}

int main() {
    int A[] = {62, 31, 84, 96, 19, 47, 31, 19, 84};
    int n = sizeof(A) / sizeof(A[0]);

    cout << "Original array: ";
    printArray(A, n);

    DistributionCountingSort(A, n);

    cout << "Sorted array:   ";
    printArray(A, n);

    return 0;
}