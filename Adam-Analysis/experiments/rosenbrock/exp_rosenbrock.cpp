#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include "Adam.h"
#include "SGDMomentum.h"
#include "AMSGrad.h"
#include "functions.h"

void run_rosenbrock_experiment(Optimizer& opt, const std::string& name, const std::string& filename) {
    std::vector<double> params = {-1.5, 2.0};
    std::vector<double> grads = {0.0, 0.0};
    opt.reset();

    std::ofstream out("results/csv/" + filename);
    out << "iteration,objective,grad_norm,dist_to_opt,x,y\n";

    std::vector<double> optimum = {1.0, 1.0};
    for (int i = 0; i <= 20000; ++i) {
        double obj = rosenbrock(params, grads);
        double gn = vec_norm(grads);
        double dist = vec_distance(params, optimum);
        if (i % 10 == 0 || i == 20000) {
            out << i << "," << std::fixed << std::setprecision(6)
                << obj << "," << gn << "," << dist << ","
                << params[0] << "," << params[1] << "\n";
        }
        if (gn < 1e-6 && i > 0) break;
        opt.step(params, grads);
    }
    std::cout << name << " Rosenbrock finished. Final: (" << params[0] << ", " << params[1] << ")\n";
}

int main() {
    std::cout << "--- Rosenbrock Experiments ---\n";
    Adam adam(0.01); SGDMomentum sgdm(0.001, 0.9); AMSGrad amsgrad(0.01);
    run_rosenbrock_experiment(adam, "Adam", "rosenbrock_adam.csv");
    run_rosenbrock_experiment(sgdm, "SGD+Momentum", "rosenbrock_sgdm.csv");
    run_rosenbrock_experiment(amsgrad, "AMSGrad", "rosenbrock_amsgrad.csv");
    return 0;
}
