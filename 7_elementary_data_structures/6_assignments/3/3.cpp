//
// Created by evasilyev on 03.05.2020.
//

#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <cmath>
#include <random>
#include <chrono>

// 1 - work, 3 - stress testing
#define MODE 3

#define VERBOSE false

using size = int64_t;
using tt = std::chrono::steady_clock;


template <typename T>
std::ostream& operator << (std::ostream& os, const std::deque<T>& dq) {
    if (dq.empty()) { os << "nullptr" << std::endl; }

    os << "nullptr -> ";
    size i = 1;
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        os << "#" << i << ": " << *it << " -> ";
        ++i;
    }
    os << "nullptr";

    return os;
}


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


template <typename T, typename U>
std::ostream& operator << (std::ostream& os, const std::map<T, U>& m) {
    os << "{";
    for (auto& e : m) {
        os << "{" << e.first << " : " << e.second << "}";
    }
    os << "}";
    return os;
}



class Enterprises {
public:
    Enterprises(size n, std::vector<size>& a, size k, std::vector<size>& e, std::vector<size>& v)
    : n_(n), a_(a), k_(k), e_(e), v_(v) {
        for (size i = 0; i < n_; ++i) {
            enterprises_.push_back(a_[i]);
            sum_of_squares += std::pow(a_[i], 2);
        }

        if (VERBOSE) { std::cout << "Enterprises initial: " << enterprises_ << std::endl; }
    }

    void make_evolution() {

#if MODE == 1
        std::cout << sum_of_squares << std::endl;
#endif
        for (size i = 0; i < k_; ++i) {

            if (e_[i] == 1) { bankrupt(v_[i]); }
            else if (e_[i] == 2) { divide(v_[i]); }

#if MODE == 1
            std::cout  << sum_of_squares << std::endl;
#endif

            if (VERBOSE) {
                std::cout << "enterprises_ = " << enterprises_ << std::endl;
                std::cout << "===========================================" << std::endl;
            }
        }

        if (VERBOSE) { std::cout << "Enterprises after evolution: " << enterprises_ << std::endl; }
    }

private:

    static std::pair<size, size> calculate_portions(size sum) {
        size first = sum / 2;
        size second = sum - first;
        if (VERBOSE) {
            std::cout << ".....first = " << first << std::endl;
            std::cout << ".....second = " << second << std::endl;
        }

        return {first, second};
    }

    void divide(size j) {
        if (VERBOSE) { std::cout << "===> go divide! j = " << j << std::endl; }
        auto it = enterprises_.begin() + j - 1;

        auto portions = calculate_portions(*it);

        // add first portion
        it = enterprises_.insert(it, portions.first);
        it = std::next(it);
        sum_of_squares += std::pow(portions.first, 2);
        if (VERBOSE) { std::cout << "add first portion squared, sum_of_squares --> " << sum_of_squares << std::endl; }

        // add second portion
        it = enterprises_.insert(it, portions.second);
        it = std::next(it);
        sum_of_squares += std::pow(portions.second, 2);
        if (VERBOSE) { std::cout << "add second portion squared, sum_of_squares --> " << sum_of_squares << std::endl; }

        // erase initial element
        sum_of_squares -= std::pow(*it, 2);
        enterprises_.erase(it);
        if (VERBOSE) { std::cout << "remove initial squared, sum_of_squares --> " << sum_of_squares << std::endl; }
    }

    void bankrupt(size j) {
        if (VERBOSE) { std::cout << "===> go bankrupt! j = " << j << std::endl; }
        if (j == 1) {
            auto it = enterprises_.begin();
            sum_of_squares -= std::pow(*it, 2);
            sum_of_squares -= std::pow(*std::next(it), 2);
            if (VERBOSE) { std::cout << "j = 1, remove cur and next squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            *std::next(it) += *it;
            sum_of_squares += std::pow(*std::next(it), 2);
            if (VERBOSE) { std::cout << "j = 1, add next squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            enterprises_.erase(it);
        }
        else if (j == enterprises_.size()) {
            auto it = enterprises_.end();
            sum_of_squares -= std::pow(*std::prev(std::prev(it)), 2);
            sum_of_squares -= std::pow(*std::prev(it), 2);
            if (VERBOSE) { std::cout << "j = size, remove prev-prev and prev squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            *std::prev(std::prev(it)) += *std::prev(it);
            sum_of_squares += std::pow(*std::prev(std::prev(it)), 2);
            if (VERBOSE) { std::cout << "j = size, add prev squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            enterprises_.erase(std::prev(it));
        }
        else {
            auto it = enterprises_.begin() + j - 1;

            auto portions = calculate_portions(*it);

            //cur
            sum_of_squares -= std::pow(*it, 2);
            if (VERBOSE) { std::cout << "remove cur squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            // prev
            sum_of_squares -= std::pow(*std::prev(it), 2);
            if (VERBOSE) { std::cout << "remove prev squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            *std::prev(it) += portions.first;
            sum_of_squares += std::pow(*std::prev(it), 2);
            if (VERBOSE) { std::cout << "add prev squared, sum_of_squares --> " << sum_of_squares << std::endl; }

            // next
            sum_of_squares -= std::pow(*std::next(it), 2);
            if (VERBOSE) { std::cout << "remove next squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            *std::next(it) += portions.second;
            sum_of_squares += std::pow(*std::next(it), 2);
            if (VERBOSE) { std::cout << "add next squared, sum_of_squares --> " << sum_of_squares << std::endl; }

            enterprises_.erase(it);
        }
    }

    const size n_;
    std::vector<size> a_;
    const size k_;
    std::vector<size> e_;
    std::vector<size> v_;

    std::deque<size> enterprises_;
    size sum_of_squares = 0;
};


class StressTester {
public:
    StressTester() = default;

    static void test() {
        std::random_device rd;
        std::mt19937 gen(rd());

        const size MIN_N = 50000, MAX_N = 100000;
        std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

        const size MIN_VAL_A = 500, MAX_VAL_A = 1000;
        std::uniform_int_distribution<> dist_VAL_A(MIN_VAL_A, MAX_VAL_A);

        const size MIN_K = 100000, MAX_K = 100000;
        std::uniform_int_distribution<> dist_K(MIN_K, MAX_K);

        const size MIN_E = 1, MAX_E = 2;
        std::uniform_int_distribution<> dist_E(MIN_E, MAX_E);

        const size MIN_V = 25000, MAX_V = 25000;
        std::uniform_int_distribution<> dist_V(MIN_V, MAX_V);

        while (true) {
            std::cout << "======================" << std::endl;

            size n = dist_N(gen);
            std::vector<size> a(n);
            for (size i = 0; i < n; ++i) { a[i] = dist_VAL_A(gen); }

            size k = dist_K(gen);
            std::vector<size> e(k), v(k);
            for (size i = 0; i < k; ++i) {
                e[i] = dist_E(gen);
                v[i] = dist_V(gen);
            }

            std::cout << "n = " << n << std::endl; //<< ", a = " << a << std::endl;
            std::cout << "k = " << k << std::endl; //", ev = " << ev << std::endl;

            tt::steady_clock::time_point begin = tt::now();
            Enterprises(n, a, k, e, v).make_evolution();
            tt::steady_clock::time_point end = tt::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();

            std::cout << std::scientific << "T = " << duration * 1e-9 << " s" << std::endl;

            if (duration * 1e-9 > 3.0) { throw std::runtime_error("Solution is too slow: " + std::to_string(duration * 1e-9) + " s"); }

            a.clear();
            e.clear();
            v.clear();
        }
    }

private:

};


int main() {

#if MODE == 1
    size n = 0;
    std::cin >> n;

    std::vector<size> a(n);
    for (size i = 0; i < n; ++i) { std::cin >> a[i]; }

    size k = 0;
    std::cin >> k;

    std::vector<size> e(k), v(k);
    for (size i = 0; i < k; ++i) {
        size e_cur = 0, v_cur = 0;
        std::cin >> e_cur;
        std::cin >> v_cur;
        e[i] = e_cur;
        v[i] = v_cur;
    }

    if (VERBOSE) {
        std::cout << "e = " << e << std::endl;
        std::cout << "v = " << v << std::endl;
    }

    Enterprises(n, a, k, e, v).make_evolution();

#elif MODE == 3
    StressTester::test();

#endif

    return 0;
}