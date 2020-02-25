//
// Created by evasilyev on 22.02.2020.
//


#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include <algorithm>

// 1 - work, 2 - stress testing
#define MODE 2


#define VERBOSE false


template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) {
    os << "[ ";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1) {
            os << " ";
        }
    }
    os << " ]";
    return os;
}



size_t greedy(const std::vector<size_t>& drills, const std::vector<size_t>& dowels) {
    size_t min_diff = std::max(drills[drills.size()-1], dowels[dowels.size()-1]);
    for (const auto& drill : drills) {
        for (const auto& dowel : dowels) {
            size_t diff = std::abs((int)drill - (int)dowel);
            if (!diff) {
                return 0;
            }
            if (diff < min_diff) {
                min_diff = diff;
            }
        }
    }

    return min_diff;
}

void update_min_diff(size_t a, size_t b, size_t& min_diff) {
    min_diff = std::min((size_t)std::abs((int)a - (int)b), min_diff);
}

size_t fast(const std::vector<size_t>& drills, const std::vector<size_t>& dowels) {
    size_t n = drills.size();
    size_t m = dowels.size();
    size_t min_diff = 1000000000;
    for (size_t i = 0, j = 0; i < n; ++i) {
        while (dowels[j] <= drills[i]) {
            update_min_diff(drills[i], dowels[j], min_diff);

            if (VERBOSE) { std::cout << "check drills[" << i << "] = " << drills[i] << " and dowels[" << j << "] = "
            << dowels[j] << std::endl; }

            if (j < m - 1) { j++; }
            else { break; }
        }

        update_min_diff(drills[i], dowels[j], min_diff);

        if (i == n - 1 && j <= m - 1) {
            for (size_t k = j; k < m; ++k) {
                update_min_diff(drills[i], dowels[j], min_diff);
                if (VERBOSE) { std::cout << "check drills[" << i << "] = " << drills[i] << " and dowels[" << k << "] = "
                                         << dowels[k] << std::endl; }
            }
        }

        if (VERBOSE) { std::cout << "i = " << i << ", j = " << j << ", min_diff = " << min_diff << std::endl; }
    }

    return min_diff;
}


void stress_testing() {

    std::random_device rd;
    std::mt19937 gen(rd());

    const size_t MIN_N = 1, MAX_N = 10;
    std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

    const size_t MIN_M = 1, MAX_M = 10;
    std::uniform_int_distribution<> dist_M(MIN_M, MAX_M);

    const int MIN_VAL = 1, MAX_VAL = 20;
    std::uniform_int_distribution<> dist_VAL(MIN_VAL, MAX_VAL);

    while (true) {
        std::cout << "======================" << std::endl;

        size_t n = dist_N(gen);
        size_t m = dist_M(gen);

        std::vector<size_t> drills(n);
        for (size_t i = 0; i < n; ++i) {
            drills[i] = dist_VAL(gen);
        }
        std::sort(drills.begin(), drills.end());

        std::vector<size_t> dowels(m);
        for (size_t i = 0; i < m; ++i) {
            dowels[i] = dist_VAL(gen);
        }
        std::sort(dowels.begin(), dowels.end());

        auto res_greedy = greedy(drills, dowels);
        auto res_fast = fast(drills, dowels);

        std::cout << "drills = " << drills << ", dowels = " << dowels <<
                  ", res_greedy = " << res_greedy <<", res_fast = " << res_fast << std::endl;

        assert(res_greedy == res_fast);

        drills.clear();
        dowels.clear();
    }
}


int main() {

#if MODE == 1
    size_t n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<size_t> drills(n), dowels(m);

    for (size_t i = 0; i < n; ++i) {
        std::cin >> drills[i];
    }
    for (size_t i = 0; i < m; ++i) {
        std::cin >> dowels[i];
    }

//    std::cout << greedy(drills, dowels) << std::endl;
    std::cout << fast(drills, dowels) << std::endl;

#elif MODE == 2
    stress_testing();

#endif

    return 0;
}