#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;
using ll = long long;

ll lcg_next(ll x, ll a, ll c, ll m) {
    return ((__int128)a * x + c) % m;
}

vector<ll> lcg(ll seed, ll a, ll c, ll m, int n_samples) {
    vector<ll> seq;
    seq.reserve(n_samples);
    ll x = seed;
    for (int i = 0; i < n_samples; i++) {
        x = lcg_next(x, a, c, m);
        seq.push_back(x);
    }
    return seq;
}

ll gcd_ll(ll a, ll b) {
    while (b != 0) {
        ll r = a % b;
        a = b;
        b = r;
    }
    return a < 0 ? -a : a;
}

vector<ll> distinct_prime_factors(ll n) {
    vector<ll> factors;
    if (n % 2 == 0) {
        factors.push_back(2);
        while (n % 2 == 0) n /= 2;
    }
    for (ll p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            factors.push_back(p);
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) factors.push_back(n);
    return factors;
}

bool hull_dobell_full_period(ll a, ll c, ll m) {
    if (gcd_ll(c, m) != 1) return false;

    for (ll p : distinct_prime_factors(m)) {
        if ((a - 1) % p != 0) return false;
    }

    if (m % 4 == 0 && (a - 1) % 4 != 0) return false;
    return true;
}

ll compute_period_small(ll seed, ll a, ll c, ll m) {
    unordered_set<ll> seen;
    ll x = seed;
    ll period = 0;
    while (!seen.count(x)) {
        seen.insert(x);
        x = lcg_next(x, a, c, m);
        period++;
    }
    return period;
}

double mean_normalized(const vector<ll>& v, ll m) {
    long double s = 0;
    for (ll x : v) s += (long double)x / (m - 1);
    return (double)(s / v.size());
}

double stddev_normalized(const vector<ll>& v, ll m) {
    double mu = mean_normalized(v, m);
    long double s = 0;
    for (ll x : v) {
        long double z = (long double)x / (m - 1);
        s += (z - mu) * (z - mu);
    }
    return (double)sqrt(s / v.size());
}

void small_example() {
    cout << "============================================================\n";
    cout << "  SMALL EXAMPLE: x_{n+1} = (5*x_n + 3) mod 16  (seed=1)\n";
    cout << "============================================================\n";
    cout << left << setw(6) << "n" << setw(8) << "x_n" << "Computation\n";
    cout << string(45, '-') << "\n";

    ll a = 5, c = 3, m = 16, x = 1;
    cout << setw(6) << 0 << setw(8) << x << "seed\n";
    for (int n = 1; n <= 16; n++) {
        ll prev = x;
        x = lcg_next(x, a, c, m);
        cout << setw(6) << n << setw(8) << x
             << "(" << a << "*" << prev << "+" << c << ") mod " << m
             << " = " << x;
        if (n == 16) cout << "  <- returns to seed";
        cout << "\n";
    }

    ll period = compute_period_small(1, 5, 3, 16);
    auto seq = lcg(1, 5, 3, 16, 16);
    cout << "\nExact period: " << period << " values\n";
    cout << "Hull-Dobell full-period test: "
         << (hull_dobell_full_period(5, 3, 16) ? "PASS" : "FAIL") << "\n";
    cout << "Normalized mean = " << fixed << setprecision(4)
         << mean_normalized(seq, 16) << "  (expected near 0.5000)\n\n";
}

struct Config {
    string name;
    string m_label;
    ll seed;
    ll a;
    ll c;
    ll m;
    bool exact_period;
    string theoretical_period;
    string basis;
};

void empirical_analysis(const vector<Config>& configs, int n_samples = 10000) {
    cout << "============================================================\n";
    cout << "  EMPIRICAL LCG ANALYSIS  (" << n_samples << " samples each)\n";
    cout << "============================================================\n";
    cout << left
         << setw(4)  << "#"
         << setw(19) << "Name"
         << setw(10) << "m"
         << setw(16) << "Period"
         << setw(10) << "Mean01"
         << setw(10) << "Std01"
         << setw(9)  << "ms"
         << setw(8)  << "OK?"
         << "\n";
    cout << string(86, '-') << "\n";

    const double expected_mean = 0.5;
    const double expected_std = sqrt(1.0 / 12.0);

    for (int i = 0; i < (int)configs.size(); i++) {
        const Config& cfg = configs[i];

        auto t0 = chrono::high_resolution_clock::now();
        auto seq = lcg(cfg.seed, cfg.a, cfg.c, cfg.m, n_samples);
        auto t1 = chrono::high_resolution_clock::now();

        double ms = chrono::duration<double, milli>(t1 - t0).count();
        double mu = mean_normalized(seq, cfg.m);
        double sig = stddev_normalized(seq, cfg.m);
        bool uniform_like = fabs(mu - expected_mean) < 0.03 &&
                            fabs(sig - expected_std) < 0.04;

        string period = cfg.exact_period
            ? to_string(compute_period_small(cfg.seed, cfg.a, cfg.c, cfg.m))
            : cfg.theoretical_period;

        cout << left
             << setw(4)  << (i + 1)
             << setw(19) << cfg.name
             << setw(10) << cfg.m_label
             << setw(16) << period
             << setw(10) << fixed << setprecision(4) << mu
             << setw(10) << fixed << setprecision(4) << sig
             << setw(9)  << fixed << setprecision(3) << ms
             << setw(8)  << (uniform_like ? "YES" : "CHECK")
             << "\n";
    }

    cout << "\nMean01 and Std01 are computed after scaling x_n to [0,1].\n";
    cout << "The expected values for a uniform distribution are mean ~= 0.5\n";
    cout << "and standard deviation ~= 1/sqrt(12) = 0.2887.\n";
}

void period_condition_report(const vector<Config>& configs) {
    cout << "\n============================================================\n";
    cout << "  PERIOD CONDITIONS\n";
    cout << "============================================================\n";
    cout << left
         << setw(4)  << "#"
         << setw(19) << "Name"
         << setw(15) << "Hull-Dobell"
         << setw(17) << "Expected"
         << "Basis\n";
    cout << string(86, '-') << "\n";

    for (int i = 0; i < (int)configs.size(); i++) {
        const Config& cfg = configs[i];
        bool hd = hull_dobell_full_period(cfg.a, cfg.c, cfg.m);
        cout << left
             << setw(4)  << (i + 1)
             << setw(19) << cfg.name
             << setw(15) << (hd ? "PASS" : "N/A or FAIL")
             << setw(17) << cfg.theoretical_period
             << cfg.basis
             << "\n";
    }
}

int main() {
    cout << "===============================================\n";
    cout << "  LINEAR CONGRUENTIAL GENERATOR - Assignment 03\n";
    cout << "  Yachay Tech | Analysis of Algorithms\n";
    cout << "===============================================\n\n";

    small_example();

    vector<Config> configs = {
        {"Small (m=16)",      "16",      1, 5,           3,           16,           true,  "16",  "exact cycle + Hull-Dobell"},
        {"Numerical Recipes", "2^32",    1, 1664525LL,   1013904223LL, 1LL << 32,   false, "m",   "Hull-Dobell"},
        {"MS Visual C++",     "2^31",    1, 214013LL,    2531011LL,    1LL << 31,   false, "m",   "Hull-Dobell"},
        {"Park-Miller",       "2^31-1",  1, 16807LL,     0LL,          2147483647LL,false, "m-1", "multiplicative LCG"},
        {"ANSI C",            "2^31",    1, 1103515245LL,12345LL,      1LL << 31,   false, "m",   "Hull-Dobell"},
    };

    empirical_analysis(configs, 10000);
    period_condition_report(configs);

    cout << "\nEach generated value costs O(1), so N samples cost O(N).\n";
    return 0;
}
