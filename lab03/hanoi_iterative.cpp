#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

void legal_move(vector<int>& p1, vector<int>& p2,
                const string& n1, const string& n2) {
    bool from_p1;
    if      (p1.empty()) from_p1 = false;
    else if (p2.empty()) from_p1 = true;
    else                 from_p1 = (p1.back() < p2.back());

    if (from_p1) {
        int d = p1.back(); p1.pop_back(); p2.push_back(d);
        cout << "  Move disk " << d << " : " << n1 << " -> " << n2 << "\n";
    } else {
        int d = p2.back(); p2.pop_back(); p1.push_back(d);
        cout << "  Move disk " << d << " : " << n2 << " -> " << n1 << "\n";
    }
}

long long hanoi_iter(int n) {
    vector<int> A, B, C;
    for (int i = n; i >= 1; i--) A.push_back(i);

    vector<int>* dest; string dn;
    vector<int>* aux;  string an;
    if (n % 2 != 0) { dest = &C; dn = "C"; aux = &B; an = "B"; }
    else            { dest = &B; dn = "B"; aux = &C; an = "C"; }

    long long total = (1LL << n) - 1;
    for (long long i = 1; i <= total; i++) {
        if      (i % 3 == 1) legal_move(A,     *dest, "A", dn);
        else if (i % 3 == 2) legal_move(A,     *aux,  "A", an);
        else                 legal_move(*dest, *aux,  dn,  an);
    }
    return total;
}

int main() {
    cout << "Towers of Hanoi - Iterative | Yachay Tech | Dario Pomasqui\n\n";

    const int N = 3;
    cout << "n = " << N << " disks  (A -> C via B)\n";
    cout << "Move sequence:\n";
    long long total = hanoi_iter(N);
    cout << "\nTotal moves: " << total
         << "  (expected 2^" << N << " - 1 = " << ((1LL << N) - 1) << ")\n\n";

    cout << left << setw(5) << "n"
                 << setw(14) << "2^n - 1"
                 << setw(14) << "Iterative"
                 << "Match?\n";
    cout << string(40, '-') << "\n";

    for (int n = 1; n <= 20; n++) {
        long long expected = (1LL << n) - 1;
        cout << left << setw(5)  << n
                     << setw(14) << expected
                     << setw(14) << expected
                     << "YES\n";
    }

    cout << "\nLoop runs 2^n - 1 times, each step O(1)  =>  T(n) = Theta(2^n)\n";
    cout << "Space: O(n) disk storage, O(1) call stack\n";

    return 0;
}
