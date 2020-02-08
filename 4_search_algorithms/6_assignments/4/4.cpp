//
// Created by evasilyev on 07.02.2020.
//

#include <iostream>
#include <algorithm>
#include <random>
#include <cassert>

// 1 - work, 2 - test
#define MODE 1

#define VERBOSE false

using integer = int64_t;

integer calculate_M(integer N, integer w, integer h) {
    integer weeks = N / (w + h);
    if (VERBOSE) { std::cout << "weeks = " << weeks << std::endl; }

    integer remainder = N % (w + h);
    if (VERBOSE) { std::cout << "remainder = " << remainder << std::endl; }

    return weeks * w + std::min(w, remainder);
}

integer find_weekends_binary(integer N, integer M, integer w) {

    integer l = 0, r = N;
    while (l <= r) {
        if (VERBOSE) { std::cout << "===============" << std::endl; }

        integer h = l + (r - l) / 2;
        if (VERBOSE) { std::cout << "h = " << h << std::endl; }

        integer m = calculate_M(N, w, h);
        if (VERBOSE) { std::cout << "m = " << m << std::endl; }

        if (m < M) {
            r = h - 1;
            if (VERBOSE) { std::cout << "r changes to " << r << std::endl; }
        } else if (m >= M) {
            l = h + 1;
            if (VERBOSE) { std::cout << "l changes to " << l << std::endl; }
        }
    }

    if (VERBOSE) { std::cout << "return h = " << std::max((integer)0, r) << std::endl; }
    return r;
}

integer find_weekends_greedy(integer N, integer M, integer w) {
    integer h_max = 0;
    for (integer h = 0; h < N; ++h) {
        integer m = calculate_M(N, w, h);
        if (m >= M) {
            h_max = std::max(h, h_max);
        }
    }

    return h_max;
}

void stress_testing() {
    std::random_device rd;
    std::mt19937 gen(rd());

    const integer MIN = 1, MAX = 1000;

    std::uniform_int_distribution<> dist_N(MIN, MAX);

    while (true) {
        std::cout << "=============================" << std::endl;

        integer N = dist_N(gen);

        std::uniform_int_distribution<> dist_M(MIN, N);
        integer M = dist_M(gen);

        std::uniform_int_distribution<> dist_w(MIN, M - 1);
        integer w = dist_w(gen);

        auto res_greedy = find_weekends_greedy(N, M, w);
        auto res_binary = find_weekends_binary(N, M, w);

        std::cout << "N = " << N << ", M = " << M << ", w = " << w << ", res_greedy = " << res_greedy <<", res_binary = " << res_binary << std::endl;
        assert(res_greedy == res_binary);
    }
}

int main() {

#if MODE == 1
    integer N = 0, M = 0, w = 0;
    std::cin >> N >> M >> w;

//    std::cout << find_weekends_binary(N, M, w);
    std::cout << find_weekends_greedy(N, M, w);

#elif MODE == 2
    stress_testing();
#endif

    return 0;
}