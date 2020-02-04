//
// Created by evasilyev on 03.02.2020.
//

#include <iostream>
#include <deque>
#include <algorithm>

std::ostream& operator<<(std::ostream& os, const std::deque<int>& v)
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

int main() {

    int N = 0, K = 0, M = 0;
    std::cin >> N >> K >> M;

    int L = 0;
    std::cin >> L;
    auto cafes = std::deque<int>(L, 0);

    for (size_t i = 0; i < L; ++i) {
        std::cin >> cafes[i];
    }
    cafes.push_front(0);
    cafes.push_back(N);

    std::sort(cafes.begin(), cafes.end());

//    std::cout << cafes << std::endl;

    for (size_t i = 0; i < L + 1; ++i) {
        if (K < 0) {
            break;
        }

        int delta = cafes[i + 1] - cafes[i];
//        std::cout << "delta = " << delta << std::endl;
        if (delta <= M) {
            continue;
        } else {
            int dK = 0;
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

    if (K >= 0) {
        std::cout << "YES";
    }
    else {
        std::cout << "NO";
    }


    return 0;
}