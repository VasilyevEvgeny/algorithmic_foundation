//
// Created by evasilyev on 27.02.2020.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

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


size_t get_k() {
    std::string k_str;
    std::getline (std::cin, k_str);
    size_t k = 0;
    std::stringstream(k_str) >> k;

    return k;
}

std::vector<size_t> get_v() {
    std::string s;
    std::getline (std::cin, s);
    std::vector<size_t> v;
    for (auto e : s) { v.push_back(e - '0'); }

    return v;
}

size_t greedy(size_t k, std::vector<size_t>& v) {
    size_t sum = 0;
    for (auto l = 0; l < v.size(); ++l) {
        for (auto r = l; r < v.size(); ++r) {
            size_t k_local = 0;
            for (size_t i = l; i <= r; ++i) {
                if (v[i] == 1) { k_local++; }
            }
            if (k_local == k) { sum++; }
        }
    }

    return sum;
}

int main() {

    auto k = get_k();
//    std::cout << k << std::endl;
    auto v = get_v();
//    std::cout << v << std::endl;

    std::cout << greedy(k, v) << std::endl;

    return 0;
}