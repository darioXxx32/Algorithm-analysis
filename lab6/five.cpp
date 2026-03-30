#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chrono;

// Partición de Lomuto
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Quick Sort recursivo
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Generar lista aleatoria
vector<int> generateRandomList(int n, int minVal = 0, int maxVal = 1000000) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = minVal + rand() % (maxVal - minVal + 1);
    }
    return arr;
}

// Generar lista ordenada (para observar peor caso si se desea)
vector<int> generateSortedList(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
    return arr;
}

// Verifica si el arreglo quedó ordenado
bool isSorted(const vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i - 1] > arr[i]) return false;
    }
    return true;
}

int main() {
    srand((unsigned)time(0));

    vector<int> sizes = {1000, 5000, 10000, 20000, 50000};
    int repetitions = 5;

    cout << fixed << setprecision(3);
    cout << "EMPIRICAL ANALYSIS OF QUICK SORT\n";
    cout << "Random generated lists\n\n";
    cout << "n\tAverage Time (ms)\n";
    cout << "---------------------------\n";

    for (int n : sizes) {
        double totalTime = 0.0;

        for (int r = 0; r < repetitions; r++) {
            vector<int> arr = generateRandomList(n);

            auto start = high_resolution_clock::now();
            quickSort(arr, 0, arr.size() - 1);
            auto end = high_resolution_clock::now();

            double elapsed = duration<double, milli>(end - start).count();
            totalTime += elapsed;

            if (!isSorted(arr)) {
                cout << "Error: array was not sorted correctly.\n";
                return 1;
            }
        }

        double averageTime = totalTime / repetitions;
        cout << n << "\t" << averageTime << "\n";
    }

    return 0;
}