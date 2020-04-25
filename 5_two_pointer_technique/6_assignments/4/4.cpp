#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <cassert>
#include <exception>

// 1 - work, 2 - manual testing, 3 - stress_testing
#define MODE 1

#define VERBOSE false

using tt = std::chrono::steady_clock;
using size = int64_t;


std::ostream& operator<< (std::ostream& os, const std::map<char, size>& target) {
    for (auto item : target) { os << "{" << item.first << " : " << item.second << "}, "; }

    return os;
}


class GreedySolver {
public:
    GreedySolver(size n, std::string s, size m, std::string t)
            : n_(n), s_(std::move(s)), m_(m), t_(std::move(t)), base_dict_(initialize_base_dict()), t_dict_(make_dict(t_)) {

        if (VERBOSE) { std::cout << t_dict_ << std::endl; }

    }

    size solve() {
        size sum = 0;

        for (auto i = 0; i < n_; ++i) {
            for (auto j = i; j < n_; ++j) {
                std::string substr = s_.substr(i, std::min(static_cast<uint64_t>(j - i + 1), s_.length()));
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
    const size n_;
    const std::string s_;
    const size m_;
    const std::string t_;

    const std::map<char, size> base_dict_;
    const std::map<char, size> t_dict_;

    static std::map<char, size> initialize_base_dict() {
        std::map<char, size> res;
        for (char c = 'a'; c <= 'z'; ++c) { res[c] = 0; }

        return res;
    }

    std::map<char, size> make_dict(const std::string& str) {
        std::map<char, size> res;
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
    FastSolver(size n, std::string s, size m, std::string t)
            : n_(n), s_(std::move(s)), m_(m), t_(std::move(t)), base_dict_(initialize_base_dict()), t_dict_(make_dict(t_)) {}

    size solve() {
        size sum = 0;
        bool is_ok;
        std::map<char, size> dict;
        dict.insert(base_dict_.begin(), base_dict_.end());
        dict.at(s_[0])++;
        for (size l = 0, r = 0; l < n_; ++l) {
            while (r < n_ && is_possible_to_compose(dict)) {
                if (VERBOSE) { std::cout << ".....is possible to compose!" << std::endl; }

                r++;
                if (VERBOSE) { std::cout << ".....r --> " << r << std::endl; }

                if (r < n_) {
                    dict.at(s_[r])++;
                    if (VERBOSE) { std::cout << ".....dict --> " << dict << std::endl; }
                }
            }
            sum += r - l;
            if (VERBOSE) { std::cout << "sum --> " << sum << std::endl; }

            dict.at(s_[l])--;
            if (VERBOSE) { std::cout << "dict --> " << dict << std::endl; }
        }

        return sum;
    }


private:
    const size n_;
    const std::string s_;
    const size m_;
    const std::string t_;

    const std::map<char, size> base_dict_;
    const std::map<char, size> t_dict_;

    static std::map<char, size> initialize_base_dict() {
        std::map<char, size> res;
        for (char c = 'a'; c <= 'z'; ++c) { res[c] = 0; }

        return res;
    }

    std::map<char, size> make_dict(const std::string& str) {
        std::map<char, size> res;
        res.insert(base_dict_.begin(), base_dict_.end());
        for (auto c : str) { ++res.at(c); }

        return res;
    }

    bool is_possible_to_compose(std::map<char, size>& dict) {
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
            {{"n", "7_elementary_data_structures"}, {"s", "abacaba"}, {"m", "3"}, {"t", "abc"}, {"true", "15"}},
            {{"n", "8"}, {"s", "acbacaba"}, {"m", "3"}, {"t", "abc"}, {"true", "19"}},

            // additional
            {{"n", "22"}, {"s", "akablabcpabcdlabcdabcd"}, {"m", "4"}, {"t", "dacb"}, {"true", "46"}},
            {{"n", "20"}, {"s", "abcabaabcdabcdaababc"}, {"m", "4"}, {"t", "abcd"}, {"true", "54"}},
            {{"n", "8"}, {"s", "abcabaab"}, {"m", "14"}, {"t", "abcdadaadadada"}, {"true", "25"}},
            {{"n", "2"}, {"s", "ab"}, {"m", "7_elementary_data_structures"}, {"t", "dabcabc"}, {"true", "3"}},
            {{"n", "12"}, {"s", "abkabcpdeabc"}, {"m", "6"}, {"t", "aabbcc"}, {"true", "15"}},
            {{"n", "5"}, {"s", "abaad"}, {"m", "3"}, {"t", "aab"}, {"true", "9"}},
            {{"n", "5"}, {"s", "aaaaa"}, {"m", "1"}, {"t", "a"}, {"true", "5"}},
            {{"n", "4"}, {"s", "aaaa"}, {"m", "3"}, {"t", "aaa"}, {"true", "9"}},
            {{"n", "1"}, {"s", "a"}, {"m", "3"}, {"t", "aaa"}, {"true", "1"}},
            {{"n", "12"}, {"s", "abkabcpdeabc"}, {"m", "4"}, {"t", "zzzz"}, {"true", "0"}},
            {{"n", "18"}, {"s", "abcpabcdeebcqabcde"}, {"m", "5"}, {"t", "abcde"}, {"true", "42"}},
            {{"n", "17"}, {"s", "axajayatajataraha"}, {"m", "1"}, {"t", "a"}, {"true", "9"}},

    };

    for (auto& line : test_data) {
        size n = static_cast<size>(std::stoi(line.at("n")));
        std::string s = line.at("s");
        size m = static_cast<size>(std::stoi(line.at("m")));
        std::string t = line.at("t");

        size res_true = static_cast<size>(std::stoi(line.at("true")));
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

    const size MIN_N = 1, MAX_N = 100;
    std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

    const size MIN_M = 1, MAX_M = 100;
    std::uniform_int_distribution<> dist_M(MIN_M, MAX_M);

    const char MIN_VAL = 'a', MAX_VAL = 'z';
    std::uniform_int_distribution<> dist_VAL(MIN_VAL, MAX_VAL);

    while (true) {
        std::cout << "======================" << std::endl;

        size n = dist_N(gen);
        std::string s;
        for (size i = 0; i < n; ++i) {
            s += static_cast<char>(dist_VAL(gen));
        }

        size m = dist_M(gen);
        std::string t;
        for (size i = 0; i < m; ++i) {
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
    size n = 0, m = 0;
    std::cin >> n >> m;

    std::cin.ignore();

    std::string s, t;
    std::getline(std::cin, s);
    std::getline(std::cin, t);

    if (VERBOSE) {
        std::cout << "s = " << s << std::endl;
        std::cout << "t = " << t << std::endl;
    }

//    std::cout << GreedySolver(n, s, m, t).solve() << std::endl;
    std::cout << FastSolver(n, s, m, t).solve() << std::endl;

#elif MODE == 2
    manual_testing();

#elif MODE == 3
    stress_testing();

#endif


    return 0;
}