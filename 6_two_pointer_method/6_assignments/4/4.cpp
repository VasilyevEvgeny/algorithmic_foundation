//
// Created by evasilyev on 25.03.2020.
//

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

// 1 - work, 2 - manual testing, 3 - stress_testing
#define MODE 1

#define VERBOSE true

std::ostream& operator<< (std::ostream& os, const std::map<char, size_t>& target) {
    for (auto item : target) { os << "{" << item.first << " : " << item.second << "}, "; }

    return os;
}


class GreedySolver {
public:
    GreedySolver(size_t n, std::string s, size_t m, std::string t)
    : n_(n), s_(std::move(s)), m_(m), t_(std::move(t)), base_freq_(initialize_base_freq()), t_freq_(make_freq(t_)) {

     if (VERBOSE) { std::cout << t_freq_ << std::endl; }

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
    const std::map<char, size_t> base_freq_;
    const std::map<char, size_t> t_freq_;

    static std::map<char, size_t> initialize_base_freq() {
        std::map<char, size_t> res;
        for (char c = 97; c <= 122; ++c) { res[c] = 0; }

        return res;
    }

    std::map<char, size_t> make_freq(const std::string& str) {
        std::map<char, size_t> res;
        res.insert(base_freq_.begin(), base_freq_.end());
        for (auto c : str) { ++res.at(c); }

        return res;
    }

    bool is_possible_to_compose(std::string& substr) {
        // compose substr from symbols of string t
        auto substr_dict = make_freq(substr);
        for (auto item : substr_dict) {
            auto symbol = item.first;
            auto freq = item.second;
            if (t_freq_.at(symbol) < freq) {
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


//    size_t process() {
//
//
//
//        size_t sum = 0;
//
//        std::map<char, size_t> dict;
//        dict.insert(base_dict_.begin(), base_dict_.end());
//
//        size_t n_it = 0;
//
//        bool is_possible;
//        size_t l = 0, r = 0;
//        while (l < n_ && r < n_) {
//            if (n_it && !l && !r) { break; }
//            if (VERBOSE) { std::cout << "dict_ = " << dict << std::endl; }
//
//            do {
//                is_possible = dict.at(s_[r]) < t_dict_.at(s_[r]);
//                if (VERBOSE) {
//                    if (is_possible) {
//                        std::cout << "s[" << r << "] = " << s_.at(r) << ", " << "dict[" << s_.at(r) << "] "  <<  dict.at(s_[r]) << " < t_dict_[" << s_.at(r) << "] "
//                                  << t_dict_.at(s_[r]) << " --> possible!" << std::endl;
//                    }
//                    else {
//                        std::cout << "s[" << r << "] = " << s_.at(r) << ", " << "dict[" << s_.at(r) << "] "  <<  dict.at(s_[r]) << " >= t_dict_[" << s_.at(r) << "] "
//                                  << t_dict_.at(s_[r]) << " --> not possible!" << std::endl;
//                    }
//                }
//
//                if (is_possible) {
//                    dict.at(s_[r])++;
//                    if (VERBOSE) { std::cout << "dict[" << s_.at(r) << "]++ " << dict.at(s_[r]) << std::endl; }
//
//                    if (r == n_ - 1) {
//                        if (VERBOSE) { std::cout << "r = n - 1, break " << r << std::endl; }
//                        break;
//                    }
//                    else {
//                        ++r;
//                        if (VERBOSE) { std::cout << "r++ " << r << std::endl; }
//                    }
//                }
//                else {
//                    if (!r) { return 1; }
//                    --r;
//                    if (VERBOSE) { std::cout << "r-- " << r << std::endl; }
//                    break;
//                }
//
//            } while (is_possible && r < n_);
//
//            auto n = r - l + 1;
//            sum += (n + 1) * n / 2 - 1;
//
//            if (VERBOSE) { std::cout << "first dict: " << dict << std::endl; }
//            if (VERBOSE) { std::cout << "l = " << l << ", r = " << r << ", substr = " << s_.substr(l, r - l + 1) <<  ", sum = " << sum << std::endl; }
//
//            if (l == n_ - 1 or r == n_ - 1) {
//                sum++;
//                if (VERBOSE) { std::cout << "!!!!! l = " << l << ", r = " << r << ", sum++ " << sum << std::endl; }
//                break;
//            }
//
//
//            for (auto i = l; i < r; ++i) { dict.at(s_[i])--; }
//            if (VERBOSE) { std::cout << "after minus: " << dict << std::endl; }
//
//            l = r;
//            dict.clear();
//            dict.insert(base_dict_.begin(), base_dict_.end());
//
//            if (VERBOSE) { std::cout << "====================" << std::endl; }
//
//            if (n_it == 10) { exit(-1); }
//            n_it++;
//        }
//
//
//        return sum;
//    }


    size_t solve() {
        size_t sum = 0;

        size_t l = 0, r = 0;
        std::string substr;
        bool is_possible;
        size_t n_it = 0;
        while (l < n_ && r < n_) {
            do {
                auto diff = r - l;
                substr = s_.substr(l, diff + 1);
                if (VERBOSE) { std::cout << "l = " << l << ", r = " << r << ", substr = " << substr << std::endl; }

                is_possible = is_possible_to_compose(substr);
                if (is_possible) {
                    if (VERBOSE) { std::cout << "is possible!" << std::endl; }
                    if (r == n_ - 1) {
                        if (VERBOSE) { std::cout << "r = n_ - 1!!!" << std::endl; }
                        break;
                    }
                    else {
                        r++;
                        if (VERBOSE) { std::cout << "r increased to " << r << std::endl; }
                    }
                }
                else {
                    if (VERBOSE) { std::cout << "is not possible!" << std::endl; }
                    if (diff > 1) {
                        r--;
                        if (VERBOSE) { std::cout << "r-- " << r << std::endl; }
                    }
                    else {
                        ++l;
                        r = l;
                        if (VERBOSE) { std::cout << "l = r = " << r << std::endl; }
                    }
                }

            } while (is_possible && r < n_);

            auto n = r - l;
            auto ds = (n + 1) * n / 2;
            if (n <= 1) { sum += ds; }
            else { sum += ds - 1; }
            if (VERBOSE) { std::cout << "sum increased to = " << sum << std::endl; }

            if (r == n_ - 1) {
                substr = s_.substr(n_- 1, 1);
                if (is_possible_to_compose(substr)) {
                    sum++;
                    if (VERBOSE) { std::cout << "sum++ " << sum << std::endl; }
                }

                if (VERBOSE) { std::cout << "!!! r = n_ - 1" << std::endl; }
                break;
            }

            l = r;
            if (VERBOSE) { std::cout << "=============" << std::endl; }


            if (n_it == 10) {
                exit(-1);
            }
            n_it++;
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
        for (char c = 97; c <= 122; ++c) { res[c] = 0; }

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
        auto dict = make_dict(substr);
        for (auto item : dict) {
            auto symbol = item.first;
            auto freq = item.second;
            if (t_dict_.at(symbol) < freq) {
                return false;
            }
        }
        return true;
    }

//    bool is_possible_to_compose(std::map<char, size_t>& dict) {
//        // compose substr from symbols of string t
//        for (auto item : dict) {
//            auto symbol = item.first;
//            auto freq = item.second;
//            if (t_dict_.at(symbol) < freq) {
//                return false;
//            }
//        }
//        return true;
//    }

};




void manual_testing() {
    std::vector<std::map<std::string, std::string>> test_data = {

            // basic
            {{"n", "1"}, {"s", "a"}, {"m", "1"}, {"t", "a"}, {"true", "1"}},
            {{"n", "1"}, {"s", "a"}, {"m", "1"}, {"t", "b"}, {"true", "0"}},
            {{"n", "1"}, {"s", "a"}, {"m", "2"}, {"t", "ab"}, {"true", "1"}},
//            {{"n", "2"}, {"s", "ab"}, {"m", "1"}, {"t", "a"}, {"true", "1"}},

            // simple
            {{"n", "4"}, {"s", "aaab"}, {"m", "3"}, {"t", "aba"}, {"true", "8"}},
            {{"n", "3"}, {"s", "abc"}, {"m", "3"}, {"t", "def"}, {"true", "0"}},
            {{"n", "3"}, {"s", "abc"}, {"m", "4"}, {"t", "adef"}, {"true", "1"}},

            // medium
            {{"n", "7"}, {"s", "abacaba"}, {"m", "3"}, {"t", "abc"}, {"true", "15"}},

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

//    std::cout << GreedySolver(n, s, m, t).solve() << std::endl;
    std::cout << FastSolver(n, s, m, t).solve() << std::endl;

#elif MODE == 2
    manual_testing();

#elif MODE == 3
//    manual_testing();

#endif


    return 0;
}