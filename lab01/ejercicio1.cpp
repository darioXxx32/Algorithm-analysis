#include <iostream>
using namespace std;

int euclid(int m, int n) {
    while (n != 0) {
        int r = m % n;
        m = n;
        n = r;
    }
    return m;
}

int main() {
    int m, n;
    cout << "Enter m: ";
    cin >> m;
    cout << "Enter n: ";
    cin >> n;
    cout << "gcd(" << m << ", " << n << ") = " << euclid(m, n) << endl;
    return 0;
}
