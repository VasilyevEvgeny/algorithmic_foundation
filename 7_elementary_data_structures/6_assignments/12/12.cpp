//
// Created by evasilyev on 30.05.2020.
//

#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <random>
#include <chrono>

// 1 - work, 2 - manual tests, 3 - stress tests
#define MODE 1

#define VERBOSE false

using size = int;
using tt = std::chrono::steady_clock;


std::ostream& operator << (std::ostream& os, const std::vector<size>& v) {
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


class Greedy {
public:
    Greedy(size N, std::vector<size> measurements, size M, std::vector<size> variabilities)
    : N_(N), measurements_(std::move(measurements)), M_(M), variabilities_(std::move(variabilities)) {}

    std::string solve() {
        std::string answer;

        for (auto& k : variabilities_) {
            if (VERBOSE) { std::cout << "k = " << k << std::endl; }

            size idx_left_target = 1, idx_right_target = 1, len_max = 0;

            for (size i = 0; i < N_; ++i) {
                bool stop = false;
                for (size j = i; j < N_; ++j) {
                    auto left = measurements_.begin() + i, right = measurements_.begin() + j + 1;
                    size min = *std::min_element(left, right);
                    size max = *std::max_element(left, right);
                    auto delta = max - min;
                    auto len_cur = j - i + 1;

                    if (VERBOSE) {
                        std::cout << "*left = " << *left << ", *right = " << *std::prev(right) << std::endl;
                        std::cout << "min = " << min << ", max = " << max << ", delta = " << delta << ", len_cur = " << len_cur << std::endl;
                    }

                    if (delta <= k) {
                        if (len_cur > len_max) {
                            idx_left_target = i + 1;
                            idx_right_target = j + 1;
                            len_max = len_cur;

                            if (VERBOSE) {
                                std::cout << "record!" << std::endl;
                                std::cout << ".....idx_left_target --> " << idx_left_target << ", idx_right_target --> "
                                          << idx_right_target << ", len_max --> " << len_max << std::endl;
                            }
                        }
                    }
                    else { stop = true; }

                    if (stop) { break; }
                }
            }
            answer += std::to_string(idx_left_target) + " " + std::to_string(idx_right_target) + "\n";
        }

        return answer;
    }


private:
    const size N_;
    const std::vector<size> measurements_;
    const size M_;
    const std::vector<size> variabilities_;

};


class AdvancedStack {
public:
    AdvancedStack() : size_(0) {}

    void push(size val) {
        if (st_.empty()) { st_.push({val, val, val}); }
        else {
            auto min_element = std::min(val, st_.top()[1]);
            auto max_element = std::max(val, st_.top()[2]);
            st_.push({val, min_element, max_element});
        }
        size_++;
    }

    void pop() {
        st_.pop();
        size_--;
    }

    size min() {
        return st_.top()[1];
    }

    size max() {
        return st_.top()[2];
    }

    bool empty() {
        return st_.empty();
    }

    std::vector<size> top() {
        return st_.top();
    }

    size len() const {
        return size_;
    }

private:
    std::stack<std::vector<size>> st_;
    size size_;

};


std::ostream& operator<< (std::ostream& os, const AdvancedStack& st) {
    auto st_copy = st;
    os << "[ ";
    while (!st_copy.empty()) {
        auto top = st_copy.top();
        os << "{" << top[0] << " -> " << top[1] << ", " << top[2] << "} ";
        st_copy.pop();
    }
    os << "]";

    return os;
}


class AdvancedQueue {
public:
    AdvancedQueue() = default;

    void push(size val) {
        st1_.push(val);
    }

    void pop() {
        if (st2_.empty()) {
            while (!st1_.empty()) {
                st2_.push(st1_.top()[0]);
                st1_.pop();
            }
        }
        st2_.pop();
    }

    size min() {
        if (st1_.empty() and !st2_.empty()) {
            return st2_.min();
        }
        else if (!st1_.empty() and st2_.empty()) {
            return st1_.min();
        }
        else {
            return std::min(st1_.min(), st2_.min());
        }
    }

    size max() {
        if (st1_.empty() and !st2_.empty()) {
            return st2_.max();
        }
        else if (!st1_.empty() and st2_.empty()) {
            return st1_.max();
        }
        else {
            return std::max(st1_.max(), st2_.max());
        }
    }

    size len() { return st1_.len() + st2_.len(); }

    AdvancedStack st1() const { return st1_; }
    AdvancedStack st2() const { return st2_; }

private:
    AdvancedStack st1_;
    AdvancedStack st2_;
};


std::ostream& operator<< (std::ostream& os, const AdvancedQueue& q) {
    os << "( st1 = " << q.st1() << " :: st2 = " << q.st2() << " )";

    return os;
}


class Fast {
public:
    Fast(size N, std::vector<size> measurements, size M, std::vector<size> variabilities)
    : N_(N), measurements_(std::move(measurements)), M_(M), variabilities_(std::move(variabilities)) {}

    std::string solve () {
        std::string answer;

//        double duration_push = 0.0, duration_pop = 0.0;

        for (auto& k : variabilities_) {
            AdvancedQueue q;

            q.push(measurements_[0]);
            size idx_left = 1, idx_right = 1;
            size idx_left_target = 1, idx_right_target = 1, len_max = 0;

            if (VERBOSE) { std::cout << "q = " << q << ", idx_left = " << idx_left << ", idx_right = "
            << idx_right << ", len_max = " << len_max << std::endl; }

            for (size i = 1; i < N_; ++i) {
//                tt::steady_clock::time_point begin_push = tt::now();
                q.push(measurements_[i]);
//                tt::steady_clock::time_point end_push = tt::now();
//                duration_push += 1e-9 * std::chrono::duration_cast<std::chrono::nanoseconds> (end_push - begin_push).count();

                idx_right++;

                if (VERBOSE) { std::cout << "pushed " << measurements_[i] << "! idx_right --> " << idx_right << std::endl; }

                auto delta = q.max() - q.min();
                auto len_cur = q.len();
                if (VERBOSE) { std::cout << "len_cur = " << len_cur << std::endl; }

                if (delta <= k) {
                    if (len_cur > len_max) {
                        len_max = idx_right - idx_left + 1;
                        idx_left_target = idx_left;
                        idx_right_target = idx_right;
                        if (VERBOSE) {
                            std::cout << "idx_left_target = " << idx_left_target << ", idx_right_target --> " << idx_right_target
                                      << ", len_max --> " << len_max << std::endl;
                        }
                    }
                }
                else {
//                    tt::steady_clock::time_point begin_pop = tt::now();
                    while (q.max() - q.min() > k && q.len() > 1) {
                        q.pop();
                        idx_left++;
                        if (VERBOSE) { std::cout << ".....q --> " << q << ", idx_left --> " << idx_left << std::endl; }
                    }
//                    tt::steady_clock::time_point end_pop = tt::now();
//                    duration_pop += 1e-9 * std::chrono::duration_cast<std::chrono::nanoseconds> (end_pop - begin_pop).count();

                }

                if (VERBOSE) { std::cout << "q = " << q << ", idx_left = " << idx_left << ", idx_right = "
                                         << idx_right << ", len_max = " << len_max << std::endl; }
            }
            answer += std::to_string(idx_left_target) + " " + std::to_string(idx_right_target) + "\n";
        }

//        std::cout << "duration_push = " << duration_push << " s" << std::endl;
//        std::cout << "duration_pop = " << duration_pop << " s" << std::endl;


        return answer;
    }

private:
    const size N_;
    const std::vector<size> measurements_;
    const size M_;
    const std::vector<size> variabilities_;
};


class ManualTester {
public:
    ManualTester() = default;

    static void test() {
        std::vector<std::map<std::string, std::string>> test_data = {

            // boundary
            {{"N", "1"}, {"measurements", "1"}, {"M", "1"}, {"variabilities", "0"}, {"true", "1 1\n"}},
            {{"N", "1"}, {"measurements", "1"}, {"M", "1"}, {"variabilities", "1"}, {"true", "1 1\n"}},
            {{"N", "2"}, {"measurements", "1 1"}, {"M", "1"}, {"variabilities", "0"}, {"true", "1 2\n"}},
            {{"N", "2"}, {"measurements", "1 1"}, {"M", "1"}, {"variabilities", "1"}, {"true", "1 2\n"}},

            // simple
            {{"N", "4"}, {"measurements", "1 5 -5 -3"}, {"M", "3"}, {"variabilities", "3 0 5"}, {"true", "3 4\n1 1\n1 2\n"}},
            {{"N", "7"}, {"measurements", "10 1 10 12 11 1 11"}, {"M", "2"}, {"variabilities", "2 1"}, {"true", "3 5\n4 5\n"}},

        };

        for (auto& line : test_data) {
            std::cerr << "===============================" << std::endl;

            size N = static_cast<size>(std::stoi(line.at("N")));
            std::vector<size> measurements = parse_str(line.at("measurements"));
            size M = static_cast<size>(std::stoi(line.at("M")));
            std::vector<size> variabilities = parse_str(line.at("variabilities"));

            std::string res_true = line.at("true");
            std::string pred = Greedy(N, measurements, M, variabilities).solve();

            std::cerr << "N = " << N << ", measurements = " << measurements << "\nM = " << M << ", variablities = " <<
            variabilities << std::endl;

            std::cerr << "res_true:\n" << res_true << "pred:\n" << pred << std::endl;

            assert(res_true == pred);
        }
    }


private:

    static std::vector<size> parse_str(const std::string& str) {
        std::stringstream iss(str);
        std::vector<size> res;
        size k;
        while (iss >> k) { res.push_back(k); }

        return res;
    }

};


class StressTester {
public:
    StressTester() = default;

    static void test() {
        std::random_device rd;
        std::mt19937 gen(rd());

        const size MIN_N = 600000, MAX_N = 600000;
        std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

        const size MIN_MEASUREMENTS = -1e9, MAX_MEASUREMENTS = +1e9;
        std::uniform_int_distribution<> dist_MEASUREMENTS(MIN_MEASUREMENTS, MAX_MEASUREMENTS);

        const size MIN_M = 100, MAX_M = 100;
        std::uniform_int_distribution<> dist_M(MIN_M, MAX_M);

        const size MIN_VARIABILITY = 0, MAX_VARIABILITY = 1e9;
        std::uniform_int_distribution<> dist_VARIABILITY(MIN_VARIABILITY, MAX_VARIABILITY);

        while (true) {
            std::cout << "======================" << std::endl;

            size N = dist_N(gen);
            std::vector<size> measurements(N);
            for (size i = 0; i < N; ++i) { measurements[i] = dist_MEASUREMENTS(gen); }

            size M = dist_M(gen);
            std::vector<size> variabilities(M);
            for (size i = 0; i < M; ++i) { variabilities[i] = dist_VARIABILITY(gen); }

            std::cout << "N = " << N << std::endl; //", measurements = " << measurements << std::endl;
            std::cout << "M = " << M << std::endl; //", variabilities = " << variabilities << std::endl;

            tt::steady_clock::time_point begin_greedy = tt::now();
            auto res_greedy = 0; //Greedy(N, measurements, M, variabilities).solve();
            tt::steady_clock::time_point end_greedy = tt::now();
            auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

            std::cout << std::scientific << "T_greedy = " << duration_greedy * 1e-9 << " s" << std::endl;

            tt::steady_clock::time_point begin_fast = tt::now();
            auto res_fast = Fast(N, measurements, M, variabilities).solve();
            tt::steady_clock::time_point end_fast = tt::now();
            auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();

            std::cout << std::scientific << "T_fast = " << duration_fast * 1e-9 << " s" << std::endl;

//            assert(res_greedy == res_fast);

            if (duration_fast * 1e-9 > 5.0) { throw std::runtime_error("Solution is too slow: " + std::to_string(duration_fast * 1e-9) + " s"); }

            measurements.clear();
            variabilities.clear();
        }
    }

private:

};


int main() {

#if MODE == 1
    size N = 0;
    std::cin >> N;

    std::vector<size> measurements(N);
    for (size i = 0; i < N; ++i) { std::cin >> measurements[i]; }

    size M = 0;
    std::cin >> M;

    std::vector<size> variability(M);
    for (size i = 0; i < M; ++i) { std::cin >> variability[i]; }

//    std::cout << Greedy(N, measurements, M, variability).solve();
    std::cout << Fast(N, measurements, M, variability).solve();

#elif MODE == 2
    ManualTester::test();

#elif MODE == 3
    StressTester::test();

#endif

    return 0;
}