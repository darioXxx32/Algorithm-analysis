#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>

class Optimizer {
protected:
    double learning_rate;
public:
    Optimizer(double lr) : learning_rate(lr) {
        if (!std::isfinite(lr) || lr <= 0.0)
            throw std::invalid_argument("learning rate must be finite and positive");
    }
    virtual ~Optimizer() = default;

    virtual void step(std::vector<double>& params, const std::vector<double>& grads) = 0;

    // Reset internal state so the optimizer can be reused for a new experiment.
    virtual void reset() = 0;

    // Return the name of this optimizer variant.
    virtual std::string name() const = 0;

    double get_learning_rate() const { return learning_rate; }
};

#endif
