//
// Created by evasilyev on 14.02.2020.
//

#include <iostream>
#include <algorithm>
#include <random>
#include <cassert>

// 1 - work, 2 - stress testing
#define MODE 1

#define VERBOSE false

using integer = uint64_t;

integer linear(integer A, integer K, integer B, integer M, integer X) {
    integer day = 0;
    integer trees = 0;
    while (trees < X) {
        day++;

        trees += (day % K) ? A : 0;
        trees += (day % M) ? B : 0;
    }

    return day;
}

class Binary {
public:
    Binary(integer _A, integer _K, integer _B, integer _M, integer _X)
    : A(_A), K(_K), B(_B), M(_M), X(_X) {};

    integer solve() {
        integer l = 0;
        integer r = std::max({K, M, X});
        while (l <= r) {
            integer m = l + (r - l) / 2;
            if (VERBOSE) { std::cout << "m = " << m << std::endl; }

            integer trees_sum = count_trees_sum(m);
            if (VERBOSE) { std::cout << "trees_sum = " << trees_sum << std::endl; }
            if (trees_sum < X) {
                l = m + 1;
                if (VERBOSE) { std::cout << "l changes to " << l << std::endl; }
            }
            else {
                r = m - 1;
                if (VERBOSE) { std::cout << "r changes to " << r << std::endl; }
            }
        }

        if (VERBOSE) { std::cout << "return r = " << r << std::endl; }
        return l;
    }

private:

    static integer count_trees(integer day, integer trees_per_day, integer weekend_every) {
        integer number_of_cycles = day / weekend_every;
        integer prev = number_of_cycles * (weekend_every - 1) * trees_per_day;

        integer current = (day % weekend_every) * trees_per_day;

        return  prev + current;
    }

    integer count_trees_sum(integer day) {
        return count_trees(day, A, K) + count_trees(day, B, M);
    }

    const integer A, K, B, M, X;

};


void stress_testing() {

    std::random_device rd;
    std::mt19937 gen(rd());

    const integer MIN_AB = 1, MAX_AB = 1;
    const integer MIN_KM = 2, MAX_KM = 5;
    const integer MIN_X = 1, MAX_X = 20;

    std::uniform_int_distribution<> dist_AB(MIN_AB, MAX_AB);
    std::uniform_int_distribution<> dist_KM(MIN_KM, MAX_KM);
    std::uniform_int_distribution<> dist_X(MIN_X, MAX_X);

    integer A, K, B, M, X;
    while (true) {
        A = dist_AB(gen);
        B = dist_AB(gen);
        K = dist_KM(gen);
        M = dist_KM(gen);
        X = dist_X(gen);

        auto res_linear = linear(A, K, B, M, X);
        auto res_binary = Binary(A, K, B, M, X).solve();

        std::cout << "A = " << A << ", K = " << K << ", B = " << B << ", M = " << M << ", X = " << X <<
        ", res_linear = " << res_linear <<", res_binary = " << res_binary << std::endl;

        assert(res_linear == res_binary);
    }
}


int main() {

#if MODE == 1
    integer A = 0, K = 0, B = 0, M = 0, X = 0;
    std::cin >> A >> K >> B >> M >> X;

//    std::cout << linear(A, K, B, M, X) << std::endl;
    std::cout << Binary(A, K, B, M, X).solve() << std::endl;

#elif MODE == 2
    stress_testing();
#endif

    return 0;
}