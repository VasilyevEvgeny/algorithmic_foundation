#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <chrono>
#include <bits/stdc++.h>

// 1 - work, 2 - manual testing, 3 - stress testing
#define MODE 1

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

std::ostream& operator<< (std::ostream& os, const std::map<char, size>& target) {
    for (auto item : target) { os << "{" << item.first << " : " << item.second << "}, "; }

    return os;
}

bool operator== (std::map<char, size>& lhs, std::map<char, size>& rhs) {
    for (auto& item : lhs) {
        if (item.second != rhs.at(item.first)) { return false; }
    }

    return true;
}


class GreedySolver {
public:
    GreedySolver(size m, std::string w, size n, std::string s)
    : m_(m), w_(std::move(w)), n_(n), s_(std::move(s)) {
        initialize_permutations(w_, "");
        if (VERBOSE) {
            std::cout << "permutations_ = " << permutations_ <<  std::endl;
        }
    }

    size solve() {
        size sum = 0;

        for (size i = 0; i < n_ - m_ + 1; ++i) {
            auto substr = std::string(s_.begin() + i, s_.begin() + i + m_);
            if (VERBOSE) { std::cout << "=> " << substr << std::endl; }
            for (auto& perm : permutations_) {
                if (substr == perm) {
                    sum++;
                    if (VERBOSE) { std::cout << "substring '" << substr << "' is valid! sum --> " << sum << std::endl; }
                    break;
                }
            }
        }

        return sum;
    }


private:

    void initialize_permutations(std::string in, const std::string& out) {
        if (in.empty()) { permutations_.push_back(out); }

        for (size i = 0; i < in.size(); ++i) {
            initialize_permutations(in.substr(1), out + in[0]);
            rotate(in.begin(), in.begin() + 1, in.end());
        }

    }

    const size m_;
    const std::string w_;
    const size n_;
    const std::string s_;
    std::vector<std::string> permutations_;
};


class FastSolver {
public:
    FastSolver(size m, std::string w, size n, std::string s)
            : m_(m), w_(std::move(w)), n_(n), s_(std::move(s)), base_dict_(initialize_base_dict()), w_dict_(make_dict(w_)) {}

    size solve() {
        size sum = 0;
        std::map<char, size> dict;
        dict.insert(base_dict_.begin(), base_dict_.end());

        for (size l = 0, r = m_ - 1; l < n_ - m_ + 1; ++l) {
            if (!l) {
                for (size i = 0; i < m_; ++i) { dict.at(s_[i])++; }
                if (VERBOSE) { std::cout << "first iteration! dict --> " << dict << std::endl; }
            }
            else {
                dict.at(s_[r])++;
                if (VERBOSE) { std::cout << "r --> " << r << ", dict --> " << dict << std::endl; }
            }

            if (dict == w_dict_) {
                sum++;
                if (VERBOSE) { std::cout << "dict is valid! sum --> " << sum << std::endl; }
            }

            dict.at(s_[l])--;
            r++;
            if (VERBOSE) { std::cout << "before next iteration... r --> " << r << ", dict --> " << dict << std::endl; }
        }



        return sum;
    }


private:

    const size m_;
    const std::string w_;
    const size n_;
    const std::string s_;

    const std::map<char, size> base_dict_;
    const std::map<char, size> w_dict_;

    static std::map<char, size> initialize_base_dict() {
        std::map<char, size> res;
        for (char c = 'a'; c <= 'z'; ++c) { res[c] = 0; }
        for (char c = 'A'; c <= 'Z'; ++c) { res[c] = 0; }

        return res;
    }

    std::map<char, size> make_dict(const std::string& str) {
        std::map<char, size> res;
        res.insert(base_dict_.begin(), base_dict_.end());
        for (auto c : str) { ++res.at(c); }

        return res;
    }
};


class ManualTester {
public:
    ManualTester() = default;

    static void test() {
        std::vector<std::map<std::string, std::string>> test_data = {

            // basic
            {{"m", "1"}, {"w", "a"}, {"n", "1"}, {"s", "a"}, {"true", "1"}},
            {{"m", "1"}, {"w", "a"}, {"n", "1"}, {"s", "b"}, {"true", "0"}},
            {{"m", "1"}, {"w", "b"}, {"n", "1"}, {"s", "a"}, {"true", "0"}},
            {{"m", "2"}, {"w", "ab"}, {"n", "2"}, {"s", "ab"}, {"true", "1"}},
            {{"m", "2"}, {"w", "bA"}, {"n", "2"}, {"s", "Ab"}, {"true", "1"}},

            // medium
            {{"m", "4"}, {"w", "cAda"}, {"n", "11"}, {"s", "AbrAcadAbRa"}, {"true", "2"}},

        };

        for (auto &line : test_data) {
            size m = std::stoi(line.at("m"));
            std::string w = line.at("w");
            size n = std::stoi(line.at("n"));
            std::string s = line.at("s");

            size res_true = std::stoi(line.at("true"));

            tt::steady_clock::time_point begin_greedy = tt::now();
            auto pred_greedy = GreedySolver(m, w, n, s).solve();
            tt::steady_clock::time_point end_greedy = tt::now();
            auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

            tt::steady_clock::time_point begin_fast = tt::now();
            auto pred_fast = FastSolver(m, w, n, s).solve();
            tt::steady_clock::time_point end_fast = tt::now();
            auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();

            std::cerr << "m = " << m << ", w = " << w << ", n = " << n << ", s = " << s << std::endl;
            std::cerr << std::scientific << "T = " << duration_greedy * 1e-9 << " s :: pred_greedy = " << pred_greedy << std::endl;
            std::cerr << std::scientific << "T = " << duration_fast * 1e-9 << " s :: pred_fast = " << pred_fast << std::endl;

            assert(res_true == pred_greedy);
            assert(res_true == pred_fast);
        }
    }
};



void stress_testing() {

    std::random_device rd;
    std::mt19937 gen(rd());

    const size MIN_M = 1, MAX_M = 3;
    std::uniform_int_distribution<> dist_M(MIN_M, MAX_M);

    const size MIN_N = 5, MAX_N = 10;
    std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

    const char MIN_VAL_UPPERCASE = 'A', MAX_VAL_UPPERCASE = 'Z';
    std::uniform_int_distribution<> dist_UPPERCASE(MIN_VAL_UPPERCASE, MAX_VAL_UPPERCASE);
    const char MIN_VAL_LOWERCASE = 'a', MAX_VAL_LOWERCASE = 'z';
    std::uniform_int_distribution<> dist_LOWERCASE(MIN_VAL_LOWERCASE, MAX_VAL_LOWERCASE);

    const int MIN_CHOICE = 0, MAX_CHOICE = 1;
    std::uniform_int_distribution<> dist_CHOICE(MIN_CHOICE, MAX_CHOICE);

    while (true) {
        std::cout << "======================" << std::endl;

        size m = dist_M(gen);
        std::string w;
        for (size i = 0; i < m; ++i) {
            auto choice = dist_CHOICE(gen);
            if (choice) { w += static_cast<char>(dist_UPPERCASE(gen)); }
            else { w += static_cast<char>(dist_LOWERCASE(gen)); }
        }

        size n = dist_N(gen);
        while (n < m) { n = dist_N(gen); }

        std::string s;
        for (size i = 0; i < n; ++i) {
            auto choice = dist_CHOICE(gen);
            if (choice) { s += static_cast<char>(dist_UPPERCASE(gen)); }
            else { s += static_cast<char>(dist_LOWERCASE(gen)); }
        }

        std::cout << "m = " << m << ", w = " << w << ", n = " << n << ", s = " << s << std::endl;

        tt::steady_clock::time_point begin_greedy = tt::now();
        auto res_greedy = GreedySolver(m, w, n, s).solve();
        tt::steady_clock::time_point end_greedy = tt::now();
        auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

        tt::steady_clock::time_point begin_fast = tt::now();
        auto res_fast = FastSolver(m, w, n, s).solve();
        tt::steady_clock::time_point end_fast = tt::now();
        auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();

        std::cout << std::scientific << "T = " << duration_greedy * 1e-9 << " s :: res_greedy = " << res_greedy << std::endl;
        std::cout << std::scientific << "T = " << duration_fast * 1e-9 << " s :: res_fast = " << res_fast << std::endl;

        assert(res_greedy == res_fast);

        w.clear();
        s.clear();
    }
}

int main() {

#if MODE == 1
    size m = 0, n = 0;
    std::cin >> m >> n;

    std::cin.ignore();

    std::string w, s;
    std::getline(std::cin, w);
    std::getline(std::cin, s);

    if (VERBOSE) {
        std::cout << "w = " << w << std::endl;
        std::cout << "s = " << s << std::endl;
    }

//    std::cout << GreedySolver(m, w, n, s).solve() << std::endl;
    std::cout << FastSolver(m, w, n, s).solve() << std::endl;

#elif MODE == 2
    ManualTester::test();

#elif MODE == 3
    stress_testing();

#endif

    return 0;
}