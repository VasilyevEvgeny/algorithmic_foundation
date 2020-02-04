//
// Created by evasilyev on 03.02.2020.
//

#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
#include <string>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::deque<T>& d)
{
    os << "[";
    for (size_t i = 0; i < d.size(); ++i) {
        os << d[i];
        if (i != d.size() - 1)
            os << ", ";
    }
    os << "]\n";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1)
            os << ", ";
    }
    os << "]\n";
    return os;
}

std::string fast(uint16_t N, int16_t K, uint16_t M, uint16_t L) {
    auto cafes = std::deque<uint16_t>(L, 0);

    for (size_t i = 0; i < L; ++i) {
        std::cin >> cafes[i];
    }
    cafes.push_front(0);
    cafes.push_back(N);

    std::sort(cafes.begin(), cafes.end());

    for (size_t i = 0; i < L + 1; ++i) {
        if (K < 0) {
            break;
        }

        uint16_t delta = cafes[i + 1] - cafes[i];
//        std::cout << "delta = " << delta << std::endl;
        if (delta <= M) {
            continue;
        } else {
            int16_t dK = 0;
            if (!(delta % M)) {
                dK = delta / M - 1;
            } else {
                dK = delta / M;
            }
//            std::cout << "dK = " << dK << std::endl;

            K -= dK;
//            std::cout << "K = " << K << std::endl;
        }
    }

    return (K >= 0) ? "YES" : "NO";
}

std::string greedy(uint16_t N, int16_t K, uint16_t M, uint16_t L) {
    auto pillars = std::vector<bool>(N + 1, false);

    for (size_t i = 0; i < L; ++i) {
        uint16_t idx = 0;
        std::cin >> idx;
        pillars[idx] = true;
    }
    pillars[0] = true;
    pillars[N] = true;

//    std::cout << pillars << std::endl;

    uint16_t left = 0;
    for (size_t i = 0; i < N + 1; ++i) {
        if (pillars[i]) {
            left = i;
//            std::cout << "left = " << i << std::endl;
            continue;
        }
        uint16_t delta = i - left;
//        std::cout << "delta = " << delta << std::endl;
        if (delta >= M) {
            K--;
            left = i;
//            std::cout << "K = " << K << std::endl;
//            std::cout << "left = " << left << std::endl;
        }
    }

    return (K >= 0) ? "YES" : "NO";
}

int main() {

    uint16_t N = 0, M = 0;
    int16_t K = 0;
    std::cin >> N >> K >> M;

    uint16_t L = 0;
    std::cin >> L;

    std::cout << fast(N, K, M, L);
    //std::cout << greedy(N, K, M, L);

    return 0;
}