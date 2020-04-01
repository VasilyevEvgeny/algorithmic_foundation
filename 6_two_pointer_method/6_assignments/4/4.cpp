#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <cassert>

// 1 - work, 2 - manual testing, 3 - stress_testing
#define MODE 3

#define VERBOSE false

using tt = std::chrono::steady_clock;


std::ostream& operator<< (std::ostream& os, const std::map<char, size_t>& target) {
    for (auto item : target) { os << "{" << item.first << " : " << item.second << "}, "; }

    return os;
}


class GreedySolver {
public:
    GreedySolver(size_t n, std::string s, size_t m, std::string t)
    : n_(n), s_(std::move(s)), m_(m), t_(std::move(t)), base_dict_(initialize_base_dict()), t_dict_(make_dict(t_)) {

     if (VERBOSE) { std::cout << t_dict_ << std::endl; }

    }

    size_t solve() {
        size_t sum = 0;

        for (auto i = 0; i < n_; ++i) {
            for (auto j = i; j < n_; ++j) {
                std::string substr = s_.substr(i, std::min(static_cast<size_t>(j - i + 1), s_.length()));
                if (VERBOSE) { std::cout << "substr = " << substr << std::endl; }

                if (is_possible_to_compose(substr)) {
                    if (VERBOSE) { std::cout << "--> possible!" << std::endl; }
                    ++sum;
                }
            }
        }

        return sum;
    }

private:
    const size_t n_;
    const std::string s_;
    const size_t m_;
    const std::string t_;

    // freq
    const std::map<char, size_t> base_dict_;
    const std::map<char, size_t> t_dict_;

    static std::map<char, size_t> initialize_base_dict() {
        std::map<char, size_t> res;
        for (char c = 'a'; c <= 'z'; ++c) { res[c] = 0; }

        return res;
    }

    std::map<char, size_t> make_dict(const std::string& str) {
        std::map<char, size_t> res;
        res.insert(base_dict_.begin(), base_dict_.end());
        for (auto c : str) { ++res.at(c); }

        return res;
    }

    bool is_possible_to_compose(std::string& substr) {
        // compose substr from symbols of string t
        auto substr_dict = make_dict(substr);
        for (auto item : substr_dict) {
            auto symbol = item.first;
            auto freq = item.second;
            if (t_dict_.at(symbol) < freq) {
                return false;
            }
        }
        return true;
    }
};



class FastSolver {
public:
    FastSolver(size_t n, std::string s, size_t m, std::string t)
    : n_(n), s_(std::move(s)), m_(m), t_(std::move(t)), base_dict_(initialize_base_dict()), t_dict_(make_dict(t_)) {

    }

    size_t solve() {
        size_t sum = 0;
        bool is_possible;
        bool go_next;
        std::map<char, size_t> dict;
        dict.insert(base_dict_.begin(), base_dict_.end());
        for (size_t l = 0, r = 0, r_prev = -1; l < n_ && r < n_; ++l) {
            go_next = false;

            do {
                if (VERBOSE) { std::cout << "l = " << l << ", r = " << r << std::endl; }
                auto substr = s_.substr(l, r - l + 1);
                if (VERBOSE) { std::cout << "substr = " << substr << std::endl; }
                dict.at(s_[r])++;

                if (VERBOSE) { std::cout << "dict --> " << dict << std::endl; }
                is_possible = is_possible_to_compose(dict);

                if (is_possible) {
                    if (VERBOSE) { std::cout << "is possible!!" << std::endl; }
                    if (r < n_ - 1) {
                        ++r;
                        if (VERBOSE) { std::cout << "r < n_ - 1, r --> " << r << std::endl; }
                    }
                    else {
                        if (VERBOSE) { std::cout << "r = n_ - 1, break " << std::endl; }
                        break;
                    }
                }
                else {
                    if (VERBOSE) { std::cout << "not possible!!" << std::endl; }
                    dict.at(s_[r]) = std::max(0, static_cast<int>(dict.at(s_[r])-1));
                    if (VERBOSE) { std::cout << "dict --> " << dict << std::endl; }
                    if (l == r) {
                        go_next = true;
                        ++r;
                        if (VERBOSE) { std::cout << "go_next = true, break" << std::endl; }
                        break;
                    }
                    --r;
                }
            } while (is_possible && l < n_ && r < n_);

            if (go_next) {
                dict.at(s_[l]) = std::max(0, static_cast<int>(dict.at(s_[l])-1));
                if (VERBOSE) { std::cout << "dict --> " << dict << std::endl; }
                continue;
            }

            auto n = r - l + 1;
            sum += (n + 1) * n / 2;
            if (VERBOSE) { std::cout << "sum plus --> " << sum << std::endl; }

            auto d = std::max(0, static_cast<int>(r_prev - l + 1));
            sum -= (d + 1) * d / 2;
            if (VERBOSE) { std::cout << "sum minus --> " << sum << std::endl; }

            if (r == n_ - 1) { break; }

            r_prev = r;

            ++r;
            dict.at(s_[l]) = std::max(0, static_cast<int>(dict.at(s_[l])-1));
            if (VERBOSE) { std::cout << "dict --> " << dict << std::endl; }


            if (VERBOSE) { std::cout << "======================" << std::endl; }
        }

        return sum;
    }


private:
    const size_t n_;
    const std::string s_;
    const size_t m_;
    const std::string t_;

    // dict
    const std::map<char, size_t> base_dict_;
    const std::map<char, size_t> t_dict_;

    static std::map<char, size_t> initialize_base_dict() {
        std::map<char, size_t> res;
        for (char c = 'a'; c <= 'z'; ++c) { res[c] = 0; }

        return res;
    }

    std::map<char, size_t> make_dict(const std::string& str) {
        std::map<char, size_t> res;
        res.insert(base_dict_.begin(), base_dict_.end());
        for (auto c : str) { ++res.at(c); }

        return res;
    }

    bool is_possible_to_compose(std::map<char, size_t>& dict) {
        for (auto item : dict) {
            auto symbol = item.first;
            auto freq = item.second;
            if (t_dict_.at(symbol) < freq) {
                return false;
            }
        }
        return true;
    }

};




void manual_testing() {
    std::vector<std::map<std::string, std::string>> test_data = {

            // basic
            {{"n", "1"}, {"s", "a"}, {"m", "1"}, {"t", "a"}, {"true", "1"}},
            {{"n", "1"}, {"s", "a"}, {"m", "1"}, {"t", "b"}, {"true", "0"}},
            {{"n", "1"}, {"s", "a"}, {"m", "2"}, {"t", "ab"}, {"true", "1"}},
            {{"n", "2"}, {"s", "ab"}, {"m", "1"}, {"t", "a"}, {"true", "1"}},

            // simple
            {{"n", "4"}, {"s", "aaab"}, {"m", "3"}, {"t", "aba"}, {"true", "8"}},
            {{"n", "3"}, {"s", "abc"}, {"m", "3"}, {"t", "def"}, {"true", "0"}},
            {{"n", "3"}, {"s", "abc"}, {"m", "4"}, {"t", "adef"}, {"true", "1"}},
            {{"n", "3"}, {"s", "abc"}, {"m", "4"}, {"t", "dbef"}, {"true", "1"}},
            {{"n", "3"}, {"s", "abc"}, {"m", "4"}, {"t", "defa"}, {"true", "1"}},
            {{"n", "5"}, {"s", "aaaaa"}, {"m", "5"}, {"t", "aaaaa"}, {"true", "15"}},
            {{"n", "5"}, {"s", "abcde"}, {"m", "5"}, {"t", "de"}, {"true", "3"}},

            // medium
            {{"n", "7"}, {"s", "abacaba"}, {"m", "3"}, {"t", "abc"}, {"true", "15"}},
            {{"n", "8"}, {"s", "acbacaba"}, {"m", "3"}, {"t", "abc"}, {"true", "19"}},

    };

    for (auto& line : test_data) {
        size_t n = static_cast<size_t>(std::stoi(line.at("n")));
        std::string s = line.at("s");
        size_t m = static_cast<size_t>(std::stoi(line.at("m")));
        std::string t = line.at("t");

        size_t res_true = static_cast<size_t>(std::stoi(line.at("true")));
        auto pred_greedy = GreedySolver(n, s, m, t).solve();
        auto pred_fast = FastSolver(n, s, m, t).solve();

        std::cerr << "n = " << n << ", s = " << s << ", m = " << m <<  ", t = " << t << ", pred_greedy = " <<
        pred_greedy << ", pred_fast = " << pred_fast << std::endl;

        assert(res_true == pred_greedy);
        assert(res_true == pred_fast);
    }
}


void stress_testing() {

    std::random_device rd;
    std::mt19937 gen(rd());

    const size_t MIN_N = 1, MAX_N = 100;
    std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

    const size_t MIN_M = 1, MAX_M = 100;
    std::uniform_int_distribution<> dist_M(MIN_M, MAX_M);

    const char MIN_VAL = 'a', MAX_VAL = 'z';
    std::uniform_int_distribution<> dist_VAL(MIN_VAL, MAX_VAL);

    while (true) {
        std::cout << "======================" << std::endl;

        size_t n = dist_N(gen);
        std::string s;
        for (size_t i = 0; i < n; ++i) {
            s += static_cast<char>(dist_VAL(gen));
        }

        size_t m = dist_M(gen);
        std::string t;
        for (size_t i = 0; i < m; ++i) {
            t += static_cast<char>(dist_VAL(gen));
        }

        std::cout << "n = " << n << ", s = " << s << ", m = " << m << ", t = " << t << std::endl;

        tt::steady_clock::time_point begin_greedy = tt::now();
        auto res_greedy = GreedySolver(n, s, m ,t).solve();
        tt::steady_clock::time_point end_greedy = tt::now();
        auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

        tt::steady_clock::time_point begin_fast = tt::now();
        auto res_fast = FastSolver(n, s, m ,t).solve();
        tt::steady_clock::time_point end_fast = tt::now();
        auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();

        std::cout << std::scientific << "T = " << duration_greedy * 1e-9 << " s :: res_greedy = " << res_greedy << std::endl;
        std::cout << std::scientific << "T = " << duration_fast * 1e-9 << " s :: res_fast = " << res_fast << std::endl;

        assert(res_greedy == res_fast);

        s.clear();
        t.clear();
    }
}



int main() {

#if MODE == 1
    size_t n = 0, m = 0;
    std::cin >> n >> m;

    std::cin.ignore();

    std::string s, t;
    std::getline(std::cin, s);
    std::getline(std::cin, t);

    if (VERBOSE) {
        std::cout << "s = " << s << std::endl;
        std::cout << "t = " << t << std::endl;
    }

    std::cout << GreedySolver(n, s, m, t).solve() << std::endl;
    std::cout << FastSolver(n, s, m, t).solve() << std::endl;

#elif MODE == 2
    manual_testing();

#elif MODE == 3
    stress_testing();

#endif


    return 0;
}