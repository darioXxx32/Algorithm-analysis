#include <iostream>
#include <vector>
using namespace std;

// Función para mezclar dos mitades ordenadas
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;   // tamaño de la mitad izquierda
    int n2 = right - mid;      // tamaño de la mitad derecha

    // arreglos temporales
    vector<int> L(n1), R(n2);

    // copiar datos a los arreglos temporales
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // índices para recorrer L, R y arr
    int i = 0, j = 0, k = left;

    // mezclar mientras existan elementos en ambos arreglos
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // copiar elementos restantes de L, si hay
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // copiar elementos restantes de R, si hay
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Función recursiva de Merge Sort
void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // ordenar mitad izquierda
        mergeSort(arr, left, mid);

        // ordenar mitad derecha
        mergeSort(arr, mid + 1, right);

        // mezclar ambas mitades
        merge(arr, left, mid, right);
    }
}

// Función para imprimir el arreglo
void printArray(const vector<int>& arr) {
    for (int x : arr)
        cout << x << " ";
    cout << endl;
}

int main() {
    vector<int> arr = {38, 27, 43, 3, 9, 82, 10};

    cout << "Original array: ";
    printArray(arr);

    mergeSort(arr, 0, arr.size() - 1);

    cout << "Sorted array: ";
    printArray(arr);

    return 0;
}