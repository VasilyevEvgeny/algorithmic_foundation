//
// Created by evasilyev on 01.04.2020.
//

#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) {
    os << "[ ";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1) {
            os << " ";
        }
    }
    os << " ]";
    return os;
}


class GreedySolver {
public:
    GreedySolver(size_t N, std::vector<size_t>& a, size_t M)
    : N_(N), a_(a), M_(M) {}



private:
    const size_t N_;
    const std::vector<size_t> a_;
    const size_t M_;

    
};


int main() {

    size_t N = 0, M = 0;
    std::cin >> N >> M;

    std::vector<size_t> a(N);
    for (auto i = 0; i < N; ++i) {
        std::cin >> a[i];
    }

    std::cout << "N = " << N << std::endl;
    std::cout << "M = " << M << std::endl;
    std::cout << "a = " << a << std::endl;


    return 0;
}