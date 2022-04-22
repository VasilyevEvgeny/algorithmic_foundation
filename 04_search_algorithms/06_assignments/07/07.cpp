#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>
#include <cassert>

// 1 - work, 2 - stress testing
#define MODE 1

#define VERBOSE false

using integer = int64_t;


template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p) {
    os << "{" << p.first << ", " << p.second << "}";
    return os;
}

template <typename T>
inline std::ostream& operator << (std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    for (const auto& item : v) {
        os << " " << item;
    }
    os << " ]";
    return os;
}


std::string round_with_precision(double value, const integer p) {
    integer power = std::pow(10, p);
    double res = std::round(value * power) / power;

    std::ostringstream os;
    os << std::fixed << std::setprecision(p) << res;

    return os.str();
}


class StraightSolver {

public:

    StraightSolver(const integer _n, const integer _k, std::vector<integer>& _vs, std::vector<integer>& _ws)
    : n(_n), k(_k), vs(_vs), ws(_ws) {
        if (VERBOSE) {
            std::cout << "vs: " << vs << std::endl;
            std::cout << "ws: " << ws << std::endl;
        }
    };

    ~StraightSolver() = default;

    double solve() const {
        std::string bitmask(k, 1); // K leading 1's
        bitmask.resize(n, 0); // N-K trailing 0's

        double metrics_max = 0.0;

        do {
            double v = 0.0, w = 0.0;
            for (size_t i = 0; i < n; ++i) // [0..N-1] integers
            {
                if (bitmask[i]) {
                    v += vs[i];
                    w += ws[i];
                    if (VERBOSE) { std::cout << " " << i; }
                }
            }
            double metrics = v / w;
            metrics_max = std::max(metrics, metrics_max);
            if (VERBOSE) { std::cout << ", metrics = " << metrics << std::endl; }
        } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

        return metrics_max;
    }

private:

    const integer n, k;
    const std::vector<integer> vs, ws;
};



class BinarySolver {

public:

    BinarySolver(const integer _n, const integer _k, std::vector<integer>& _vs, std::vector<integer>& _ws)
    : n(_n), k(_k), vs(_vs), ws(_ws) {
        if (VERBOSE) {
            std::cout << "vs: " << vs << std::endl;
            std::cout << "ws: " << ws << std::endl;
        }
    };

    bool check(const double x, std::vector<double>& comb) const {
        for (size_t i = 0; i < n; ++i) {
            comb[i] = vs[i] - x * ws[i];
        }

        std::sort(comb.begin(), comb.end());

        double sum = 0.0;
        for (size_t i = n - k; i < n; ++i) {
            sum += comb[i];
        }

        return sum > 0.0;
    }

    double solve() const {
        std::vector<double> comb(n);

        const double eps = 1e-7;

        double l = 0.0, r = 1e7;
        while (r - l > eps) {
            const double m = (l + r) / 2;
            if (check(m, comb)) {
                l = m + eps;
            }
            else {
                r = m - eps;
            }
        }

        return l;
    }


private:

    const integer n, k;
    const std::vector<integer> vs, ws;
};



void stress_testing() {

    const integer p = 5;

    std::random_device rd;
    std::mt19937 gen(rd());

    while (true) {

        const integer MIN_N = 1, MAX_N = 5;
        std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);
        integer n = dist_N(gen);

        const integer MIN_K = 1, MAX_K = n;
        std::uniform_int_distribution<> dist_K(MIN_K, MAX_K);
        integer k = dist_K(gen);

        std::vector<integer> vs(n);
        const integer MIN_V = 0, MAX_V = 3;
        std::uniform_int_distribution<> dist_V(MIN_V, MAX_V);
        for (size_t i = 0; i < n; ++i) {
            vs[i] = dist_V(gen);
        }

        std::vector<integer> ws(n);
        const integer MIN_W = 1, MAX_W = 3;
        std::uniform_int_distribution<> dist_W(MIN_W, MAX_W);
        for (size_t i = 0; i < n; ++i) {
            ws[i] = dist_W(gen);
        }

        auto res_straight = round_with_precision(StraightSolver(n, k, vs, ws).solve(), p);
        auto res_binary = round_with_precision(BinarySolver(n, k, vs, ws).solve(), p);

        std::cout << "n = " << n << ", k = " << k << ", vs = " << vs << ", ws = " << ws <<
                  ", res_straight = " << res_straight <<", res_binary = " << res_binary << std::endl;

        assert(res_straight == res_binary);
    }
}


int main() {

#if MODE == 1
    const integer p = 5;

    integer n = 0, k = 0;
    std::cin >> n >> k;

    auto vs = std::vector<integer>(n, 0);
    auto ws = std::vector<integer>(n, 0);

    for (size_t i = 0; i < n; ++i) {
        std::cin >> vs[i];
        std::cin >> ws[i];
    }

//    std::cout << round_with_precision(StraightSolver(n, k, vs, ws).solve(), p) << std::endl;
    std::cout << round_with_precision(BinarySolver(n, k, vs, ws).solve(), p) << std::endl;

#elif MODE == 2
    stress_testing();

#endif

    return 0;
}