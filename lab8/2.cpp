#include <iostream>
using namespace std;

void comparisonCountingSort(int A[], int n) {
    int *count = new int[n];
    int *S = new int[n];

    // Inicializar contadores en 0
    for (int i = 0; i < n; i++) {
        count[i] = 0;
    }

    // Comparar cada par de elementos
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (A[i] < A[j]) {
                count[j]++;
            } else {
                count[i]++;
            }
        }
    }

    // Colocar cada elemento en su posición final
    for (int i = 0; i < n; i++) {
        S[count[i]] = A[i];
    }

    // Copiar al arreglo original
    for (int i = 0; i < n; i++) {
        A[i] = S[i];
    }

    delete[] count;
    delete[] S;
}

void printArray(int A[], int n) {
    for (int i = 0; i < n; i++) {
        cout << A[i] << " ";
    }
    cout << endl;
}

int main() {
    int A[] = {62, 31, 84, 96, 19, 47};
    int n = sizeof(A) / sizeof(A[0]);

    cout << "Original array: ";
    printArray(A, n);

    comparisonCountingSort(A, n);

    cout << "Sorted array:   ";
    printArray(A, n);

    return 0;
}