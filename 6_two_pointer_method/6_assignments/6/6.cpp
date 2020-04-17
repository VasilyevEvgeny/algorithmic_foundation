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
#include <stdexcept>
#include <random>
#include <chrono>
#include <algorithm>

// 1 - work, 2 - manual testing, 3 - stress testing
#define MODE 1
// 1 - EOF, 2 - \n
#define INPUT_MODE 1
// print debug information or not
#define VERBOSE false

using size = int64_t;
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
        std::sort(sticks_.begin(), sticks_.end());
        size n = sticks_.size();

        std::set<std::vector<size>> triangles;

        if (n < 3) { return generate_answer(0); }

        for (auto i_a = 0; i_a < n - 2; ++i_a) {
            for (auto i_b = i_a + 1; i_b < n - 1; ++i_b) {
                for (auto i_c = i_b + 1; i_c < n; ++i_c) {
                    auto a = sticks_[i_a];
                    auto b = sticks_[i_b];
                    auto c = sticks_[i_c];

                    if (exists(a, b, c)) {
                        if (is_obtuse(a, b, c)) {
                            auto size_before = triangles.size();
                            triangles.insert({a, b, c});
                            auto size_after = triangles.size();

                            if (VERBOSE) {
                                std::cout << "a = " << a << ", b = " << b << ", c = " << c << " --> ";
                                if (size_after - size_before) { std::cout << "yes" << ", k --> " << triangles.size(); }
                                else { std::cout << "no"; }
                                std::cout << std::endl;
                            }
                        }
                    }
                }
            }
        }

        return generate_answer(triangles.size());
    }

private:

    static bool exists(size a, size b, size c) {
        return (a + b > c) && (a + c > b) && (b + c > a);
    }

    static bool is_obtuse(size a, size b, size c) {
        // c - longest side
        return c * c > a * a + b * b;
    }

    static std::string generate_answer(size k) {
        return "Sasha can compose " + std::to_string(k) + " triangles";
    }

    std::vector<size> sticks_;
};


class FastSolver {
public:
    explicit FastSolver(std::vector<size> sticks)
            : sticks_(std::move(sticks)) {}

    std::string solve() {
        std::sort(sticks_.begin(), sticks_.end());
        size n = sticks_.size();

        if (VERBOSE) { std::cout << "sticks = " << sticks_ << std::endl; }

        if (n < 3) { return generate_answer(0); }

        size k = 0;

        // unique
        std::set<size> tmp(sticks_.begin(), sticks_.end());
        std::vector<size> sticks_unique(tmp.begin(), tmp.end());

        size last_a = 0, last_b = 0;
        for (size i_a = 0; i_a < n - 2; ++i_a) {
            if (i_a) { if (sticks_[i_a] == sticks_[i_a - 1]) { continue; }}
            for (size i_b = i_a + 1; i_b < n - 1; ++i_b) {
                if ((sticks_[i_a] == last_a) && (sticks_[i_b] == last_b)) { continue; }
                if ((sticks_[i_b] == sticks_[i_b - 1]) && (sticks_[i_b] != sticks_[i_a])) { continue; }

                last_a = sticks_[i_a];
                last_b = sticks_[i_b];

                if (VERBOSE) { std::cout << "::=> sticks_[" << i_a << "] = " << sticks_[i_a] << ", sticks_[" << i_b << "] = " << sticks_[i_b] << std::endl;  }

                size a = sticks_[i_a];
                size b = sticks_[i_b];

                size i_c_left = binary_search_left(a, b, sticks_unique);
                size i_c_right = binary_search_right(a, b, sticks_unique);

                if (VERBOSE) { std::cout << "a = " << a << ", b = " << b << ", i_c_left = " << i_c_left << ", i_c_right = " << i_c_right; }

                if (i_c_left != -1) {
                    if (VERBOSE) { std::cout << ", sticks_unique[i_c_left] = " << sticks_unique[i_c_left] << ", sticks_unique[i_c_right] = " << sticks_unique[i_c_right]; }
                    k += i_c_right - i_c_left + 1;
                    if (VERBOSE) { std::cout << ", k --> " << k; }
                } else { if (VERBOSE) { std::cout << ", i_c_left is out of bounds!"; }}

                if (VERBOSE) { std::cout << std::endl; }
            }
        }

        return generate_answer(k);
    }

private:

    static size binary_search_left(size a, size b, std::vector<size>& sticks_unique) {
        if (VERBOSE) { std::cout << ".....BINARY SEARCH <<LEFT>>: a = " << a << ", b = " << b << ", sticks_unique = " << sticks_unique << std::endl; }

        size n = sticks_unique.size();
        if (n == 1) {
            bool is_triangle_obtuse = is_obtuse(a, b, sticks_unique[0]);
            if (is_triangle_obtuse) {
                if (VERBOSE) { std::cout << ".....triangle with a = " << a << ", b = " << b << ", c = " << sticks_unique[0] << " is obtuse!" << std::endl; }
                return 0;
            }
            else {
                if (VERBOSE) { std::cout << ".....triangle with a = " << a << ", b = " << b << ", c = " << sticks_unique[0] << " is not obtuse!" << std::endl; }
                return -1;
            }
        }

        size i_l = 0, i_r = sticks_unique.size() - 1;
        while (i_l <= i_r) {
            if (VERBOSE) { std::cout << ".....i_l = " << i_l << ", i_r = " << i_r << std::endl; }
            auto i_m = i_l + (i_r - i_l) / 2;
            auto m = sticks_unique[i_m];
            if (VERBOSE) { std::cout << ".....sticks_unique[i_m] --> " << m << std::endl; }
            if (is_obtuse(a, b, m)) {
                i_r = i_m - 1;
                if (VERBOSE) { std::cout << ".....obtuse! sticks_unique[i_l] = " << sticks_unique[i_l] << ", sticks_unique[i_r] --> " << sticks_unique[i_r] << std::endl; }
            }
            else {
                i_l = i_m + 1;
                if (VERBOSE) { std::cout << ".....not obtuse! sticks_unique[i_r] = " << sticks_unique[i_r] << ", sticks_unique[i_l] --> " << sticks_unique[i_l] << std::endl; }
            }
        }

        if (VERBOSE) { std::cout << ".....---> return sticks_unique[i_r + 1] = " << sticks_unique[i_r + 1] << std::endl; }

        auto res = i_r + 1;
        if (res > n - 1) { return -1; }
        else { return i_r + 1; }
    }

    static size binary_search_right(size a, size b, std::vector<size>& sticks_unique) {
        if (VERBOSE) { std::cout << ".....BINARY SEARCH <<RIGHT>>: a = " << a << ", b = " << b << ", sticks_unique = " << sticks_unique << std::endl; }

        size i_l = 0, i_r = sticks_unique.size() - 1;
        while (i_l <= i_r) {
            if (VERBOSE) { std::cout << ".....i_l = " << i_l << ", i_r = " << i_r << std::endl; }
            auto i_m = i_l + (i_r - i_l) / 2;
            auto m = sticks_unique[i_m];
            if (VERBOSE) { std::cout << ".....sticks_unique[i_m] --> " << m << std::endl; }
            if (exists(a, b, m)) {
                i_l = i_m + 1;
                if (VERBOSE) { std::cout << ".....exists! sticks_unique[i_r] = " << sticks_unique[i_r] << ", sticks_unique[i_l] --> " << sticks_unique[i_l] << std::endl; }
            }
            else {
                i_r = i_m - 1;
                if (VERBOSE) { std::cout << ".....not exists! sticks_unique[i_l] = " << sticks_unique[i_l] << ", sticks_unique[i_r] --> " << sticks_unique[i_r] << std::endl; }
            }
        }

        if (VERBOSE) { std::cout << ".....---> return sticks_unique[i_l - 1] = " << sticks_unique[i_l - 1] << std::endl; }

        return i_l - 1;
    }

    static bool exists(size a, size b, size c) {
        return a + b > c;
    }

    static bool is_obtuse(size a, size b, size c) {
        // c - longest side
        return c * c > a * a + b * b;
    }

    static std::string generate_answer(size k) {
        return "Sasha can compose " + std::to_string(k) + " triangles";
    }

    std::vector<size> sticks_;
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
                {{"sticks", "4 4 4 7"}, {"true", "1"}},
                {{"sticks", "5 6 6 9"}, {"true", "2"}},
                {{"sticks", "4 5 8 9"}, {"true", "2"}},
                {{"sticks", "3 4 5 5 6"}, {"true", "2"}},

                // medium
                {{"sticks", "5 9 11 12"}, {"true", "2"}},
                {{"sticks", "8 4 9 4"}, {"true", "1"}},

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

    const size MIN_N = 1, MAX_N = 100;
    std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

    const size MIN_VAL = 1, MAX_VAL = 100;
    std::uniform_int_distribution<> dist_VAL(MIN_VAL, MAX_VAL);

    while (true) {
        std::cout << "======================" << std::endl;

        const size n_sticks = dist_N(gen);

        std::vector<size> sticks;
        for (size i = 0; i < n_sticks; ++i) { sticks.push_back(dist_VAL(gen)); }

        std::cout << "n = " << sticks.size() << ", sticks = " << sticks << std::endl;

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

        if (duration_fast * 1e-9 > 3.0) { throw std::runtime_error("Fast solution is too slow: " + std::to_string(duration_fast * 1e-9) + " s"); }

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