#include "Adam.h"
#include <cmath>
#include <stdexcept>
#include <limits>

Adam::Adam(double lr, double b1, double b2, double eps)
    : Optimizer(lr), beta1(b1), beta2(b2), epsilon(eps), t(0) {
    if (!std::isfinite(b1) || b1 < 0.0 || b1 >= 1.0 ||
        !std::isfinite(b2) || b2 < 0.0 || b2 >= 1.0 ||
        !std::isfinite(eps) || eps <= 0.0)
        throw std::invalid_argument("Adam requires beta1,beta2 in [0,1) and finite epsilon > 0");
}

void Adam::step(std::vector<double>& params, const std::vector<double>& grads) {
    if (params.size() != grads.size())
        throw std::invalid_argument("Adam::step - params and grads size mismatch");
    for (size_t i = 0; i < grads.size(); ++i)
        if (!std::isfinite(grads[i]) || !std::isfinite(params[i]))
            throw std::domain_error("Adam::step received a non-finite parameter or gradient");
    if (t > 0 && m.size() != params.size())
        throw std::invalid_argument("Adam parameter dimension changed; call reset before reuse");

    // Lazy initialization of moment vectors
    if (t == 0) {
        m.assign(params.size(), 0.0);
        v.assign(params.size(), 0.0);
    }

    t++;

    // Precompute bias correction denominators
    double bc1 = 1.0 - std::pow(beta1, t);
    double bc2 = 1.0 - std::pow(beta2, t);

    for (size_t i = 0; i < params.size(); ++i) {
        // Update biased first moment estimate
        m[i] = beta1 * m[i] + (1.0 - beta1) * grads[i];
        // Update biased second raw moment estimate
        v[i] = beta2 * v[i] + (1.0 - beta2) * grads[i] * grads[i];

        // Compute bias-corrected moment estimates
        double m_hat = m[i] / bc1;
        double v_hat = v[i] / bc2;

        // Update parameters
        double update = learning_rate * m_hat / (std::sqrt(v_hat) + epsilon);

        if (!std::isfinite(update) || !std::isfinite(params[i] - update))
            throw std::overflow_error("Adam::step produced a non-finite update");
        params[i] -= update;
    }
}

void Adam::reset() {
    t = 0;
    m.clear();
    v.clear();
}

std::string Adam::name() const {
    return "Adam";
}
