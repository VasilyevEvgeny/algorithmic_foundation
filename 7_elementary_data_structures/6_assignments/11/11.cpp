//
// Created by evasilyev on 29.05.2020.
//

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#define VERBOSE false

using size = int64_t;


class Greedy {
public:
    Greedy(size N, std::vector<size> numbers, size K, std::vector<size> rounds)
    : N_(N), numbers_(std::move(numbers)), K_(K), rounds_(std::move(rounds)) {}

    size solve() {
        size sum = 0;

        for (size i = 0; i < K_; ++i) {
            size a = rounds_[i];
            size max = -1e8;
            for (size j = 0; j < N_ - a + 1; ++j) {
                auto cur_min = *std::min_element(numbers_.begin() + j, numbers_.begin() + j + a);
                max = std::max(max, cur_min);

                if (VERBOSE) { std::cout << "[ " << numbers_[j] << "..." << numbers_[j + a - 1] << "], cur_min = " << cur_min << ", max --> " << max << std::endl; }
            }
            sum += max;
            if (VERBOSE) { std::cout << "sum --> " << sum << std::endl; }
        }

        return sum;
    }

private:
    const size N_;
    const std::vector<size> numbers_;

    const size K_;
    const std::vector<size> rounds_;
};


class Fast {
public:
    Fast(size N, std::vector<size> numbers, size K, std::vector<size> rounds)
            : N_(N), numbers_(std::move(numbers)), K_(K), rounds_(std::move(rounds)) {}

    size solve() {
        size sum = 0;

        for (size i = 0; i < K_; ++i) {
            size a = rounds_[i];

        }

        return sum;
    }

private:
    const size N_;
    const std::vector<size> numbers_;

    const size K_;
    const std::vector<size> rounds_;
};



int main() {

    size N = 0, K = 0;
    std::cin >> N >> K;

    std::vector<size> numbers(N);
    for (size i = 0; i < N; ++i) { std::cin >> numbers[i]; }

    std::vector<size> rounds(K);
    for (size i = 0; i < K; ++i) { std::cin >> rounds[i]; }

    std::cout << Greedy(N, numbers, K, rounds).solve() << std::endl;


    return 0;
}