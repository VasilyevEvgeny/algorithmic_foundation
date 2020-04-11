//
// Created by evasilyev on 11.04.2020.
//

#include <iostream>
#include <vector>
#include <string>
#include <set>

// 1 - EOF, 2 - \n
#define INPUT_MODE 1
#define VERBOSE false

using size = uint64_t;

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
    explicit GreedySolver(std::vector<size> sticks)
    : sticks_(std::move(sticks)) {}

    std::string solve() {
        std::set<size> s(sticks_.begin(), sticks_.end());
        sticks_.assign(s.begin(), s.end());
        size n = sticks_.size();

        size k = 0;
        for (auto i = 0; i < n - 2; ++i) {
            for (auto j = i + 1; j < n - 1; ++j) {
                for (auto r = j + 1; r < n; ++r) {
                    bool is_triangle_obtuse = is_obtuse(sticks_[i], sticks_[j], sticks_[r]);
                    if (is_triangle_obtuse) { k++; }

                    if (VERBOSE) {
                        std::cout << "a = " << sticks_[i] << ", b = " << sticks_[j] << ", c = " << sticks_[r] << " --> ";
                        if (is_triangle_obtuse) { std::cout << "ya"; }
                        else { std::cout << "no"; }
                        std::cout << ", k --> " << k << std::endl;
                    }

                }
            }
        }

        return "Sasha can compose " + std::to_string(k) + " triangles";
    }

private:

    static bool is_obtuse(size a, size b, size c) {
        // c - longest side

        return c * c > a * a + b * b;
    }

    std::vector<size> sticks_;

};




std::vector<size> get_sticks_release() {
    std::vector<size> sticks;
    size val;
    while (std::cin >> val) { sticks.push_back(val); }

    return sticks;
}

std::vector<size> get_sticks_debug() {
    std::vector<size> sticks;
    std::string line;
    while (getline(std::cin, line)) {
        if (!line.empty()) { sticks.push_back(static_cast<size>(std::stoi(line))); }
        else { break; }
    }

    return sticks;
}



int main () {

#if INPUT_MODE == 1
    std::vector<size> sticks = get_sticks_release();
#elif INPUT_MODE == 2
    std::vector<size> sticks = get_sticks_debug();
#endif

    std::cout << GreedySolver(sticks).solve() << std::endl;

}