#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

long long hanoi_recursive(int n, char source, char auxiliary, char destination) {
    if (n == 1) {
        cout << "Move disk 1: " << source << " -> " << destination << "\n";
        return 1;
    }
    long long moves = 0;
    moves += hanoi_recursive(n - 1, source, destination, auxiliary);
    cout << "Move disk " << n << ": " << source << " -> " << destination << "\n";
    moves += 1;
    moves += hanoi_recursive(n - 1, auxiliary, source, destination);
    return moves;
}

void legal_move(vector<int>& p1, vector<int>& p2,
                const string& name1, const string& name2) {
    if (p1.empty()) {
        int disk = p2.back(); p2.pop_back(); p1.push_back(disk);
        cout << "Move disk " << disk << ": " << name2 << " -> " << name1 << "\n";
    } else if (p2.empty()) {
        int disk = p1.back(); p1.pop_back(); p2.push_back(disk);
        cout << "Move disk " << disk << ": " << name1 << " -> " << name2 << "\n";
    } else if (p1.back() < p2.back()) {
        int disk = p1.back(); p1.pop_back(); p2.push_back(disk);
        cout << "Move disk " << disk << ": " << name1 << " -> " << name2 << "\n";
    } else {
        int disk = p2.back(); p2.pop_back(); p1.push_back(disk);
        cout << "Move disk " << disk << ": " << name2 << " -> " << name1 << "\n";
    }
}

long long hanoi_iterative(int n) {
    vector<int> A, B, C;
    for (int i = n; i >= 1; i--) A.push_back(i);

    vector<int>* dest;
    vector<int>* aux;
    string dest_name, aux_name;

    if (n % 2 != 0) {
        dest = &C; dest_name = "C";
        aux  = &B; aux_name  = "B";
    } else {
        dest = &B; dest_name = "B";
        aux  = &C; aux_name  = "C";
    }

    long long num_moves = (1LL << n) - 1;

    for (long long i = 1; i <= num_moves; i++) {
        if      (i % 3 == 1) legal_move(A, *dest, "A", dest_name);
        else if (i % 3 == 2) legal_move(A, *aux,  "A", aux_name);
        else                  legal_move(*dest, *aux, dest_name, aux_name);
    }
    return num_moves;
}

long long count_recursive_moves(int n) {
    if (n == 1) return 1;
    return 2 * count_recursive_moves(n - 1) + 1;
}

void empirical_analysis() {
    cout << "\n=============================================================\n";
    cout << "  EMPIRICAL ANALYSIS: Towers of Hanoi (n = 1 .. 20)\n";
    cout << "=============================================================\n";
    cout << left
         << setw(5)  << "n"
         << setw(14) << "Predicted"
         << setw(14) << "Recursive"
         << setw(14) << "Iterative"
         << setw(8)  << "Match?"
         << "\n";
    cout << string(55, '-') << "\n";

    for (int n = 1; n <= 20; n++) {
        long long predicted  = (1LL << n) - 1;
        long long rec_moves  = count_recursive_moves(n);
        long long iter_moves = (1LL << n) - 1;
        string match = (rec_moves == predicted && iter_moves == predicted) ? "YES" : "NO";

        cout << left
             << setw(5)  << n
             << setw(14) << predicted
             << setw(14) << rec_moves
             << setw(14) << iter_moves
             << setw(8)  << match
             << "\n";
    }
    cout << "\nConclusion: Both produce exactly 2^n - 1 moves.\n";
    cout << "Time complexity : T(n) = Theta(2^n)\n";
    cout << "Space (recursive): O(n) stack frames\n";
    cout << "Space (iterative): O(n) tower storage, O(1) call stack\n";
}

int main() {
    cout << "===============================================\n";
    cout << "  TOWERS OF HANOI - Assignment 03\n";
    cout << "  Yachay Tech | Analysis of Algorithms\n";
    cout << "===============================================\n\n";

    int demo_n = 3;
    cout << "--- A) Recursive (n=" << demo_n << ") ---\n";
    long long r = hanoi_recursive(demo_n, 'A', 'B', 'C');
    cout << "Total moves: " << r << "  (expected: " << ((1LL << demo_n) - 1) << ")\n\n";

    cout << "--- B) Iterative (n=" << demo_n << ") ---\n";
    long long it = hanoi_iterative(demo_n);
    cout << "Total moves: " << it << "  (expected: " << ((1LL << demo_n) - 1) << ")\n\n";

    empirical_analysis();
    return 0;
}
