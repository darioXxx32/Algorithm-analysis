#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ---------------------------------------------------------------
// Test Functions: each returns f(x) and writes gradient into grad.
// ---------------------------------------------------------------

// Quadratic: f(x) = sum(x_i^2), gradient = 2*x_i
// Minimum at origin (0,...,0), value 0.
double quadratic(const std::vector<double>& x, std::vector<double>& grad);

// Rosenbrock (2-D): f(x,y) = (1-x)^2 + 100*(y-x^2)^2
// Minimum at (1, 1), value 0.
double rosenbrock(const std::vector<double>& x, std::vector<double>& grad);

// Rastrigin (N-D): f(x) = 10*d + sum(x_i^2 - 10*cos(2*pi*x_i))
// Minimum at origin (0,...,0), value 0.
double rastrigin(const std::vector<double>& x, std::vector<double>& grad);

// Reddi's 1-D Counterexample gradient (Reddi et al., 2018)
// Demonstrates Adam's non-convergence with C = 101.
void reddi_gradient(int t, const std::vector<double>& x, std::vector<double>& grad);

// ---------------------------------------------------------------
// Utility
// ---------------------------------------------------------------

// Euclidean norm of a vector
double vec_norm(const std::vector<double>& v);

// Euclidean distance between two vectors
double vec_distance(const std::vector<double>& a, const std::vector<double>& b);

#endif
