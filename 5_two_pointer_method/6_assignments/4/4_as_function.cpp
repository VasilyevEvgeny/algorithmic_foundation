#include <iostream>
#include <map>

#define VERBOSE false

using size = uint64_t;


std::ostream& operator<< (std::ostream& os, const std::map<char, size>& target) {
    for (auto item : target) { os << "{" << item.first << " : " << item.second << "}, "; }

    return os;
}


size solve(const size n_, const std::string& s_, const size m_, const std::string& t_) {

    //
    // initializations
    //

    std::map<char, size> base_dict_, t_dict_;
    for (char c = 'a'; c <= 'z'; ++c) { base_dict_[c] = 0; t_dict_[c] = 0; }

    for (auto c : t_) { ++t_dict_.at(c); }

    //
    // solution
    //

    size sum = 0;
    bool is_possible;
    bool go_next;
    std::map<char, size> dict;
    dict.insert(base_dict_.begin(), base_dict_.end());
    for (size l = 0, r = 0, r_prev = -1; l < n_ && r < n_; ++l) {
        go_next = false;

        do {
            if (VERBOSE) { std::cout << "l = " << l << ", r = " << r << std::endl; }
            auto substr = s_.substr(l, r - l + 1);
            if (VERBOSE) { std::cout << "substr = " << substr << std::endl; }
            dict.at(s_[r])++;

            if (VERBOSE) { std::cout << "dict --> " << dict << std::endl; }

            // is possible to compose or not
            is_possible = true;
            for (auto item : dict) {
                auto symbol = item.first;
                auto freq = item.second;
                if (t_dict_.at(symbol) < freq) {
                    is_possible = false;
                    break;
                }
            }

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


int main () {

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

    std::cout << solve(n, s, m, t) << std::endl;

    return 0;
}