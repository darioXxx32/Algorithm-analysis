#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include "Adam.h"
#include "SGDMomentum.h"
#include "AMSGrad.h"
#include "functions.h"

void run_rastrigin_experiment(Optimizer& opt, const std::string& name, const std::string& filename) {
    std::vector<double> params = {2.5, 2.5};
    std::vector<double> grads = {0.0, 0.0};
    opt.reset();

    std::ofstream out("results/csv/" + filename);
    out << "iteration,objective,grad_norm,dist_to_opt,x,y\n";

    std::vector<double> optimum = {0.0, 0.0};
    for (int i = 0; i <= 5000; ++i) {
        double obj = rastrigin(params, grads);
        double gn = vec_norm(grads);
        double dist = vec_distance(params, optimum);
        if (i % 10 == 0 || i == 5000) {
            out << i << "," << std::fixed << std::setprecision(6)
                << obj << "," << gn << "," << dist << ","
                << params[0] << "," << params[1] << "\n";
        }
        opt.step(params, grads);
    }
    std::cout << name << " Rastrigin finished. Final: (" << params[0] << ", " << params[1] << ")\n";
}

int main() {
    std::cout << "--- Rastrigin Experiments ---\n";
    Adam adam(0.05); SGDMomentum sgdm(0.005, 0.9); AMSGrad amsgrad(0.05);
    run_rastrigin_experiment(adam, "Adam", "rastrigin_adam.csv");
    run_rastrigin_experiment(sgdm, "SGD+Momentum", "rastrigin_sgdm.csv");
    run_rastrigin_experiment(amsgrad, "AMSGrad", "rastrigin_amsgrad.csv");
    return 0;
}
