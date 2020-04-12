//
// Created by evasilyev on 11.04.2020.
//

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <sstream>
#include <cassert>
#include <random>
#include <chrono>

// 1 - work, 2 - manual testing, 3 - stress testing
#define MODE 3
// 1 - EOF, 2 - \n
#define INPUT_MODE 2
// print debug information or not
#define VERBOSE false

using size = uint64_t;
using tt = std::chrono::steady_clock;

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

        if (n < 3) { return first_part_ + "0" + last_part_; }

        size k = 0;
        for (auto i = 0; i < n - 2; ++i) {
            for (auto j = i + 1; j < n - 1; ++j) {
                for (auto r = j + 1; r < n; ++r) {
                    bool is_triangle_exist = is_exist(sticks_[i], sticks_[j], sticks_[r]);
                    bool is_triangle_obtuse = is_obtuse(sticks_[i], sticks_[j], sticks_[r]);
                    if (is_triangle_exist && is_triangle_obtuse) { k++; }

                    if (VERBOSE) {
                        std::cout << "a = " << sticks_[i] << ", b = " << sticks_[j] << ", c = " << sticks_[r] << " --> ";
                        if (is_triangle_obtuse) { std::cout << "yes"; }
                        else { std::cout << "no"; }
                        std::cout << ", k --> " << k << std::endl;
                    }
                }
            }
        }

        return first_part_ + std::to_string(k) + last_part_;
    }

private:

    static bool is_exist(size a, size b, size c) {
        return (a + b > c) && (a + c > b) && (b + c > a);
    }

    static bool is_obtuse(size a, size b, size c) {
        // c - longest side

        return c * c > a * a + b * b;
    }

    std::vector<size> sticks_;
    const std::string first_part_ = "Sasha can compose ";
    const std::string last_part_ = " triangles";

};


class FastSolver {
public:
    explicit FastSolver(std::vector<size> sticks)
            : sticks_(std::move(sticks)) {}

    std::string solve() {
        std::set<size> s(sticks_.begin(), sticks_.end());
        sticks_.assign(s.begin(), s.end());
        size n = sticks_.size();

        if (VERBOSE) { std::cout << "sticks = " << sticks_ << std::endl; }

        if (n < 3) { return first_part_ + "0" + last_part_; }

        size n_it = 0;
        size max_it = 10;

        size is_ok = false;
        size k = 0;
        for (size i_a = 0, i_b = 1, i_c = 2; i_c < n; ++i_a, ++i_b) {
            while (is_exist(sticks_[i_a], sticks_[i_b], sticks_[std::min(n - 1, i_c + 1)]) &&
                   is_obtuse(sticks_[i_a], sticks_[i_b], sticks_[std::min(n - 1, i_c + 1)])) {
                is_ok = true;
                if (i_c == n - 1) { break; }
                else { i_c++; }
            }

            if (VERBOSE) { std::cout << "a = " << sticks_[i_a] << ", b = " << sticks_[i_b] << ", c --> " << sticks_[i_c] << std::endl; }

            if (is_ok) {
                k += i_c - i_b;
                if (VERBOSE) { std::cout << "k --> " << k << std::endl; }
            }


//            size b_min = sticks_[i_c] - sticks_[i_a] + 1;
//            size b_max = sticks_[i_c] - 1; //std::sqrt(std::pow(sticks_[i_c], 2) - std::pow(sticks_[i_a], 2));
//
//            if (VERBOSE) { std::cout << "b_min = " << b_min << ", b_max = " << b_max << std::endl; }
//
//            for (size i = i_b; i < i_c; ++i) {
//                if (sticks_[i] >= b_min && sticks_[i] <= b_max && is_obtuse(sticks_[i_a], sticks_[i], sticks_[i_c])) {
//                    k++;
//                    if (VERBOSE) { std::cout << "b = " << sticks_[i] << " is ok, k --> " << k << std::endl; }
//                }
//                else {
//                    if (VERBOSE) { std::cout << "b = " << sticks_[i] << " is not ok" << std::endl; }
//                }
//            }

            if (i_a == n - 3 && i_b == n - 2 && i_c == n - 1) { break; }
            is_ok = false;

            ++n_it;
            if (n_it > max_it) { break; }
        }

        return first_part_ + std::to_string(k) + last_part_;;
    }

private:

    static bool is_exist(size a, size b, size c) {
        return (a + b > c) && (a + c > b) && (b + c > a);
    }

    static bool is_obtuse(size a, size b, size c) {
        // c - longest side

        return c * c > a * a + b * b;
    }

    std::vector<size> sticks_;
    const std::string first_part_ = "Sasha can compose ";
    const std::string last_part_ = " triangles";
};


class ManualTester {
public:
    ManualTester() = default;

    static void test() {
        std::vector<std::map<std::string, std::string>> test_data = {

                // basic
                {{"sticks", "3 3 3"}, {"true", "0"}},
                {{"sticks", "3 4 5"}, {"true", "0"}},
                {{"sticks", "3 4 6"}, {"true", "1"}},
                {{"sticks", "3 4 5 6"}, {"true", "2"}},
                {{"sticks", "3 4 5 5 6"}, {"true", "2"}},

                // medium
                {{"sticks", "5 9 11 12"}, {"true", "2"}},

        };

        for (auto& line : test_data) {
            auto sticks = str_to_vector(line.at("sticks"));
            std::string res_true = "Sasha can compose " + line.at("true") + " triangles";
            auto pred_greedy = GreedySolver(sticks).solve();
            auto pred_fast = FastSolver(sticks).solve();

            std::cerr << "sticks = " << sticks << ", pred_greedy = " <<
                      pred_greedy << ", pred_fast = " << pred_fast << std::endl;

            assert(res_true == pred_greedy);
            assert(res_true == pred_fast);
        }

    }

private:

    static std::vector<size> str_to_vector(const std::string& str) {
        std::stringstream iss(str);
        std::vector<size> res;
        size a_i = 0;
        while (iss >> a_i)
            res.push_back(a_i);

        return res;
    }
};


void stress_testing() {
    std::random_device rd;
    std::mt19937 gen(rd());

    const size MIN_N = 1, MAX_N = 5;
    std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

    const size MIN_VAL = 1, MAX_VAL = 20;
    std::uniform_int_distribution<> dist_VAL(MIN_VAL, MAX_VAL);

    while (true) {
        std::cout << "======================" << std::endl;

        const size n_sticks = dist_N(gen);

        std::vector<size> sticks;
        for (size i = 0; i < n_sticks; ++i) { sticks.push_back(dist_VAL(gen)); }

        std::cout << "sticks = " << sticks << std::endl;

        tt::steady_clock::time_point begin_greedy = tt::now();
        auto res_greedy = GreedySolver(sticks).solve();
        tt::steady_clock::time_point end_greedy = tt::now();
        auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

        tt::steady_clock::time_point begin_fast = tt::now();
        auto res_fast = FastSolver(sticks).solve();
        tt::steady_clock::time_point end_fast = tt::now();
        auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();

        std::cout << std::scientific << "T = " << duration_greedy * 1e-9 << " s :: res_greedy = " << res_greedy << std::endl;
        std::cout << std::scientific << "T = " << duration_fast * 1e-9 << " s :: res_fast = " << res_fast << std::endl;

        assert(res_greedy == res_fast);

        sticks.clear();
    }
}


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

#if MODE == 1

#if INPUT_MODE == 1
    std::vector<size> sticks = get_sticks_release();
#elif INPUT_MODE == 2
    std::vector<size> sticks = get_sticks_debug();
#endif

//    std::cout << GreedySolver(sticks).solve() << std::endl;
    std::cout << FastSolver(sticks).solve() << std::endl;

#elif MODE == 2
    ManualTester::test();

#elif MODE == 3
    stress_testing();

#endif

}