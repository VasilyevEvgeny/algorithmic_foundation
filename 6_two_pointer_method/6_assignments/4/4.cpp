//
// Created by evasilyev on 25.03.2020.
//

#include <iostream>
#include <string>
#include <map>

#define VERBOSE false

std::ostream& operator<< (std::ostream& os, const std::map<char, size_t>& target) {
    for (auto item : target) { os << "{" << item.first << " : " << item.second << "}, "; }

    return os;
}


class GreedySolver {
public:
    GreedySolver(size_t n, std::string& s, size_t m, std::string& t)
    : n_(n), s_(std::move(s)), m_(m), t_(std::move(t)), base_freq_(initialize_base_freq()), t_freq_(make_freq(t_)) {

     if (VERBOSE) { std::cout << t_freq_ << std::endl; }

    }

    size_t process() {
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


    static std::map<char, size_t> initialize_base_freq() {
        std::map<char, size_t> res;
        for (char c = 97; c <= 122; ++c) { res[c] = 0; }

        return res;
    }

private:
    const size_t n_;
    const std::string s_;
    const size_t m_;
    const std::string t_;

    // freq
    const std::map<char, size_t> base_freq_;
    const std::map<char, size_t> t_freq_;

    std::map<char, size_t> make_freq(const std::string& str) {
        std::map<char, size_t> res;
        res.insert(base_freq_.begin(), base_freq_.end());
        for (auto c : str) { ++res.at(c); }

        return res;
    }


    bool is_possible_to_compose(std::string& substr) {
        // compose substr from symbols of string t
        auto substr_freq = make_freq(substr);
        for (auto item : substr_freq) {
            auto symbol = item.first;
            auto freq = item.second;
            if (t_freq_.at(symbol) < freq) {
                return false;
            }
        }
        return true;
    }

};





int main() {

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

    std::cout << GreedySolver(n, s, m, t).process() << std::endl;

    return 0;
}