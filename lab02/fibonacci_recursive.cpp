#include <iostream>
using namespace std;

int fibonacciRecursive(int n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }

    return fibonacciRecursive(n - 1) + fibonacciRecursive(n - 2);
}

void printFibonacciRecursiveSequence(int terms) {
    cout << "Fibonacci Sequence (Recursive) up to " << terms << " terms:" << endl;

    for (int i = 0; i < terms; i++) {
        cout << fibonacciRecursive(i) << " ";
    }

    cout << endl;
}

int main() {
    int terms = 10;
    printFibonacciRecursiveSequence(terms);
    return 0;
}
