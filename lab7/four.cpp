#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// Imprimir matriz
void printMatrix(const vector<vector<double>>& M, const string& name) {
    cout << name << ":\n";
    for (const auto& row : M) {
        for (double val : row) {
            cout << setw(10) << fixed << setprecision(2) << val << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Imprimir vector
void printVector(const vector<double>& v, const string& name) {
    cout << name << ":\n";
    for (int i = 0; i < v.size(); i++) {
        cout << name << "[" << i + 1 << "] = "
             << fixed << setprecision(2) << v[i] << endl;
    }
    cout << endl;
}

// LU decomposition based on Gaussian elimination
bool luDecomposition(const vector<vector<double>>& A,
                     vector<vector<double>>& L,
                     vector<vector<double>>& U) {
    int n = A.size();

    U = A;
    L.assign(n, vector<double>(n, 0.0));

    // identidad en L
    for (int i = 0; i < n; i++) {
        L[i][i] = 1.0;
    }

    for (int i = 0; i < n - 1; i++) {
        if (U[i][i] == 0) {
            cout << "Zero pivot encountered. LU without pivoting cannot continue.\n";
            return false;
        }

        for (int j = i + 1; j < n; j++) {
            double factor = U[j][i] / U[i][i];
            L[j][i] = factor;

            for (int k = i; k < n; k++) {
                U[j][k] = U[j][k] - factor * U[i][k];
            }
        }
    }

    return true;
}

// Forward substitution: Ly = b
vector<double> forwardSubstitution(const vector<vector<double>>& L,
                                   const vector<double>& b) {
    int n = L.size();
    vector<double> y(n);

    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < i; j++) {
            sum += L[i][j] * y[j];
        }
        y[i] = (b[i] - sum) / L[i][i];
    }

    return y;
}

// Backward substitution: Ux = y
vector<double> backwardSubstitution(const vector<vector<double>>& U,
                                    const vector<double>& y) {
    int n = U.size();
    vector<double> x(n);

    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += U[i][j] * x[j];
        }
        x[i] = (y[i] - sum) / U[i][i];
    }

    return x;
}

int main() {
    vector<vector<double>> A = {
        { 2,  1,  1},
        { 4, -6,  0},
        {-2,  7,  2}
    };

    vector<double> b = {5, -2, 9};

    vector<vector<double>> L, U;

    if (!luDecomposition(A, L, U)) {
        return 1;
    }

    vector<double> y = forwardSubstitution(L, b);
    vector<double> x = backwardSubstitution(U, y);

    printMatrix(A, "A");
    printMatrix(L, "L");
    printMatrix(U, "U");
    printVector(b, "b");
    printVector(y, "y");
    printVector(x, "x");

    return 0;
}