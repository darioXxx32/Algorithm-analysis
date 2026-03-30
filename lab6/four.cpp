#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// Imprime el arreglo
void printArray(const vector<int>& arr) {
    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl;
}

// Función de partición (Lomuto)
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];   // pivote = último elemento
    int i = low - 1;         // índice del último elemento menor o igual al pivote

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1; // posición final del pivote
}

// Quick Sort recursivo
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        // ordenar subarreglo izquierdo
        quickSort(arr, low, pi - 1);

        // ordenar subarreglo derecho
        quickSort(arr, pi + 1, high);
    }
}

// Generar lista aleatoria
vector<int> generateRandomList(int n, int minVal = 0, int maxVal = 99) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = minVal + rand() % (maxVal - minVal + 1);
    }
    return arr;
}

int main() {
    srand(time(0));

    // generar lista aleatoria de tamaño 10
    vector<int> arr = generateRandomList(10);

    cout << "Original array: ";
    printArray(arr);

    quickSort(arr, 0, arr.size() - 1);

    cout << "Sorted array: ";
    printArray(arr);

    return 0;
}