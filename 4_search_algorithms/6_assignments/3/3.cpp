//
// Created by evasilyev on 06.02.2020.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <exception>
#include <cassert>

// 1 - work, 2 - test
#define MODE 1

// work
#define VERBOSE false

int find_place_greedy(std::vector<int>& meerkats, int h) {
    for (size_t i = 0; i < meerkats.size(); ++i) {
        if (h <= meerkats[i]) {
            return i;
        }
    }

    return meerkats.size();
}

int find_place_binary(std::vector<int>& meerkats, int h) {
    if (VERBOSE) {
        std::cout << "=================" << std::endl;
    }

    int l = 0;
    int r = meerkats.size();
    while (l <= r) {
        if (l == r) {
            if (h <= meerkats[l]) {
                return l;
            }
            else {
                return std::min({(int)meerkats.size(), l + 1});
            }
        }

        int m = l + (r - l) / 2;

        if (VERBOSE) {
            std::cout << "m = " << m << std::endl;
        }

        if (h == meerkats[m]) {
            if (VERBOSE) {
                std::cout << "return m = " << m << std::endl;
            }

            return m;
        }
        if (h < meerkats[m]) {
            r = std::max(0, m - 1);
            if (VERBOSE) {
                std::cout << "r changed to " << r << std::endl;
            }
        }
        else {
            l = m + 1;
            if (VERBOSE) {
                std::cout << "l changed to " << l << std::endl;
            }
        }
    }

    if (VERBOSE) {
        std::cout << "return l = " << l << std::endl;
    }

    return l;

}

void extreme_testing() {

    const int INPUT_AMBIT = 1;
    const int INPUT_AMPLITUDE = 101;

    const int VALUE_AMBIT = -1000000000;
    const int VALUE_AMPLITUDE = 2000000000;

    while (true) {
        std::cout << "=============================" << std::endl;

        int n = rand() % INPUT_AMPLITUDE + INPUT_AMBIT;
        int k = rand() % INPUT_AMPLITUDE + INPUT_AMBIT;

        auto meerkats = std::vector<int>(n, 0);
        auto marmots = std::vector<int>(k, 0);

        int epochs = 0;
        while (epochs < n) {
            auto val = rand() % VALUE_AMPLITUDE + VALUE_AMBIT;
            auto occurrences = std::count(meerkats.begin(), meerkats.begin() + epochs + 1, val);
            if (!occurrences) {
                meerkats[epochs] = val;
                epochs++;
            }
        }
        sort(meerkats.begin(), meerkats.end());

        for (size_t i = 0; i < n; ++i) {
            std::cout << meerkats[i] << " ";
        }
        std::cout << "\n";

        for (size_t i = 0; i < k; ++i) {
            auto h = rand() % VALUE_AMPLITUDE + VALUE_AMBIT;
            auto res_greedy = find_place_greedy(meerkats, h);
            auto res_binary = find_place_binary(meerkats, h);
            std::cout << "h = " << h << ", res_greedy = " << res_greedy << ", res_binary = " << res_binary << std::endl;
            assert(res_greedy == res_binary);
        }
    }
}

int main() {

#if MODE == 1
    int n = 0, k = 0;
    std::cin >> n >> k;

    auto meerkats = std::vector<int>(n, 0);
    auto marmots = std::vector<int>(k, 0);

    for (size_t i = 0; i < n; ++i) {
        std::cin >> meerkats[i];
    }

    for (size_t i = 0; i < k; ++i) {
        std::cin >> marmots[i];
    }

    for (size_t i = 0; i < k; ++i) {
        //std::cout << find_place_greedy(meerkats, marmots[i]) << std::endl;
        std::cout << find_place_binary(meerkats, marmots[i]) << std::endl;
    }
#elif MODE == 2
    extreme_testing();
#endif

    return 0;
}