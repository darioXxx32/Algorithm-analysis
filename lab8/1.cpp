#include <iostream>
using namespace std;

int main() {
    int u, v;

    cout << "Enter u: ";
    cin >> u;
    cout << "Enter v: ";
    cin >> v;

    cout << "\nBefore swap:" << endl;
    cout << "u = " << u << ", v = " << v << endl;

    // Swap without extra storage
    u = u + v;
    v = u - v;
    u = u - v;

    cout << "\nAfter swap:" << endl;
    cout << "u = " << u << ", v = " << v << endl;

    return 0;
}