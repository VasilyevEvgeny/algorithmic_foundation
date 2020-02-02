//
// Created by evasilyev on 26.01.2020.
//

#include <iostream>
#include <vector>


std::ostream& operator<<(std::ostream& os, const std::vector<int>& v)
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

int FindTargetNumber(std::vector<int>& v, int x) {
    int l = 0;
    int r = (int)v.size() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (x < v[m]) {
            r = m - 1;
        } else {
            l = m + 1;
        }
    }

    return (int)v.size() - l;

}

int main() {
    int N = 0;
    std::cin >> N;

    std::vector<int> v(N);
    for (size_t i = 0; i < N; ++i) {
        std::cin >> v[i];
    }

    int M = 0;
    std::cin >> M;

    int x = 0;
    for (size_t i = 0; i < M; ++i) {
        std::cin >> x;
        std::cout << FindTargetNumber(v, x) << std::endl;
    }

    return 0;
}
