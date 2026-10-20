#ifndef AMSGRAD_H
#define AMSGRAD_H

#include "Optimizer.h"
#include <vector>
#include <string>

// AMSGrad (Reddi, Kale & Kumar, 2018)
//
// Variant of Adam that maintains the maximum of all past second moment
// estimates, fixing the convergence issue identified by Reddi et al.
//
// Update rule:
//   m_t = beta1 * m_{t-1} + (1 - beta1) * g_t
//   v_t = beta2 * v_{t-1} + (1 - beta2) * g_t^2
//   v_hat_t = max(v_hat_{t-1}, v_t)
//   theta_{t+1} = theta_t - alpha * m_t / (sqrt(v_hat_t) + epsilon)
//
// Note: This implementation uses the uncorrected first moment m_t
// (not bias-corrected m_hat) as described in the original AMSGrad paper.
class AMSGrad : public Optimizer {
private:
    double beta1;
    double beta2;
    double epsilon;
    int t;
    std::vector<double> m;       // first moment vector
    std::vector<double> v;       // second moment vector
    std::vector<double> v_hat;   // max second moment vector

public:
    AMSGrad(double lr = 0.001, double b1 = 0.9, double b2 = 0.999, double eps = 1e-8);
    void step(std::vector<double>& params, const std::vector<double>& grads) override;
    void reset() override;
    std::string name() const override;

    double getBeta1() const { return beta1; }
    double getBeta2() const { return beta2; }
    double getEpsilon() const { return epsilon; }
};

#endif
