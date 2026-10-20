#ifndef ADAM_H
#define ADAM_H

#include "Optimizer.h"
#include <vector>
#include <string>

// Adam: Adaptive Moment Estimation (Kingma & Ba, 2015)
//
// Update rule per parameter i at timestep t:
//   m_t = beta1 * m_{t-1} + (1 - beta1) * g_t
//   v_t = beta2 * v_{t-1} + (1 - beta2) * g_t^2
//   m_hat = m_t / (1 - beta1^t)
//   v_hat = v_t / (1 - beta2^t)
//   theta_{t+1} = theta_t - alpha * m_hat / (sqrt(v_hat) + epsilon)
//
// Default hyperparameters: alpha=0.001, beta1=0.9, beta2=0.999, eps=1e-8
// State convention: m0=v0=0; t increments before bias correction.
class Adam : public Optimizer {
private:
    double beta1;
    double beta2;
    double epsilon;
    int t;                       // timestep counter
    std::vector<double> m;       // first moment vector
    std::vector<double> v;       // second moment vector

public:
    Adam(double lr = 0.001, double b1 = 0.9, double b2 = 0.999, double eps = 1e-8);
    void step(std::vector<double>& params, const std::vector<double>& grads) override;
    void reset() override;
    std::string name() const override;

    // Accessors for logging
    double getBeta1() const { return beta1; }
    double getBeta2() const { return beta2; }
    double getEpsilon() const { return epsilon; }
    int getTimestep() const { return t; }
};

#endif



