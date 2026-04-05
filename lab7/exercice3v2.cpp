#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

void printArray(const vector<int>& arr) {
    for (int x : arr) cout << x << " ";
    cout << endl;
}

void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void buildHeap(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

vector<int> generateRandomArray(int n, int minVal = 0, int maxVal = 99) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = minVal + rand() % (maxVal - minVal + 1);
    }
    return arr;
}

int main() {
    srand(time(0));

    vector<int> arr = generateRandomArray(10);

    cout << "Original random array: ";
    printArray(arr);

    buildHeap(arr);

    cout << "Array transformed into max-heap: ";
    printArray(arr);

    return 0;
}