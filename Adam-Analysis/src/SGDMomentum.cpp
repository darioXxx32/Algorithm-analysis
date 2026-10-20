#include "SGDMomentum.h"
#include <stdexcept>
#include <cmath>
#include <limits>

SGDMomentum::SGDMomentum(double lr, double mom)
    : Optimizer(lr), momentum(mom) {
    if (!std::isfinite(mom) || mom < 0.0 || mom >= 1.0)
        throw std::invalid_argument("momentum must be finite and in [0,1)");
}

void SGDMomentum::step(std::vector<double>& params, const std::vector<double>& grads) {
    if (params.size() != grads.size())
        throw std::invalid_argument("SGDMomentum::step - params and grads size mismatch");
    for (size_t i = 0; i < grads.size(); ++i)
        if (!std::isfinite(grads[i]) || !std::isfinite(params[i]))
            throw std::domain_error("SGDMomentum::step received a non-finite parameter or gradient");
    if (!v.empty() && v.size() != params.size())
        throw std::invalid_argument("SGDMomentum parameter dimension changed; call reset before reuse");

    if (v.empty()) {
        v.assign(params.size(), 0.0);
    }

    for (size_t i = 0; i < params.size(); ++i) {
        // v_t = momentum * v_{t-1} + lr * g_t
        v[i] = momentum * v[i] + learning_rate * grads[i];

        // NaN/Inf safety
        if (!std::isfinite(v[i]) || !std::isfinite(params[i] - v[i]))
            throw std::overflow_error("SGDMomentum::step produced a non-finite update");
        params[i] -= v[i];
    }
}

void SGDMomentum::reset() {
    v.clear();
}

std::string SGDMomentum::name() const {
    return "SGD+Momentum";
}
