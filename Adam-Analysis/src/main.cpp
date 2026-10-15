// ============================================================================
// Adam Optimizer Analysis — Main Experiment Runner
// ECMC1018 Analysis of Algorithms — Presentation I
//
// Runs all experiments (Quadratic, Rosenbrock, Rastrigin, Reddi Counterexample)
// for Adam, SGD+Momentum, and AMSGrad. Logs comprehensive metrics to CSV.
// ============================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <cmath>
#include <functional>
#include <sstream>
#include <algorithm>

#include "Adam.h"
#include "SGDMomentum.h"
#include "AMSGrad.h"
#include "functions.h"

// Type alias for objective functions: f(x, grad) -> objective value
using ObjFunc = std::function<double(const std::vector<double>&, std::vector<double>&)>;

// ============================================================================
// Experiment configuration
// ============================================================================
struct ExperimentConfig {
    std::string func_name;          // e.g. "quadratic", "rosenbrock"
    ObjFunc func;                   // objective function
    std::vector<double> x0;         // initial point
    std::vector<double> optimum;    // known optimum for distance measurement
    int max_iters;                  // maximum iterations
    int log_interval;               // log every N iterations
    double grad_tol;                // convergence tolerance on gradient norm
};

// ============================================================================
// Run a single optimizer on a single experiment, write detailed CSV
// ============================================================================
struct ExperimentResult {
    std::string optimizer_name;
    std::string function_name;
    int final_iteration;
    int max_iterations;
    double grad_tolerance;
    double learning_rate;
    double beta1;
    double beta2;
    double epsilon;
    double momentum;
    std::string stop_reason;
    double final_objective;
    double final_grad_norm;
    double final_dist_to_opt;
    double elapsed_ms;
    bool converged;
};

ExperimentResult run_experiment(
    Optimizer& opt,
    const ExperimentConfig& cfg,
    const std::string& csv_path
) {
    // Initialize
    std::vector<double> params = cfg.x0;
    size_t d = params.size();
    std::vector<double> grads(d, 0.0);

    opt.reset();

    // Open CSV
    std::ofstream out(csv_path);
    if (!out.is_open()) {
        std::cerr << "ERROR: Cannot open " << csv_path << std::endl;
        return {};
    }

    // CSV header
    out << "iteration,objective,grad_norm,dist_to_opt";
    for (size_t i = 0; i < d; ++i) out << ",x" << i;
    out << "\n";

    ExperimentResult result;
    result.optimizer_name = opt.name();
    result.function_name = cfg.func_name;
    result.converged = false;
    result.max_iterations = cfg.max_iters;
    result.grad_tolerance = cfg.grad_tol;
    result.learning_rate = opt.get_learning_rate();
    result.beta1 = result.beta2 = result.epsilon = result.momentum = 0.0;
    if (auto* a = dynamic_cast<Adam*>(&opt)) { result.beta1=a->getBeta1(); result.beta2=a->getBeta2(); result.epsilon=a->getEpsilon(); }
    if (auto* a = dynamic_cast<AMSGrad*>(&opt)) { result.beta1=a->getBeta1(); result.beta2=a->getBeta2(); result.epsilon=a->getEpsilon(); }
    if (auto* s = dynamic_cast<SGDMomentum*>(&opt)) result.momentum=s->getMomentum();
    result.stop_reason = "max_iterations";

    auto t_start = std::chrono::high_resolution_clock::now();

    int final_iter = 0;
    for (int iter = 0; iter <= cfg.max_iters; ++iter) {
        double obj = cfg.func(params, grads);
        double gn = vec_norm(grads);
        double dist = vec_distance(params, cfg.optimum);

        // Log at interval or at first/last iteration
        if (iter % cfg.log_interval == 0 || iter == cfg.max_iters) {
            out << iter << ","
                << std::fixed << std::setprecision(10) << obj << ","
                << std::setprecision(10) << gn << ","
                << std::setprecision(10) << dist;
            for (size_t i = 0; i < d; ++i)
                out << "," << std::setprecision(10) << params[i];
            out << "\n";
        }

        // Check convergence
        if (gn < cfg.grad_tol && iter > 0) {
            result.converged = true;
            result.stop_reason = "gradient_tolerance";
            final_iter = iter;
            result.final_objective = obj;
            result.final_grad_norm = gn;
            result.final_dist_to_opt = dist;
            // Log convergence point
            out << iter << ","
                << std::fixed << std::setprecision(10) << obj << ","
                << std::setprecision(10) << gn << ","
                << std::setprecision(10) << dist;
            for (size_t i = 0; i < d; ++i)
                out << "," << std::setprecision(10) << params[i];
            out << "\n";
            break;
        }

        // Step
        opt.step(params, grads);
        final_iter = iter + 1;

        // Check for numerical issues
        bool finite = true;
        for (size_t i = 0; i < d; ++i) {
            if (!std::isfinite(params[i])) { finite = false; break; }
        }
        if (!finite) {
            std::cerr << "WARNING: Non-finite params at iter " << iter+1
                      << " for " << opt.name() << " on " << cfg.func_name << std::endl;
            result.stop_reason = "non_finite_parameters";
            break;
        }
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(t_end - t_start).count();

    if (!result.converged) {
        // Compute final metrics
        double obj = cfg.func(params, grads);
        result.final_objective = obj;
        result.final_grad_norm = vec_norm(grads);
        result.final_dist_to_opt = vec_distance(params, cfg.optimum);
    }

    result.final_iteration = final_iter;
    result.elapsed_ms = elapsed;

    out.close();

    std::cout << "  " << std::left << std::setw(14) << opt.name()
              << " | iters=" << std::setw(6) << final_iter
              << " | obj=" << std::scientific << std::setprecision(4) << result.final_objective
              << " | |g|=" << result.final_grad_norm
              << " | dist=" << result.final_dist_to_opt
              << " | " << std::fixed << std::setprecision(1) << elapsed << " ms"
              << (result.converged ? " [CONVERGED]" : "")
              << "\n";

    return result;
}

// ============================================================================
// Run Reddi counterexample (special: gradient from schedule, not from f)
// ============================================================================
ExperimentResult run_reddi_experiment(
    Optimizer& opt,
    const std::string& csv_path,
    int max_iters = 300000,
    int log_interval = 1000
) {
    std::vector<double> params = {0.0};
    std::vector<double> grads = {0.0};

    opt.reset();

    std::ofstream out(csv_path);
    out << "iteration,x\n";

    auto t_start = std::chrono::high_resolution_clock::now();

    for (int i = 1; i <= max_iters; ++i) {
        if (i % log_interval == 0 || i == 1) {
            out << i << "," << std::fixed << std::setprecision(6) << params[0] << "\n";
        }
        reddi_gradient(i, params, grads);
        opt.step(params, grads);

        // Projection onto [-1, 1] as per Reddi et al.
        params[0] = std::max(-1.0, std::min(1.0, params[0]));
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(t_end - t_start).count();

    out.close();

    std::cout << "  " << std::left << std::setw(14) << opt.name()
              << " | final x=" << std::fixed << std::setprecision(6) << params[0]
              << " | optimal=-1.0"
              << " | " << std::setprecision(1) << elapsed << " ms\n";

    ExperimentResult result;
    result.optimizer_name = opt.name();
    result.function_name = "reddi";
    result.final_iteration = max_iters;
    result.final_objective = params[0]; // x is the metric here
    result.final_grad_norm = 0;
    result.final_dist_to_opt = std::abs(params[0] - (-1.0));
    result.elapsed_ms = elapsed;
    result.converged = (std::abs(params[0] - (-1.0)) < 0.05);
    return result;
}

// ============================================================================
// Learning rate sensitivity sweep for Adam
// ============================================================================
void run_lr_sweep(
    const ExperimentConfig& cfg,
    const std::vector<double>& learning_rates,
    const std::string& csv_path
) {
    std::ofstream out(csv_path);
    out << "learning_rate,final_iter,final_objective,final_grad_norm,final_dist,elapsed_ms,converged,max_iterations,grad_tolerance\n";

    for (double lr : learning_rates) {
        Adam adam(lr);
        std::string dummy_csv = "results/csv/lr_sweep_tmp.csv";
        ExperimentConfig temp_cfg = cfg;
        auto res = run_experiment(adam, temp_cfg, dummy_csv);

        out << std::scientific << std::setprecision(4) << lr << ","
            << res.final_iteration << ","
            << res.final_objective << ","
            << res.final_grad_norm << ","
            << res.final_dist_to_opt << ","
            << std::fixed << std::setprecision(2) << res.elapsed_ms << ","
            << (res.converged ? 1 : 0) << "," << temp_cfg.max_iters << ","
            << std::scientific << std::setprecision(8) << temp_cfg.grad_tol << "\n";
    }

    out.close();
    std::cout << "  Learning rate sweep saved to " << csv_path << "\n";
}

// ============================================================================
// Summary table
// ============================================================================
void write_summary(const std::vector<ExperimentResult>& results, const std::string& path) {
    std::ofstream out(path);
    out << "optimizer,function,final_iter,final_objective,final_grad_norm,final_dist,elapsed_ms,converged,learning_rate,beta1,beta2,epsilon,momentum,max_iterations,grad_tolerance,stop_reason\n";
    for (const auto& r : results) {
        out << r.optimizer_name << ","
            << r.function_name << ","
            << r.final_iteration << ","
            << std::scientific << std::setprecision(6) << r.final_objective << ","
            << r.final_grad_norm << ","
            << r.final_dist_to_opt << ","
            << std::fixed << std::setprecision(2) << r.elapsed_ms << ","
            << (r.converged ? 1 : 0) << "," << std::scientific << std::setprecision(8)
            << r.learning_rate << "," << r.beta1 << "," << r.beta2 << "," << r.epsilon << "," << r.momentum << ","
            << std::defaultfloat << r.max_iterations << "," << std::scientific << std::setprecision(8) << r.grad_tolerance << "," << r.stop_reason << "\n";
    }
    out.close();
    std::cout << "\nSummary saved to " << path << "\n";
}

// ============================================================================
// MAIN
// ============================================================================
int main() {
    std::vector<ExperimentResult> all_results;

    // ------------------------------------------------------------------
    // Experiment 1: Quadratic f(x) = sum(x_i^2), 2D
    // ------------------------------------------------------------------
    std::cout << "=== Experiment 1: Quadratic (2D) ===\n";
    {
        ExperimentConfig cfg;
        cfg.func_name = "quadratic_2d";
        cfg.func = quadratic;
        cfg.x0 = {5.0, -3.0};
        cfg.optimum = {0.0, 0.0};
        cfg.max_iters = 5000;
        cfg.log_interval = 1;
        cfg.grad_tol = 1e-8;

        Adam adam(0.01);
        SGDMomentum sgdm(0.01, 0.9);
        AMSGrad amsgrad(0.01);

        all_results.push_back(run_experiment(adam,    cfg, "results/csv/quadratic_2d_adam.csv"));
        all_results.push_back(run_experiment(sgdm,    cfg, "results/csv/quadratic_2d_sgdm.csv"));
        all_results.push_back(run_experiment(amsgrad, cfg, "results/csv/quadratic_2d_amsgrad.csv"));
    }

    // ------------------------------------------------------------------
    // Experiment 1b: Quadratic f(x) = sum(x_i^2), 10D
    // ------------------------------------------------------------------
    std::cout << "\n=== Experiment 1b: Quadratic (10D) ===\n";
    {
        ExperimentConfig cfg;
        cfg.func_name = "quadratic_10d";
        cfg.func = quadratic;
        cfg.x0 = {5.0, -3.0, 2.0, -4.0, 1.0, -2.0, 3.0, -1.0, 4.0, -5.0};
        cfg.optimum = std::vector<double>(10, 0.0);
        cfg.max_iters = 10000;
        cfg.log_interval = 10;
        cfg.grad_tol = 1e-8;

        Adam adam(0.01);
        SGDMomentum sgdm(0.01, 0.9);
        AMSGrad amsgrad(0.01);

        all_results.push_back(run_experiment(adam,    cfg, "results/csv/quadratic_10d_adam.csv"));
        all_results.push_back(run_experiment(sgdm,    cfg, "results/csv/quadratic_10d_sgdm.csv"));
        all_results.push_back(run_experiment(amsgrad, cfg, "results/csv/quadratic_10d_amsgrad.csv"));
    }

    // ------------------------------------------------------------------
    // Experiment 2: Rosenbrock
    // ------------------------------------------------------------------
    std::cout << "\n=== Experiment 2: Rosenbrock (2D) ===\n";
    {
        ExperimentConfig cfg;
        cfg.func_name = "rosenbrock";
        cfg.func = rosenbrock;
        cfg.x0 = {-1.5, 2.0};
        cfg.optimum = {1.0, 1.0};
        cfg.max_iters = 20000;
        cfg.log_interval = 10;
        cfg.grad_tol = 1e-6;

        Adam adam(0.01);
        SGDMomentum sgdm(0.001, 0.9);
        AMSGrad amsgrad(0.01);

        all_results.push_back(run_experiment(adam,    cfg, "results/csv/rosenbrock_adam.csv"));
        all_results.push_back(run_experiment(sgdm,    cfg, "results/csv/rosenbrock_sgdm.csv"));
        all_results.push_back(run_experiment(amsgrad, cfg, "results/csv/rosenbrock_amsgrad.csv"));
    }

    // ------------------------------------------------------------------
    // Experiment 3: Rastrigin (2D)
    // ------------------------------------------------------------------
    std::cout << "\n=== Experiment 3: Rastrigin (2D) ===\n";
    {
        ExperimentConfig cfg;
        cfg.func_name = "rastrigin";
        cfg.func = rastrigin;
        cfg.x0 = {2.5, 2.5};
        cfg.optimum = {0.0, 0.0};
        cfg.max_iters = 5000;
        cfg.log_interval = 10;
        cfg.grad_tol = 1e-6;

        Adam adam(0.05);
        SGDMomentum sgdm(0.005, 0.9);
        AMSGrad amsgrad(0.05);

        all_results.push_back(run_experiment(adam,    cfg, "results/csv/rastrigin_adam.csv"));
        all_results.push_back(run_experiment(sgdm,    cfg, "results/csv/rastrigin_sgdm.csv"));
        all_results.push_back(run_experiment(amsgrad, cfg, "results/csv/rastrigin_amsgrad.csv"));
    }

    // ------------------------------------------------------------------
    // Experiment 4: Reddi Counterexample
    // ------------------------------------------------------------------
    std::cout << "\n=== Experiment 4: Reddi Counterexample ===\n";
    {
        double lr = 0.01;
        Adam adam(lr, 0.9, 0.99);
        AMSGrad amsgrad(lr, 0.9, 0.99);

        all_results.push_back(run_reddi_experiment(adam,    "results/csv/reddi_adam.csv"));
        all_results.push_back(run_reddi_experiment(amsgrad, "results/csv/reddi_amsgrad.csv"));
    }

    // ------------------------------------------------------------------
    // Experiment 5: Learning Rate Sensitivity (Adam on Rosenbrock)
    // ------------------------------------------------------------------
    std::cout << "\n=== Experiment 5: Learning Rate Sensitivity (Adam, Rosenbrock) ===\n";
    {
        ExperimentConfig cfg;
        cfg.func_name = "rosenbrock";
        cfg.func = rosenbrock;
        cfg.x0 = {-1.5, 2.0};
        cfg.optimum = {1.0, 1.0};
        cfg.max_iters = 20000;
        cfg.log_interval = 50;
        cfg.grad_tol = 1e-6;

        std::vector<double> lrs = {0.0001, 0.0005, 0.001, 0.005, 0.01, 0.05, 0.1};
        run_lr_sweep(cfg, lrs, "results/csv/lr_sweep_rosenbrock.csv");

        // Also save individual trajectories for key LRs
        for (double lr : {0.001, 0.01, 0.1}) {
            Adam adam(lr);
            std::ostringstream fname;
            fname << "results/csv/rosenbrock_adam_lr" << std::fixed << std::setprecision(4) << lr << ".csv";
            run_experiment(adam, cfg, fname.str());
        }
    }

    // ------------------------------------------------------------------
    // Summary
    // ------------------------------------------------------------------
    write_summary(all_results, "results/csv/summary.csv");

    std::cout << "\n=== All experiments complete. ===\n";
    return 0;
}
