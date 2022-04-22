//
// Created by evasilyev on 18.02.2020.
//

#include <iostream>
#include <vector>
#include <random>
#include <cassert>

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


template <typename T>
bool operator== (const std::vector<T>& lhs, const std::vector<T>& rhs) {
    size_t n = std::max(lhs.size(), rhs.size());
    if (n != std::min(lhs.size(), rhs.size())) {
        return false;
    }

    for (size_t i = 0; i < n; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}


std::vector<size_t> greedy(const std::vector<size_t>& numbers, size_t X) {
    size_t n = numbers.size();
    for (size_t i = 0; i < n; ++i) {
        size_t sum = 0;
        for (size_t j = i; j < n; ++j) {
            sum += numbers[j];
            if (sum == X) {
                return {i + 1, j + 1};
            }
        }
    }

    return {1, 0};
}


std::vector<size_t> fast(const std::vector<size_t>& numbers, size_t X) {
    size_t n = numbers.size();
    for (size_t l = 0, r = 0, sum = 0; l < n; ++l) {
        while (sum < X && r < n) {
            sum += numbers[r];
            r++;
        }
        if (VERBOSE) { std::cout << "l = " << l << ", r = " << r << std::endl; }

        if (sum == X) {
            return {l + 1, r};
        }
        else {
            sum -= numbers[l];
        }
    }

    return {1, 0};
}


void stress_testing() {

    std::random_device rd;
    std::mt19937 gen(rd());

    const size_t MIN_N = 1, MAX_N = 10;
    std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

    const size_t MIN_X = 1, MAX_X = 10;
    std::uniform_int_distribution<> dist_X(MIN_X, MAX_X);

    const size_t MIN_VAL = 1, MAX_VAL = 10;
    std::uniform_int_distribution<> dist_VAL(MIN_VAL, MAX_VAL);

    while (true) {

        size_t N = dist_N(gen);
        size_t X = dist_X(gen);

        std::vector<size_t> numbers(N);

        for (size_t i = 0; i < N; ++i) {
            numbers[i] = dist_VAL(gen);
        }

        auto res_greedy = greedy(numbers, X);
        auto res_fast = fast(numbers, X);

        std::cout << "N = " << N << ", X = " << X << ", numbers = [" << numbers <<
                  "], res_greedy = " << res_greedy <<", res_fast = " << res_fast << std::endl;

        assert(res_greedy == res_fast);
    }
}




int main() {

#if MODE == 1
    size_t N = 0, X = 0;
    std::cin >> N >> X;

    std::vector<size_t> numbers(N);
    for (size_t i = 0; i < N; ++i) {
        std::cin >> numbers[i];
    }

//    std::cout << greedy(numbers, X) << std::endl;
    std::cout << fast(numbers, X) << std::endl;


#elif MODE == 2
    stress_testing();

#endif

    return 0;
}