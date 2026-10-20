import os

base_dir = r"C:\Users\User\Desktop\algo\Adam-Analysis"
dirs = [
    "src",
    "include",
    "experiments/convex",
    "experiments/rastrigin",
    "experiments/rosenbrock",
    "experiments/reddi",
    "results/csv",
    "results/plots"
]

for d in dirs:
    os.makedirs(os.path.join(base_dir, d), exist_ok=True)

files = {}

files["CMakeLists.txt"] = """cmake_minimum_required(VERSION 3.10)
project(AdamAnalysis)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include src)

add_executable(AdamAnalysis 
    src/main.cpp 
    src/Adam.cpp 
    src/SGDMomentum.cpp 
    src/AMSGrad.cpp
    src/functions.cpp
)
"""

files["README.md"] = """# Adam Optimizer Analysis

This repository contains the C++ implementation and empirical analysis of the Adam optimization algorithm, comparing it against SGD with Momentum and AMSGrad. 
This project is part of the ECMC1018 Analysis of Algorithms course.

## Structure
- `src/`: C++ source files for the optimizers and test functions.
- `experiments/`: Configuration and scripts for specific experiments.
- `results/`: CSV files and plots generated from the experiments.
"""

files["src/Optimizer.h"] = """#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>

class Optimizer {
protected:
    double learning_rate;
public:
    Optimizer(double lr) : learning_rate(lr) {}
    virtual ~Optimizer() = default;
    
    virtual void step(std::vector<double>& params, const std::vector<double>& grads) = 0;
};

#endif
"""

files["src/Adam.h"] = """#ifndef ADAM_H
#define ADAM_H

#include "Optimizer.h"
#include <vector>

class Adam : public Optimizer {
private:
    double beta1;
    double beta2;
    double epsilon;
    int t;
    std::vector<double> m;
    std::vector<double> v;

public:
    Adam(double lr = 0.001, double b1 = 0.9, double b2 = 0.999, double eps = 1e-8);
    void step(std::vector<double>& params, const std::vector<double>& grads) override;
};

#endif
"""

files["src/Adam.cpp"] = """#include "Adam.h"
#include <cmath>
#include <stdexcept>

Adam::Adam(double lr, double b1, double b2, double eps)
    : Optimizer(lr), beta1(b1), beta2(b2), epsilon(eps), t(0) {}

void Adam::step(std::vector<double>& params, const std::vector<double>& grads) {
    if (params.size() != grads.size()) throw std::invalid_argument("Size mismatch");
    if (t == 0) {
        m.assign(params.size(), 0.0);
        v.assign(params.size(), 0.0);
    }
    t++;
    for (size_t i = 0; i < params.size(); ++i) {
        m[i] = beta1 * m[i] + (1.0 - beta1) * grads[i];
        v[i] = beta2 * v[i] + (1.0 - beta2) * grads[i] * grads[i];
        double m_hat = m[i] / (1.0 - std::pow(beta1, t));
        double v_hat = v[i] / (1.0 - std::pow(beta2, t));
        params[i] -= learning_rate * m_hat / (std::sqrt(v_hat) + epsilon);
    }
}
"""

files["src/SGDMomentum.h"] = """#ifndef SGDMOMENTUM_H
#define SGDMOMENTUM_H

#include "Optimizer.h"
#include <vector>

class SGDMomentum : public Optimizer {
private:
    double momentum;
    std::vector<double> v;

public:
    SGDMomentum(double lr = 0.01, double mom = 0.9);
    void step(std::vector<double>& params, const std::vector<double>& grads) override;
};

#endif
"""

files["src/SGDMomentum.cpp"] = """#include "SGDMomentum.h"
#include <stdexcept>

SGDMomentum::SGDMomentum(double lr, double mom)
    : Optimizer(lr), momentum(mom) {}

void SGDMomentum::step(std::vector<double>& params, const std::vector<double>& grads) {
    if (params.size() != grads.size()) throw std::invalid_argument("Size mismatch");
    if (v.empty()) {
        v.assign(params.size(), 0.0);
    }
    for (size_t i = 0; i < params.size(); ++i) {
        v[i] = momentum * v[i] + learning_rate * grads[i];
        params[i] -= v[i];
    }
}
"""

files["src/AMSGrad.h"] = """#ifndef AMSGRAD_H
#define AMSGRAD_H

#include "Optimizer.h"
#include <vector>

class AMSGrad : public Optimizer {
private:
    double beta1;
    double beta2;
    double epsilon;
    int t;
    std::vector<double> m;
    std::vector<double> v;
    std::vector<double> v_hat;

public:
    AMSGrad(double lr = 0.001, double b1 = 0.9, double b2 = 0.999, double eps = 1e-8);
    void step(std::vector<double>& params, const std::vector<double>& grads) override;
};

#endif
"""

files["src/AMSGrad.cpp"] = """#include "AMSGrad.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

AMSGrad::AMSGrad(double lr, double b1, double b2, double eps)
    : Optimizer(lr), beta1(b1), beta2(b2), epsilon(eps), t(0) {}

void AMSGrad::step(std::vector<double>& params, const std::vector<double>& grads) {
    if (params.size() != grads.size()) throw std::invalid_argument("Size mismatch");
    if (t == 0) {
        m.assign(params.size(), 0.0);
        v.assign(params.size(), 0.0);
        v_hat.assign(params.size(), 0.0);
    }
    t++;
    for (size_t i = 0; i < params.size(); ++i) {
        m[i] = beta1 * m[i] + (1.0 - beta1) * grads[i];
        v[i] = beta2 * v[i] + (1.0 - beta2) * grads[i] * grads[i];
        v_hat[i] = std::max(v_hat[i], v[i]);
        params[i] -= learning_rate * m[i] / (std::sqrt(v_hat[i]) + epsilon);
    }
}
"""

files["src/functions.h"] = """#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>

#define M_PI 3.14159265358979323846

// Returns objective value and computes gradient in-place
double rosenbrock(const std::vector<double>& x, std::vector<double>& grad);
double rastrigin(const std::vector<double>& x, std::vector<double>& grad);

#endif
"""

files["src/functions.cpp"] = """#include "functions.h"
#include <cmath>

double rosenbrock(const std::vector<double>& x, std::vector<double>& grad) {
    double a = 1.0;
    double b = 100.0;
    double val = std::pow(a - x[0], 2) + b * std::pow(x[1] - std::pow(x[0], 2), 2);
    
    grad[0] = -2.0 * (a - x[0]) - 4.0 * b * x[0] * (x[1] - std::pow(x[0], 2));
    grad[1] = 2.0 * b * (x[1] - std::pow(x[0], 2));
    
    return val;
}

double rastrigin(const std::vector<double>& x, std::vector<double>& grad) {
    double A = 10.0;
    double val = A * x.size();
    for (size_t i = 0; i < x.size(); ++i) {
        val += (x[i] * x[i] - A * std::cos(2.0 * M_PI * x[i]));
        grad[i] = 2.0 * x[i] + 20.0 * M_PI * std::sin(2.0 * M_PI * x[i]);
    }
    return val;
}
"""

files["src/main.cpp"] = """#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include "Adam.h"
#include "SGDMomentum.h"
#include "AMSGrad.h"
#include "functions.h"

void run_experiment(Optimizer& opt, const std::string& name, const std::string& filename) {
    std::vector<double> params = {-1.5, 2.0}; // Initial point
    std::vector<double> grads = {0.0, 0.0};
    
    std::ofstream out("results/csv/" + filename);
    out << "iteration,objective,x,y\\n";
    
    for (int i = 0; i <= 2000; ++i) {
        double obj = rosenbrock(params, grads);
        if (i % 10 == 0 || i == 2000) {
            out << i << "," << std::fixed << std::setprecision(6) << obj << "," << params[0] << "," << params[1] << "\\n";
        }
        opt.step(params, grads);
    }
    
    std::cout << name << " finished. Final params: (" << params[0] << ", " << params[1] << ")\\n";
}

int main() {
    Adam adam(0.01);
    run_experiment(adam, "Adam", "rosenbrock_adam.csv");
    
    SGDMomentum sgdm(0.001, 0.9);
    run_experiment(sgdm, "SGD+Momentum", "rosenbrock_sgdm.csv");
    
    AMSGrad amsgrad(0.01);
    run_experiment(amsgrad, "AMSGrad", "rosenbrock_amsgrad.csv");
    
    return 0;
}
"""

for path, content in files.items():
    with open(os.path.join(base_dir, path), "w", encoding="utf-8") as f:
        f.write(content)

print("Project generated successfully.")
