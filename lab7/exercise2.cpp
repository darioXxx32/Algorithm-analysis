#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// Imprimir vector solución
void printSolution(const vector<double>& x) {
    cout << "Solution vector:\n";
    for (int i = 0; i < x.size(); i++) {
        cout << "x[" << i + 1 << "] = " 
             << fixed << setprecision(2) << x[i] << endl;
    }
    cout << endl;
}

// Backward substitution
vector<double> backwardSubstitution(const vector<vector<double>>& A) {
    int n = A.size();
    vector<double> x(n);

    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;

        for (int j = i + 1; j < n; j++) {
            sum += A[i][j] * x[j];
        }

        x[i] = (A[i][n] - sum) / A[i][i];
    }

    return x;
}

int main() {
    // Matriz aumentada triangular superior
    vector<vector<double>> A = {
        {2.0, 1.0, -1.0, 8.0},
        {0.0, 0.5, 0.5, 1.0},
        {0.0, 0.0, -1.0, 1.0}
    };

    vector<double> x = backwardSubstitution(A);

    printSolution(x);

    return 0;
}
