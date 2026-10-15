#include "AMSGrad.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <limits>

AMSGrad::AMSGrad(double lr, double b1, double b2, double eps)
    : Optimizer(lr), beta1(b1), beta2(b2), epsilon(eps), t(0) {
    if (!std::isfinite(b1) || b1 < 0.0 || b1 >= 1.0 ||
        !std::isfinite(b2) || b2 < 0.0 || b2 >= 1.0 ||
        !std::isfinite(eps) || eps <= 0.0)
        throw std::invalid_argument("AMSGrad requires beta1,beta2 in [0,1) and finite epsilon > 0");
}

void AMSGrad::step(std::vector<double>& params, const std::vector<double>& grads) {
    if (params.size() != grads.size())
        throw std::invalid_argument("AMSGrad::step - params and grads size mismatch");
    for (size_t i = 0; i < grads.size(); ++i)
        if (!std::isfinite(grads[i]) || !std::isfinite(params[i]))
            throw std::domain_error("AMSGrad::step received a non-finite parameter or gradient");
    if (t > 0 && m.size() != params.size())
        throw std::invalid_argument("AMSGrad parameter dimension changed; call reset before reuse");

    if (t == 0) {
        m.assign(params.size(), 0.0);
        v.assign(params.size(), 0.0);
        v_hat.assign(params.size(), 0.0);
    }

    t++;

    for (size_t i = 0; i < params.size(); ++i) {
        // Update biased first moment estimate
        m[i] = beta1 * m[i] + (1.0 - beta1) * grads[i];
        // Update biased second raw moment estimate
        v[i] = beta2 * v[i] + (1.0 - beta2) * grads[i] * grads[i];
        // Keep running maximum of v
        v_hat[i] = std::max(v_hat[i], v[i]);

        // Update parameters using max second moment
        double update = learning_rate * m[i] / (std::sqrt(v_hat[i]) + epsilon);

        // NaN/Inf safety
        if (!std::isfinite(update) || !std::isfinite(params[i] - update))
            throw std::overflow_error("AMSGrad::step produced a non-finite update");
        params[i] -= update;
    }
}

void AMSGrad::reset() {
    t = 0;
    m.clear();
    v.clear();
    v_hat.clear();
}

std::string AMSGrad::name() const {
    return "AMSGrad";
}
