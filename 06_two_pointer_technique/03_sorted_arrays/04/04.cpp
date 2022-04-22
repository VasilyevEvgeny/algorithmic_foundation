#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cassert>

// 1 - work, 2 - stress testing
#define MODE 1

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


std::vector<size_t> greedy(const std::vector<int>& arr1, const std::vector<int>& arr2, int X) {
    for (size_t i = 0; i < arr1.size(); ++i) {
        for (size_t j = 0; j < arr2.size(); ++j) {
            if (arr1[i] + arr2[j] == X) {
                return {i + 1, j + 1};
            }
        }
    }

    return {0, 0};
}


std::vector<size_t> fast(const std::vector<int>& arr1, const std::vector<int>& arr2, int X) {
    for (int i = 0, j = (int)arr2.size() - 1; i < arr1.size(); ++i) {
        while (j >= 0) {
            if (arr1[i] + arr2[j] > X) { j--; }
            else { break; }
        }

        if (VERBOSE) { std::cout << "i = " << i << ", j = " << j << std::endl; }

        if (j >= 0) {
            if (arr1[i] + arr2[j] == X) {
                return {(size_t)i + 1, (size_t)j + 1};
            }
        }
    }

    return {0, 0};
}



void stress_testing() {

    std::random_device rd;
    std::mt19937 gen(rd());

    const size_t MIN_N = 1, MAX_N = 10;
    std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

    const size_t MIN_M = 1, MAX_M = 10;
    std::uniform_int_distribution<> dist_M(MIN_M, MAX_M);

    const int MIN_X = -20, MAX_X = 20;
    std::uniform_int_distribution<> dist_X(MIN_X, MAX_X);

    const int MIN_VAL = -10, MAX_VAL = 10;
    std::uniform_int_distribution<> dist_VAL(MIN_VAL, MAX_VAL);

    while (true) {

        size_t N = dist_N(gen);
        size_t M = dist_M(gen);
        int X = dist_X(gen);

        std::vector<int> arr1(N);
        for (size_t i = 0; i < N; ++i) {
            arr1[i] = dist_VAL(gen);
        }
        std::sort(arr1.begin(), arr1.end());

        std::vector<int> arr2(M);
        for (size_t i = 0; i < M; ++i) {
            arr2[i] = dist_VAL(gen);
        }
        std::sort(arr2.begin(), arr2.end());

        auto res_greedy = greedy(arr1, arr2, X);
        auto res_fast = fast(arr1, arr2, X);

        std::cout << "N = " << N << ", M = " << M << ", arr1 = [" << arr1 << "], arr2 = [" << arr2 <<
                  "], res_greedy = " << res_greedy <<", res_fast = " << res_fast << std::endl;

        if (res_greedy == std::vector<size_t>{0, 0} || res_fast == std::vector<size_t>{0, 0}) {
            assert(res_greedy == res_fast);
        }
        else {
            assert(arr1[res_greedy[0]-1] + arr2[res_greedy[1]-1] == X);
            assert(arr1[res_fast[0]-1] + arr2[res_fast[1]-1] == X);
        }

        arr1.clear();
        arr2.clear();

    }
}




int main () {

#if MODE == 1
    int N = 0, M = 0, X = 0;
    std::cin >> N >> M >> X;

    std::vector<int> arr1(N);
    for (size_t i = 0; i < N; ++i) {
        std::cin >> arr1[i];
    }

    std::vector<int> arr2(M);
    for (size_t i = 0; i < M; ++i) {
        std::cin >> arr2[i];
    }

//    std::cout << greedy(arr1, arr2, X) << std::endl;
    std::cout << fast(arr1, arr2, X) << std::endl;

#elif MODE == 2
    stress_testing();

#endif

    return 0;
}