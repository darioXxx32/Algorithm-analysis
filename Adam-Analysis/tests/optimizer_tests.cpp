#include "Adam.h"
#include "SGDMomentum.h"
#include "AMSGrad.h"
#include "functions.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace {
void require(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}
bool near(double a, double b, double tol = 1e-12) {
    return std::abs(a - b) <= tol * (1.0 + std::abs(b));
}
template<class F> void must_throw(F f, const char* message) {
    bool threw = false;
    try { f(); } catch (const std::exception&) { threw = true; }
    require(threw, message);
}
}

int main() {
    try {
        std::vector<double> x{2.0, -3.0}, g(2);
        require(near(quadratic(x, g), 13.0) && near(g[0], 4.0) && near(g[1], -6.0), "quadratic value/gradient");
        x = {1.0, 1.0};
        require(near(rosenbrock(x, g), 0.0) && near(g[0], 0.0) && near(g[1], 0.0), "Rosenbrock minimum");
        x = {0.0, 0.0};
        require(near(rastrigin(x, g), 0.0) && near(g[0], 0.0) && near(g[1], 0.0), "Rastrigin minimum");

        Adam adam(0.1, 0.9, 0.999, 1e-8);
        x = {2.0}; g = {2.0}; adam.step(x, g);
        require(near(x[0], 1.9, 1e-8) && adam.getTimestep() == 1, "Adam first bias-corrected update");
        adam.reset(); require(adam.getTimestep() == 0, "Adam reset");

        SGDMomentum momentum(0.1, 0.9);
        x = {2.0}; g = {2.0}; momentum.step(x, g); require(near(x[0], 1.8), "momentum first update");
        AMSGrad ams(0.1, 0.9, 0.999, 1e-8);
        x = {2.0}; g = {2.0}; ams.step(x, g); require(near(x[0], 1.6837722840, 1e-8), "AMSGrad first uncorrected-moment update");

        must_throw([] { Adam bad(0.0); }, "reject nonpositive learning rate");
        must_throw([] { Adam bad(0.1, 1.0); }, "reject beta1=1");
        must_throw([] { AMSGrad bad(0.1, 0.9, -0.1); }, "reject invalid beta2");
        must_throw([] { SGDMomentum bad(0.1, 1.0); }, "reject invalid momentum");
        Adam dims; x = {1.0}; g = {1.0}; dims.step(x, g); x = {1.0, 2.0}; g = {1.0, 2.0};
        must_throw([&] { dims.step(x, g); }, "reject changed dimension");
        Adam nonfinite; x = {1.0}; g = {std::numeric_limits<double>::infinity()};
        must_throw([&] { nonfinite.step(x, g); }, "reject non-finite gradient");
        std::cout << "All optimizer tests passed.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failure: " << e.what() << '\n';
        return 1;
    }
}
