#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

long long hanoi(int n, char src, char aux, char dst) {
    if (n == 1) {
        cout << "  Move disk 1 : " << src << " -> " << dst << "\n";
        return 1;
    }
    long long moves = hanoi(n - 1, src, dst, aux);
    cout << "  Move disk " << n << " : " << src << " -> " << dst << "\n";
    moves += 1;
    moves += hanoi(n - 1, aux, src, dst);
    return moves;
}

long long count_moves(int n) {
    if (n == 1) return 1;
    return 2LL * count_moves(n - 1) + 1;
}

int main() {
    cout << "Towers of Hanoi - Recursive | Yachay Tech | Dario Pomasqui\n\n";

    const int N = 3;
    cout << "n = " << N << " disks  (A -> C via B)\n";
    cout << "Move sequence:\n";
    long long total = hanoi(N, 'A', 'B', 'C');
    cout << "\nTotal moves: " << total
         << "  (expected 2^" << N << " - 1 = " << ((1LL << N) - 1) << ")\n\n";

    cout << left << setw(5) << "n"
                 << setw(14) << "2^n - 1"
                 << setw(14) << "Recursive"
                 << "Match?\n";
    cout << string(40, '-') << "\n";

    for (int n = 1; n <= 20; n++) {
        long long expected = (1LL << n) - 1;
        long long result   = count_moves(n);
        cout << left << setw(5)  << n
                     << setw(14) << expected
                     << setw(14) << result
                     << ((result == expected) ? "YES" : "NO") << "\n";
    }

    cout << "\nT(n) = 2T(n-1) + 1  =>  T(n) = Theta(2^n)\n";
    cout << "Space: O(n) call stack\n";

    return 0;
}
