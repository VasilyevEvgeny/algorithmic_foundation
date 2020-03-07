//
// Created by evasilyev on 27.02.2020.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <cassert>

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


size_t get_k() {
    std::string k_str;
    std::getline (std::cin, k_str);
    size_t k = 0;
    std::stringstream(k_str) >> k;

    return k;
}

std::vector<size_t> get_v() {
    std::string s;
    std::getline (std::cin, s);
    std::vector<size_t> v;
    for (auto e : s) { v.push_back(e - '0'); }

    return v;
}

bool check(size_t k, std::vector<size_t>& sub) {
    size_t k_local = 0;
    for (const auto& e : sub) {
        if (e == 1) { k_local++; }
        if (k_local > k) {
            return false;
        }
    }

    return k_local == k;
}

size_t fast(size_t k, std::vector<size_t>& v) {
    size_t sum = 0;
    for (auto l = 0; l < v.size(); ++l) {
        size_t r = l;
        do {
            std::vector<size_t> sub{v.begin() + l, v.begin() + r + 1};
            if (VERBOSE) { std::cout << "sub = " << sub << ", l = " << l << ", r = " << r << std::endl; }
            if (check(k, sub)) {
                sum++;
                if (VERBOSE) { std::cout << "true! sum = " << sum << std::endl; }
            }
            r++;
        } while (r < v.size());
    }

    return sum;
}

size_t greedy(size_t k, std::vector<size_t>& v) {
    size_t sum = 0;
    for (auto l = 0; l < v.size(); ++l) {
        for (auto r = l; r < v.size(); ++r) {
            size_t k_local = 0;
            for (size_t i = l; i <= r; ++i) {
                if (v[i] == 1) { k_local++; }
            }
            if (k_local == k) { sum++; }
        }
    }

    return sum;
}


void stress_testing() {

    std::random_device rd;
    std::mt19937 gen(rd());

    const size_t MIN_K = 0, MAX_K = 10;
    std::uniform_int_distribution<> dist_K(MIN_K, MAX_K);

    const size_t MIN_VAL = 0, MAX_VAL = 1;
    std::uniform_int_distribution<> dist_VAL(MIN_VAL, MAX_VAL);

    const int MIN_L = 1, MAX_L = 10000;
    std::uniform_int_distribution<> dist_L(MIN_L, MAX_L);

    while (true) {
        std::cout << "======================" << std::endl;

        size_t k = dist_K(gen);
        size_t length = dist_L(gen);

        std::vector<size_t> v;
        for (auto i = 0; i < length; ++i) { v.push_back(dist_VAL(gen)); }

        auto res_greedy = 1; //greedy(k, v);
        auto res_fast = fast(k, v);

        std::cout << "k = " << k << ", v = " << v <<
                  ", res_greedy = " << res_greedy <<", res_fast = " << res_fast << std::endl;

//        assert(res_greedy == res_fast);

        v.clear();
    }
}



int main() {

#if MODE == 1

    auto k = get_k();
//    std::cout << k << std::endl;
    auto v = get_v();
//    std::cout << v << std::endl;

    //    std::cout << greedy(k, v) << std::endl;
    std::cout << fast(k, v) << std::endl;

#elif MODE == 2
    stress_testing();

#endif

    return 0;
}