//
// Created by evasilyev on 01.04.2020.
//

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <random>
#include <exception>
#include <chrono>

#define VERBOSE false

// 1 - work, 2 - manual testing, 3 - stress testing
#define MODE 1

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
    GreedySolver(const size N, std::vector<size> a, const size M)
    : N_(N), a_(std::move(a)), M_(M) {}

    size solve() {

        // max and min
        size a_max = *std::max_element(a_.begin(), a_.end());
        size a_min = *std::min_element(a_.begin(), a_.end());
        size volume_min = 0, volume_max = 0;
        if (N_ <= M_) { return a_max; }
        else if (N_ > 2 *  M_) { return -1; }
        else { volume_min = a_max; volume_max = 2 * a_max; }
        if (VERBOSE) { std::cout << "volume_max = " << volume_max << std::endl; }

        // sort
        std::sort(a_.begin(), a_.end(), std::greater<>());

        // main cycle
        for (size volume = volume_min; volume <= volume_max; ++volume) {
            bool placed = false;
            std::vector<size> boxes(M_, 0), counts(M_, 0);
            for (auto n = 0; n < N_; ++n) {
                placed = false;
                for (auto m = 0; m < M_; ++m) {
                    if (boxes[m] + a_[n] <= volume && counts[m] < 2) {
                        boxes[m] += a_[n];
                        counts[m]++;
                        placed = true;
                        break;
                    }
                }
                if (!placed) { break; }
            }
            if (placed) { return volume; }
        }

        return -1;
    }


private:
    const size N_;
    std::vector<size> a_;
    const size M_;

    
};


class FastSolver {
public:
    FastSolver(const size N, std::vector<size> a, const size M)
    : N_(N), a_(std::move(a)), M_(M) {}

    bool is_volume_ok(size m) {
        if (VERBOSE) { std::cout << ".....checking volume = " << m << std::endl; }
        size l, r;
        for (l = 0, r = N_ - 1; l < M_; ++l) {
            if (VERBOSE) { std::cout << ".....l = " << l << ", r = " << r << std::endl; }
            if (l != r && a_[l] + a_[r] <= m) {
                r--;
            }
        }
        l--;

        if (VERBOSE) { std::cout << ".....in the end l = " << l << ", r = " << r << " --> return " << (std::abs(l - r) <= 1) << std::endl; }

        return r - l < 1;
    }

    size solve() {

        if (VERBOSE) { std::cout << "==========================================" << std::endl; }

        // max and min
        size a_max = *std::max_element(a_.begin(), a_.end());
        size a_min = *std::min_element(a_.begin(), a_.end());
        size volume_min = 0, volume_max = 0;
        if (N_ <= M_) { return a_max; }
        else if (N_ > 2 *  M_) { return -1; }
        else { volume_min = a_max; volume_max = 2 * a_max; }
        if (VERBOSE) { std::cout << "volume_min = " << volume_min << ", volume_max = " << volume_max << std::endl; }

        // sort
        std::sort(a_.begin(), a_.end(), std::greater<>());

        // main cycle
        size l = volume_min, r = volume_max;
        while (l <= r) {
            auto m = l + (r - l) / 2;
            if (VERBOSE) { std::cout << "mean volume = " << m << std::endl; }

            if (is_volume_ok(m)) {
                r = m - 1;
                if (VERBOSE) { std::cout << "volume = " << m << " is ok, l = " << l << ", r --> " << r << std::endl; }
            }
            else {
                l = m + 1;
                if (VERBOSE) { std::cout << "volume = " << m << " is not ok, l --> " << l << ", r = " << r << std::endl; }
            }
        }

        return r + 1;
    }


private:
    const size N_;
    std::vector<size> a_;
    const size M_;


};


class Tester {
public:
    Tester() = default;

protected:

    static std::vector<size> str_to_vector(const std::string& str) {
        std::stringstream iss(str);
        std::vector<size> res;
        size a_i = 0;
        while (iss >> a_i)
            res.push_back(a_i);

        return res;
    }
};


class ManualTester : public Tester {
public:
    ManualTester() = default;

    static void test() {
        std::vector<std::map<std::string, std::string>> test_data = {

            // basic
            {{"N", "1"}, {"a", "1"}, {"M", "1"}, {"true", "1"}},
            {{"N", "1"}, {"a", "2"}, {"M", "1"}, {"true", "2"}},
            {{"N", "1"}, {"a", "3"}, {"M", "1"}, {"true", "3"}},
            {{"N", "2"}, {"a", "1 2"}, {"M", "1"}, {"true", "3"}},
            {{"N", "3"}, {"a", "1 2 3"}, {"M", "1"}, {"true", "-1"}},

            // medium
            {{"N", "4"}, {"a", "3 1 2 5"}, {"M", "1"}, {"true", "-1"}},
            {{"N", "4"}, {"a", "3 1 2 5"}, {"M", "2"}, {"true", "6"}},
            {{"N", "4"}, {"a", "3 1 2 5"}, {"M", "3"}, {"true", "5"}},
            {{"N", "4"}, {"a", "3 1 2 5"}, {"M", "4"}, {"true", "5"}},
            {{"N", "4"}, {"a", "3 1 2 5"}, {"M", "5"}, {"true", "5"}},
            {{"N", "4"}, {"a", "3 1 2 5"}, {"M", "5"}, {"true", "5"}},
            {{"N", "6"}, {"a", "7_elementary_data_structures 1 4 3 10 5"}, {"M", "3"}, {"true", "11"}},
            {{"N", "6"}, {"a", "7_elementary_data_structures 1 4 3 10 5"}, {"M", "4"}, {"true", "10"}},


        };

        for (auto& line : test_data) {
            size N = static_cast<size>(std::stoi(line.at("N")));
            std::vector<size> a = str_to_vector(line.at("a"));
            size M = static_cast<size>(std::stoi(line.at("M")));

            size res_true = static_cast<size>(std::stoi(line.at("true")));
            auto pred_greedy = GreedySolver(N, a, M).solve();
            auto pred_fast = FastSolver(N, a, M).solve();

            std::cerr << "N = " << N << ", a = " << a << ", M = " << M << ", pred_greedy = " <<
                      pred_greedy <<  ", pred_fast = " << pred_fast << std::endl;

            assert(res_true == pred_greedy);
            assert(res_true == pred_fast);
        }
    }

};


class StressTester : public Tester {
public:
    StressTester() = default;

    static void test() {
        std::random_device rd;
        std::mt19937 gen(rd());

        const size MIN_N = 1, MAX_N = 100000;
        std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

        const size MIN_M = 1, MAX_M = 100000;
        std::uniform_int_distribution<> dist_M(MIN_M, MAX_M);

        const size MIN_VAL = 1, MAX_VAL = 1000000000;
        std::uniform_int_distribution<> dist_VAL(MIN_VAL, MAX_VAL);

        while (true) {
            std::cout << "======================" << std::endl;

            const size N = dist_N(gen);
            const size M = dist_M(gen);

            std::vector<size> a;
            for (size i = 0; i < N; ++i) { a.push_back(dist_VAL(gen)); }

            std::cout << "N = " << N << ", M = " << M << ", a = " << a << std::endl;

            tt::steady_clock::time_point begin_greedy = tt::now();
            auto res_greedy = 0; //GreedySolver(N, a, M).solve();
            tt::steady_clock::time_point end_greedy = tt::now();
            auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

            tt::steady_clock::time_point begin_fast = tt::now();
            auto res_fast = FastSolver(N, a, M).solve();
            tt::steady_clock::time_point end_fast = tt::now();
            auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();

            std::cout << std::scientific << "T = " << duration_greedy * 1e-9 << " s :: res_greedy = " << res_greedy << std::endl;
            std::cout << std::scientific << "T = " << duration_fast * 1e-9 << " s :: res_fast = " << res_fast << std::endl;

//            assert(res_greedy == res_fast);

            a.clear();
        }
    }
};




int main() {

#if MODE == 1
    size N = 0, M = 0;
    std::cin >> N >> M;

    std::vector<size> a(N);
    for (auto i = 0; i < N; ++i) {
        std::cin >> a[i];
    }

    if (VERBOSE) {
        std::cout << "N = " << N << std::endl;
        std::cout << "a = " << a << std::endl;
        std::cout << "M = " << M << std::endl;
    }

//    std::cout << GreedySolver(N, a, M).solve() << std::endl;
    std::cout << FastSolver(N, a, M).solve() << std::endl;

#elif MODE == 2
    ManualTester::test();

#elif MODE == 3
    StressTester::test();

#endif

    return 0;
}