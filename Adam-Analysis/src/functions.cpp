#include "functions.h"
#include <cmath>
#include <stdexcept>

// ---------------------------------------------------------------
// Quadratic: f(x) = sum(x_i^2)
// ---------------------------------------------------------------
double quadratic(const std::vector<double>& x, std::vector<double>& grad) {
    if (grad.size() != x.size()) throw std::invalid_argument("quadratic gradient dimension mismatch");
    double val = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        val += x[i] * x[i];
        grad[i] = 2.0 * x[i];
    }
    return val;
}

// ---------------------------------------------------------------
// Rosenbrock (2-D): f(x,y) = (1-x)^2 + 100*(y-x^2)^2
// ---------------------------------------------------------------
double rosenbrock(const std::vector<double>& x, std::vector<double>& grad) {
    if (x.size() != 2 || grad.size() != 2) throw std::invalid_argument("Rosenbrock requires two parameters and matching gradient storage");
    double a = 1.0;
    double b = 100.0;
    double val = std::pow(a - x[0], 2) + b * std::pow(x[1] - std::pow(x[0], 2), 2);

    grad[0] = -2.0 * (a - x[0]) - 4.0 * b * x[0] * (x[1] - std::pow(x[0], 2));
    grad[1] = 2.0 * b * (x[1] - std::pow(x[0], 2));

    return val;
}

// ---------------------------------------------------------------
// Rastrigin (N-D): f(x) = 10*d + sum(x_i^2 - 10*cos(2*pi*x_i))
// ---------------------------------------------------------------
double rastrigin(const std::vector<double>& x, std::vector<double>& grad) {
    if (grad.size() != x.size()) throw std::invalid_argument("Rastrigin gradient dimension mismatch");
    double A = 10.0;
    double val = A * x.size();
    for (size_t i = 0; i < x.size(); ++i) {
        val += (x[i] * x[i] - A * std::cos(2.0 * M_PI * x[i]));
        grad[i] = 2.0 * x[i] + 2.0 * A * M_PI * std::sin(2.0 * M_PI * x[i]);
    }
    return val;
}

// ---------------------------------------------------------------
// Reddi's 1-D Counterexample (Reddi et al., 2018)
// C = 101. If t % C == 1, grad = C; otherwise grad = -1.
// The optimal x is -1.
// ---------------------------------------------------------------
void reddi_gradient(int t, const std::vector<double>& x, std::vector<double>& grad) {
    int C = 101;
    if (t % C == 1) {
        grad[0] = static_cast<double>(C);
    } else {
        grad[0] = -1.0;
    }
}

// ---------------------------------------------------------------
// Utilities
// ---------------------------------------------------------------
double vec_norm(const std::vector<double>& v) {
    double sum = 0.0;
    for (double x : v) {
        sum += x * x;
    }
    return std::sqrt(sum);
}

double vec_distance(const std::vector<double>& a, const std::vector<double>& b) {
    if (a.size() != b.size()) throw std::invalid_argument("Vector size mismatch in vec_distance");
    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        double d = a[i] - b[i];
        sum += d * d;
    }
    return std::sqrt(sum);
}
