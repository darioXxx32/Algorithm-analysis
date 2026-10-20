#include <iostream>
using namespace std;

void generateFibonacciIterative(int n) {
    if (n <= 0) {
        return;
    }

    int prev2 = 0;
    int prev1 = 1;
    int current;

    cout << "Fibonacci Sequence (Iterative) up to " << n << " terms:" << endl;

    if (n >= 1) {
        cout << prev2 << " ";
    }
    if (n >= 2) {
        cout << prev1 << " ";
    }

    for (int i = 3; i <= n; i++) {
        current = prev1 + prev2;
        cout << current << " ";
        prev2 = prev1;
        prev1 = current;
    }

    cout << endl;
}

int main() {
    int terms = 10;
    generateFibonacciIterative(terms);
    return 0;
}
