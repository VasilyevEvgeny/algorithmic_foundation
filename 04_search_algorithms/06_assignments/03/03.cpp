#include <iostream>
#include <vector>
#include <algorithm>
#include <exception>
#include <cassert>
#include <random>

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

        int m = l + (r - l) / 2;

        if (VERBOSE) {
            std::cout << "m = " << m << std::endl;
        }

        if (h > meerkats[m]) {
            l = m + 1;

            if (VERBOSE) {
                std::cout << "l changed to " << l << std::endl;
            }
        }
        else  {
            r = m - 1;
            if (VERBOSE) {
                std::cout << "r changed to " << r << std::endl;
            }
        }
    }

    if (VERBOSE) {
        std::cout << "std::min((int)meerkats.size(), l) = " << std::min((int)meerkats.size(), l) << std::endl;
    }

    return std::min((int)meerkats.size(), l);
}

void stress_testing() {
    std::random_device rd;
    std::mt19937 gen(rd());

    const int INPUT_MIN = 1, INPUT_MAX = 10;
    const int VALUE_MIN = -10, VALUE_MAX = 10;

    std::uniform_int_distribution<> input(INPUT_MIN, INPUT_MAX), value(VALUE_MIN, VALUE_MAX);

    while (true) {
        std::cout << "=============================" << std::endl;

        int n = input(gen);
        int k = input(gen);

        auto meerkats = std::vector<int>(n, 0);
        auto marmots = std::vector<int>(k, 0);

        for (size_t i = 0; i < n; ++i) {
            meerkats[i] = value(gen);
        }
        sort(meerkats.begin(), meerkats.end());

        for (size_t i = 0; i < n; ++i) {
            std::cout << meerkats[i] << " ";
        }
        std::cout << "\n";

        for (size_t i = 0; i < k; ++i) {
            auto h = value(gen);
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
    stress_testing();
#endif

    return 0;
}