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
#include <algorithm>

// 1 - work, 2 - manual testing, 3 - stress testing
#define MODE 3
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
        std::set<size> tmp(sticks_.begin(), sticks_.end());
        std::vector<size> sticks_unique(tmp.begin(), tmp.end());
        size n_unique = sticks_unique.size();
        std::map<size, bool> sticks_map;

        for (size i_a = 0; i_a < n - 2; ++i_a) {
            for (size i = 0; i < n_unique; ++i) { sticks_map[sticks_unique[i]] = false; }

            for (size i_b = i_a + 1; i_b < n - 1; ++i_b) {
                if (!sticks_map.at(sticks_[i_b])) { sticks_map.at(sticks_[i_b]) = true; }
                else { continue; }

                std::set<size> s{sticks_.begin() + i_b + 1, sticks_.end()};
                std::vector<size> cs_unique(s.begin(), s.end());

                auto a = sticks_[i_a];
                auto b = sticks_[i_b];
                size i_c_left = binary_search_left(a, b, cs_unique);
                size i_c_right = binary_search_right(a, b, cs_unique);

                if (VERBOSE) { std::cout << "a = " << a << ", b = " << b; }

                if (i_c_left != -1) {
                    if (VERBOSE) { std::cout << ", cs_unique[i_c_left] = " << cs_unique[i_c_left] << ", cs_unique[i_c_right] = "
                                             << cs_unique[i_c_right]; }
                    k += i_c_right - i_c_left + 1;
                    if (VERBOSE) { std::cout << ", k --> " << k; }
                }
                else { if (VERBOSE) { std::cout << ", i_c_left is out of bounds!"; }}

                if (VERBOSE) { std::cout << std::endl; }

            }
        }

        return generate_answer(k);
    }

    std::string solve_slow() {
        std::sort(sticks_.begin(), sticks_.end());
        size n = sticks_.size();

        if (VERBOSE) { std::cout << "sticks = " << sticks_ << std::endl; }

        if (n < 3) { return generate_answer(0); }

        size k = 0;
        std::set<std::vector<size>> ab;
        for (size i_a = 0; i_a < n - 2; ++i_a) {
            for (size i_b = i_a + 1; i_b < n - 1; ++i_b) {
                if (ab.find({sticks_[i_a], sticks_[i_b]}) != ab.end()) { continue; }
                else {
                    ab.insert({sticks_[i_a], sticks_[i_b]});

                    std::set<size> s{sticks_.begin() + i_b + 1, sticks_.end()};
                    std::vector<size> cs_unique(s.begin(), s.end());

                    auto a = sticks_[i_a];
                    auto b = sticks_[i_b];
                    size i_c_left = binary_search_left(a, b, cs_unique);
                    size i_c_right = binary_search_right(a, b, cs_unique);

                    if (VERBOSE) { std::cout << "a = " << a << ", b = " << b; }

                    if (i_c_left != -1) {
                        if (VERBOSE) { std::cout << ", cs_unique[i_c_left] = " << cs_unique[i_c_left] << ", cs_unique[i_c_right] = "
                                  << cs_unique[i_c_right]; }
                        k += i_c_right - i_c_left + 1;
                        if (VERBOSE) { std::cout << ", k --> " << k; }
                    }
                    else { if (VERBOSE) { std::cout << ", i_c_left is out of bounds!"; }}

                    if (VERBOSE) { std::cout << std::endl; }
                }
            }
        }

        return generate_answer(k);
    }

private:

    static size binary_search_left(size a, size b, std::vector<size>& cs_unique) {
        if (VERBOSE) { std::cout << ".....BINARY SEARCH <<LEFT>>: a = " << a << ", b = " << b << ", cs_unique = " << cs_unique << std::endl; }

        size n = cs_unique.size();
        if (n == 1) {
            bool is_triangle_obtuse = is_obtuse(a, b, cs_unique[0]);
            if (is_triangle_obtuse) {
                if (VERBOSE) { std::cout << ".....triangle with a = " << a << ", b = " << b << ", c = " << cs_unique[0] << " is obtuse!" << std::endl; }
                return 0; }
            else {
                if (VERBOSE) { std::cout << ".....triangle with a = " << a << ", b = " << b << ", c = " << cs_unique[0] << " is not obtuse!" << std::endl; }
                return -1;
            }
        }

        size it = 0;
        auto max_it = 10;
        size i_l = 0, i_r = cs_unique.size() - 1;
        while (i_l <= i_r) {
            if (VERBOSE) { std::cout << ".....i_l = " << i_l << ", i_r = " << i_r << std::endl; }
            auto i_m = i_l + (i_r - i_l) / 2;
            auto m = cs_unique[i_m];
            if (VERBOSE) { std::cout << ".....cs_unique[i_m] --> " << m << std::endl; }
            if (is_obtuse(a, b, m)) {
                i_r = i_m - 1;
                if (VERBOSE) { std::cout << ".....obtuse! cs_unique[i_l] = " << cs_unique[i_l] << ", cs_unique[i_r] --> " << cs_unique[i_r] << std::endl; }
            }
            else {
                i_l = i_m + 1;
                if (VERBOSE) { std::cout << ".....not obtuse! cs_unique[i_r] = " << cs_unique[i_r] << ", cs_unique[i_l] --> " << cs_unique[i_l] << std::endl; }
            }

            ++it;
//            if (it > max_it) { exit(-1); }
        }

        if (VERBOSE) { std::cout << ".....---> return cs_unique[i_r + 1] = " << cs_unique[i_r + 1] << std::endl; }

        auto res = i_r + 1;
        if (res > n - 1) { return -1; }
        else { return i_r + 1; }
    }

    static size binary_search_right(size a, size b, std::vector<size>& cs_unique) {
        if (VERBOSE) { std::cout << ".....BINARY SEARCH <<RIGHT>>: a = " << a << ", b = " << b << ", cs_unique = " << cs_unique << std::endl; }

        size it = 0;
        auto max_it = 10;
        size i_l = 0, i_r = cs_unique.size() - 1;
        while (i_l <= i_r) {
            if (VERBOSE) { std::cout << ".....i_l = " << i_l << ", i_r = " << i_r << std::endl; }
            auto i_m = i_l + (i_r - i_l) / 2;
            auto m = cs_unique[i_m];
            if (VERBOSE) { std::cout << ".....cs_unique[i_m] --> " << m << std::endl; }
            if (exists(a, b, m)) {
                i_l = i_m + 1;
                if (VERBOSE) { std::cout << ".....exists! cs_unique[i_r] = " << cs_unique[i_r] << ", cs_unique[i_l] --> " << cs_unique[i_l] << std::endl; }
            }
            else {
                i_r = i_m - 1;
                if (VERBOSE) { std::cout << ".....not exists! cs_unique[i_l] = " << cs_unique[i_l] << ", cs_unique[i_r] --> " << cs_unique[i_r] << std::endl; }
            }

            ++it;
//            if (it > max_it) { exit(-1); }
        }

        if (VERBOSE) { std::cout << ".....---> return cs_unique[i_l - 1] = " << cs_unique[i_l - 1] << std::endl; }

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
                {{"sticks", "4 5 8 9"}, {"true", "2"}},
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

    const size MIN_N = 1, MAX_N = 7000;
    std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

    const size MIN_VAL = 1, MAX_VAL = 7000;
    std::uniform_int_distribution<> dist_VAL(MIN_VAL, MAX_VAL);

    while (true) {
        std::cout << "======================" << std::endl;

        const size n_sticks = dist_N(gen);

        std::vector<size> sticks;
        for (size i = 0; i < n_sticks; ++i) { sticks.push_back(dist_VAL(gen)); }

        std::cout << "sticks = " << sticks << std::endl;

        tt::steady_clock::time_point begin_greedy = tt::now();
        auto res_greedy = 0; //GreedySolver(sticks).solve();
        tt::steady_clock::time_point end_greedy = tt::now();
        auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

        tt::steady_clock::time_point begin_fast = tt::now();
        auto res_fast = FastSolver(sticks).solve();
        tt::steady_clock::time_point end_fast = tt::now();
        auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();

        std::cout << std::scientific << "T = " << duration_greedy * 1e-9 << " s :: res_greedy = " << res_greedy << std::endl;
        std::cout << std::scientific << "T = " << duration_fast * 1e-9 << " s :: res_fast = " << res_fast << std::endl;

//        assert(res_greedy == res_fast);

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