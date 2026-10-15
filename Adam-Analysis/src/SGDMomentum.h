#ifndef SGDMOMENTUM_H
#define SGDMOMENTUM_H

#include "Optimizer.h"
#include <vector>
#include <string>

// SGD with Momentum (heavy ball method)
//
// Update rule:
//   u_t = beta * u_{t-1} + g_t
//   theta_{t+1} = theta_t - alpha * u_t
//
// This uses the "learning rate applied after accumulation" variant,
// equivalent to: v_t = momentum * v_{t-1} + lr * g_t; theta -= v_t
//
// Default hyperparameters: lr=0.01, momentum=0.9
class SGDMomentum : public Optimizer {
private:
    double momentum;
    std::vector<double> v;  // velocity vector

public:
    SGDMomentum(double lr = 0.01, double mom = 0.9);
    void step(std::vector<double>& params, const std::vector<double>& grads) override;
    void reset() override;
    std::string name() const override;

    double getMomentum() const { return momentum; }
};

#endif
