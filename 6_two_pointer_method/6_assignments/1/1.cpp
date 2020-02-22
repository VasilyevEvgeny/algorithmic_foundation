//
// Created by evasilyev on 20.02.2020.
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
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1) {
            os << " ";
        }
    }
    return os;
}


size_t greedy(size_t n, size_t dist, const std::vector<size_t>& monuments) {
    size_t ways = 0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i; j < n; ++j) {
            if (monuments[j] - monuments[i] > dist) {
                if (VERBOSE) { std::cout << "monuments[i] = " << monuments[i] << ", monuments[j] = " << monuments[j] << std::endl; }
                ways++;
            }
        }
    }

    return ways;
}


size_t fast(size_t n, size_t dist, const std::vector<size_t>& monuments) {
    size_t no_ways = 0;
    for (size_t l = 0, r = 0; l < n; ++l) {
        if (r == l) {  r++; }
        while (r < n && monuments[r] - monuments[l] <= dist) { r++; }
        no_ways += r - l - 1;

        if (VERBOSE) { std::cout << "l = " << l << ", r = " << r << ", no_ways = " << no_ways << std::endl; }
    }

    return size_t(n * (n - 1.0) / 2 - no_ways);
}



void stress_testing() {

    std::random_device rd;
    std::mt19937 gen(rd());

    const size_t MIN_N = 2, MAX_N = 10;
    std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

    const size_t MIN_R = 1, MAX_R = 20;
    std::uniform_int_distribution<> dist_R(MIN_R, MAX_R);

    const int MIN_D = 1, MAX_D = 20;
    std::uniform_int_distribution<> dist_D(MIN_D, MAX_D);

    while (true) {

        size_t n = dist_N(gen);
        size_t r = dist_R(gen);

        std::vector<size_t> monuments(n);
        for (size_t i = 0; i < n; ++i) {
            monuments[i] = dist_D(gen);
        }
        std::sort(monuments.begin(), monuments.end());

        auto res_greedy = greedy(n, r, monuments);
        auto res_fast = fast(n, r, monuments);

        std::cout << "n = " << n << ", r = " << r << ", monuments = [" << monuments << "]" <<
                  ", res_greedy = " << res_greedy <<", res_fast = " << res_fast << std::endl;

        assert(res_greedy == res_fast);

        monuments.clear();
    }
}



int main() {

#if MODE == 1
    size_t n = 0, r = 0;
    std::cin >> n >> r;

    std::vector<size_t> monuments(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> monuments[i];
    }

//    std::cout << greedy(n, r, monuments) << std::endl;
    std::cout << fast(n, r, monuments) << std::endl;

#elif MODE == 2
    stress_testing();

#endif


    return 0;
}