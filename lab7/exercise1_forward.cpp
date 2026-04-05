#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// Imprimir matriz aumentada
void printMatrix(const vector<vector<double>>& A) {
    for (const auto& row : A) {
        for (double val : row) {
            cout << setw(10) << fixed << setprecision(2) << val << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Forward elimination
void forwardElimination(vector<vector<double>>& A) {
    int n = A.size();

    for (int i = 0; i < n - 1; i++) {
        // verificar pivote nulo
        if (A[i][i] == 0) {
            cout << "Zero pivot encountered. Row swapping would be needed.\n";
            return;
        }

        for (int j = i + 1; j < n; j++) {
            double factor = A[j][i] / A[i][i];

            for (int k = i; k <= n; k++) {
                A[j][k] = A[j][k] - factor * A[i][k];
            }
        }
    }
}

int main() {
    // Matriz aumentada del sistema
    vector<vector<double>> A = {
        {2,  1, -1,   8},
        {-3, -1, 2, -11},
        {-2, 1,  2,  -3}
    };

    cout << "Original augmented matrix:\n";
    printMatrix(A);

    forwardElimination(A);

    cout << "After forward elimination:\n";
    printMatrix(A);

    return 0;
}