#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <sstream>
#include <random>
#include <chrono>
#include <deque>
#include <algorithm>

// 1 - work, 2 - manual testing, 3 - stress testing
#define MODE 1

#define VERBOSE false

using size = int64_t;
using tt = std::chrono::steady_clock;


template <typename T, typename U>
std::ostream& operator << (std::ostream& os, const std::pair<T, U>& p) {
    os << p.first << " " << p.second;
    return os;
}

template <typename T, typename U>
bool operator == (std::pair<T, U>& lhs, std::pair<T, U>& rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <typename T>
std::ostream& operator << (std::ostream& os, const std::deque<T>& v) {
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
    explicit GreedySolver(std::string s) : s_(std::move(s)), n_(s_.length()) {}

    std::pair<size, size> solve() {
        size P_max = 0, L = 0, L_max = 0;
        for (size i = 0; i < n_; ++i) {
            for (size j = i; j < n_; ++j) {
                std::string substr(s_.begin() + i, s_.begin() + j + 1);
                if (VERBOSE) { std::cout << "substr = " << substr << std::endl; }

                if (is_alternating(substr, i)) {
                    L++;
                    if (VERBOSE) { std::cout << "alternating! L --> " << L << std::endl; }
                }
                else {
                    if (VERBOSE) { std::cout << "not alternating!" << std::endl; }
                    if (L > L_max) {
                        L_max = L;
                        P_max = i;
                        if (VERBOSE) { std::cout << "L > L_max! L_max --> " << L_max << ", P_max --> " << P_max << std::endl; }
                    }
                    L = 0;
                    if (VERBOSE) { std::cout << "L --> 0, next start position..." << std::endl; }
                    break;
                }
            }

            if (VERBOSE) { std::cout << "whole string has been processed and this substring is alternating!" << std::endl; }
            if (L > L_max) {
                L_max = L;
                P_max = i;
                if (VERBOSE) { std::cout << "L > L_max! L_max --> " << L_max << ", P_max --> " << P_max << std::endl; }
            }
            L = 0;
            if (VERBOSE) { std::cout << "L --> 0" << std::endl; }

        }

        return {P_max + 1, L_max};
    }

private:

    static bool is_alternating(std::string& substr, size P) {
        std::string odd, even;
        for (size i = 0; i < substr.length(); ++i) {
            if (VERBOSE) {
                std::cout << ".....substr[" << i << "] = " << substr[i] << std::endl;
                std::cout << ".....odd = " << odd << ", even = " << even << std::endl;
            }

            if ((P + 1 + i) % 2) {
                if (VERBOSE) { std::cout << ".....i = " << i << " is odd!" << std::endl; }
                if (even.find(substr[i]) != std::string::npos) {
                    if (VERBOSE) { std::cout << ".....intersection found! return false" << std::endl;}
                    return false;
                }
                odd += substr[i];
                if (VERBOSE) { std::cout << ".....odd --> " << odd << std::endl; }
            }
            else {
                if (VERBOSE) { std::cout << ".....i = " << i << " is even!" << std::endl; }
                if (odd.find(substr[i]) != std::string::npos) {
                    if (VERBOSE) { std::cout << ".....intersection found! return false" << std::endl;}
                    return false;
                }
                even += substr[i];
                if (VERBOSE) { std::cout << ".....even --> " << even << std::endl; }
            }

            if (VERBOSE) { std::cout << ".....==> :: after processing: odd = " << odd << ", even = " << even << std::endl; }

        }

        if (VERBOSE) { std::cout << "no intersection! return true" << std::endl; }

        return true;
    }

    const std::string s_;
    const size n_;
};



class FastSolver {
public:
    explicit FastSolver(std::string s) : s_(std::move(s)), n_(s_.length()) {}

    std::pair<size, size> solve() {
        size P_max = 0, L, L_max = 0;
        std::deque<char> even, odd;
        for (size l = 0, r = 0; r < n_; ++l) {
            do {
                char symbol = s_[r];
                if (VERBOSE) {
                    std::cout << "r = " << r << ", symbol = " << symbol << std::endl;
                    std::cout << "even = " << even << ", odd = " << odd << std::endl;
                }

                if ((r + 1) % 2) {
                    if (VERBOSE) { std::cout << ".....odd!" << std::endl; }

                    if (std::find(even.begin(), even.end(), symbol) != even.end()) {
                        if (VERBOSE) { std::cout << ".....not alternating! break searching right bound" << std::endl; }
                        break;
                    }
                    else {
                        odd.push_back(symbol);
                        if (VERBOSE) { std::cout << ".....alternating! odd --> " << odd << std::endl; }
                    }
                }
                else {
                    if (VERBOSE) { std::cout << ".....even!" << std::endl; }

                    if (std::find(odd.begin(), odd.end(), symbol) != odd.end()) {
                        if (VERBOSE) { std::cout << ".....not alternating! break searching right bound" << std::endl; }
                        break;
                    }
                    else {
                        even.push_back(symbol);
                        if (VERBOSE) { std::cout << ".....alternating! even --> " << even << std::endl; }
                    }
                }

                r++;
                if (VERBOSE) { std::cout << "r --> " << r << std::endl; }

            } while (r < n_);

            L = r - l;
            if (VERBOSE) { std::cout << "L = r - l = " << L << std::endl; }
            if (L > L_max) {
                L_max = L;
                P_max = l;
                if (VERBOSE) {
                    std::cout << "L > L_max! L_max --> " << L_max << ", P_max --> " << P_max << std::endl;
                }
            }

            if ((l + 1) % 2) {
                if (!odd.empty()) {
                    odd.pop_front();
                    if (VERBOSE) { std::cout << "pop first element from odd: odd --> " << odd << std::endl; }
                }
            }
            else {
                if (!even.empty()) {
                    even.pop_front();
                    if (VERBOSE) { std::cout << "pop first element from even: even --> " << even << std::endl; }
                }
            }
        }

        return {P_max + 1, L_max};
    }

private:

    const std::string s_;
    const size n_;
};



class ManualTester {
public:
    ManualTester() = default;

    static void test() {
        std::vector<std::map<std::string, std::string>> test_data = {

                // basic
                {{"s", "7777"}, {"true", "1 1"}},
                {{"s", "01234567802345"}, {"true", "2 13"}},

        };

        test_data.push_back(generate_complicated_case());

        for (auto &line : test_data) {
            std::string s = line.at("s");
            auto res_true = str_to_pair(line.at("true"));

            tt::steady_clock::time_point begin_greedy = tt::now();
            auto pred_greedy = 0; //GreedySolver(s).solve();
            tt::steady_clock::time_point end_greedy = tt::now();
            auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

            tt::steady_clock::time_point begin_fast = tt::now();
            auto pred_fast = FastSolver(s).solve();
            tt::steady_clock::time_point end_fast = tt::now();
            auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();


            std::cerr << "s = " << s << std::endl;
            std::cerr << std::scientific << "T = " << duration_greedy * 1e-9 << " s :: pred_greedy = " << pred_greedy << std::endl;
            std::cerr << std::scientific << "T = " << duration_fast * 1e-9 << " s :: pred_fast = " << pred_fast << std::endl;

//            assert(res_true == pred_greedy);
            assert(res_true == pred_fast);
        }
    }

private:

    static std::pair<size, size> str_to_pair(std::string& str) {
        std::istringstream ss(str);
        size P = 0, L = 0;
        ss >> P >> L;
        return {P, L};
    }

    static std::map<std::string, std::string> generate_complicated_case() {
        std::string s;
        size n = 30000;
        for (size i = 0; i < n - 1; ++i) {
            if (i % 2) { s += "0"; }
            else { s += "1"; }
        }
        s += "1";
        size true_answer = n - 1;

        return {{"s", s}, {"true", "1 " + std::to_string(true_answer)}};
    }
};


void stress_testing() {
    std::random_device rd;
    std::mt19937 gen(rd());

    const size MIN_N = 1, MAX_N = 30000;
    std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

    const size MIN_VAL = 0, MAX_VAL = 9;
    std::uniform_int_distribution<> dist_VAL(MIN_VAL, MAX_VAL);

    while (true) {
        std::cout << "======================" << std::endl;

        const size N = dist_N(gen);

        std::string s;
        for (size i = 0; i < N; ++i) { s += std::to_string(dist_VAL(gen)); }

        std::cout << "N = " << N << ", s = " << s << std::endl;

        tt::steady_clock::time_point begin_greedy = tt::now();
        auto res_greedy = GreedySolver(s).solve();
        tt::steady_clock::time_point end_greedy = tt::now();
        auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

        tt::steady_clock::time_point begin_fast = tt::now();
        auto res_fast = FastSolver(s).solve();
        tt::steady_clock::time_point end_fast = tt::now();
        auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();

        std::cout << std::scientific << "T = " << duration_greedy * 1e-9 << " s :: res_greedy = " << res_greedy << std::endl;
        std::cout << std::scientific << "T = " << duration_fast * 1e-9 << " s :: res_fast = " << res_fast << std::endl;

        if (duration_fast * 1e-9 > 2.0) { throw std::runtime_error("Fast solution is too slow: " + std::to_string(duration_fast * 1e-9) + " s"); }

        assert(res_greedy == res_fast);

        s.clear();
    }
}


int main() {

#if MODE == 1
    std::string s;
    std::getline(std::cin, s);

//    std::cout << GreedySolver(s).solve() << std::endl;
    std::cout << FastSolver(s).solve() << std::endl;

#elif MODE == 2
    ManualTester::test();

#elif MODE == 3
    stress_testing();

#endif

    return 0;
}