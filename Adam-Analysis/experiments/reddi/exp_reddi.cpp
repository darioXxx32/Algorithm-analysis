#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "Adam.h"
#include "AMSGrad.h"
#include "functions.h"

void run_reddi_experiment(Optimizer& opt, const std::string& name, const std::string& filename) {
    std::vector<double> params = {0.0};
    std::vector<double> grads = {0.0};
    opt.reset();

    std::ofstream out("results/csv/" + filename);
    out << "iteration,x\n";

    int max_iters = 300000;
    for (int i = 1; i <= max_iters; ++i) {
        if (i % 1000 == 0 || i == 1) {
            out << i << "," << std::fixed << std::setprecision(6) << params[0] << "\n";
        }
        reddi_gradient(i, params, grads);
        opt.step(params, grads);
        params[0] = std::max(-1.0, std::min(1.0, params[0]));
    }
    std::cout << name << " Reddi finished. Final x=" << params[0] << "\n";
}

int main() {
    std::cout << "--- Reddi 1D Counterexample ---\n";
    double lr = 0.01;
    Adam adam(lr, 0.9, 0.99); AMSGrad amsgrad(lr, 0.9, 0.99);
    run_reddi_experiment(adam, "Adam", "reddi_adam.csv");
    run_reddi_experiment(amsgrad, "AMSGrad", "reddi_amsgrad.csv");
    return 0;
}
