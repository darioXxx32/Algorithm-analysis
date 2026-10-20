#include <iostream>
#include <vector>
using namespace std;

// Imprimir arreglo
void printArray(const vector<int>& arr) {
    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl;
}

// Heapify para max-heap
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // Comparar con hijo izquierdo
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    // Comparar con hijo derecho
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // Si el mayor no es el padre, intercambiar y seguir
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// Construir max-heap
void buildHeap(vector<int>& arr) {
    int n = arr.size();

    // Empezar desde el último nodo interno hasta la raíz
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

int main() {
    vector<int> arr = {4, 10, 3, 5, 1};

    cout << "Original array: ";
    printArray(arr);

    buildHeap(arr);

    cout << "Array transformed into max-heap: ";
    printArray(arr);

    return 0;
}