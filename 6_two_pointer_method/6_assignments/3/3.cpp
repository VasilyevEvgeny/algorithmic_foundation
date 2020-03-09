//
// Created by evasilyev on 27.02.2020.
//

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cassert>

// 1 - work, 2 - stress testing
#define MODE 1

#define VERBOSE false



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


std::vector<size_t> get_v(std::string& s) {
    std::vector<size_t> v;
    for (auto e : s) { v.push_back(e - '0'); }

    return v;
}

std::vector<size_t> get_units(std::vector<size_t>& v) {
    std::vector<size_t> units;
    for (auto i = 0; i < v.size(); ++i) {
        if (v[i] == 1) { units.push_back(i); }
    }

    return units;
}


size_t optimal(size_t k, std::string& s) {
    size_t sum = 0;
    if (!k) {
        size_t n = 0;
        for (auto i = 0; i < s.length(); ++i) {
            if (s[i] == '0') { n++; }
            else {
                sum += (1 + n) * n / 2;
                if (VERBOSE) { std::cout << "n = " << n << ", sum is changed to " << sum << std::endl; }
                n = 0;
            }
            if (i == s.length() - 1) {
                sum += (1 + n) * n / 2;
            }
        }
    }
    else {
        for (size_t l = 0; l < s.length(); ++l) {

            // set left unit pos
            if (s[l] == '0') { continue; }
            size_t r = l;
            if (VERBOSE) { std::cout << "l = r = " << l << std::endl; }

            // find right unit pos
            size_t k_local = (s[r] == '1') ? 1 : 0;
            while (k_local < k && r < s.length() - 1) {
                r++;
                if (s[r] == '1') { k_local++; }
                if (VERBOSE) { std::cout << "k_local = " << k_local << ", l = " << l << ", r = " << r << std::endl; }
            }
            if (k_local < k) { break; }

            // find n_before
            size_t n_before = 0, before = l;
            if (l) {
                while (s[before - 1] == '0' && before > 0) {
                    before--;
                }
            }
            n_before = l - before;
            if (VERBOSE) { std::cout << "n_before = " << n_before << std::endl; }

            // find n_after
            size_t n_after = 0, after = r;
            if (r < s.length() - 1) {
                while (s[after + 1] == '0' && after < s.length() - 1) {
                    after++;
                }
            }
            n_after = after - r;
            if (VERBOSE) { std::cout << "n_after = " << n_after << std::endl; }

            sum += (n_before + 1) * (n_after + 1);
            if (VERBOSE) { std::cout << "sum = " << sum << std::endl; }
        }
    }

    return sum;
}


size_t fastest(size_t k, std::vector<size_t>& v) {
    auto units = get_units(v);
    if (VERBOSE) { std::cout << "units = " << units << std::endl; }

    size_t sum = 0;
    if (k > units.size()) {
        return 0;
    }
    else if (!k) {
        size_t n = 0;
        for (auto i = 0; i < v.size(); ++i) {
            if (v[i] == 0) { n++; }
            else {
                sum += (1 + n) * n / 2;
                if (VERBOSE) { std::cout << "n = " << n << ", sum is changed to " << sum << std::endl; }
                n = 0;
            }
            if (i == v.size() - 1) {
                sum += (1 + n) * n / 2;
            }
        }
    }
    else {
        bool out = false;
        for (auto l = 0, r = 0; l < units.size(); ++l, r = l) {
            while (r - l < k - 1) {
                if (r < units.size() - 1) { r++; }
                else { out = true; break; }
            }
            if (out) { break; }

            size_t n_before = 0, n_after = 0;

            if (!l) { n_before = units[0]; }
            else { n_before = units[l] - units[l - 1] - 1; }

            if (r == units.size() - 1) { n_after = v.size() - units[units.size()-1] - 1; }
            else { n_after = units[r + 1] - units[r] - 1; }

            sum += (n_before + 1) * (n_after + 1);

            if (VERBOSE) { std::cout << "l = " << l << ", r = " << r << ", n_before = " << n_before << ", n_after = " << n_after << ", sum = " << sum << std::endl;}
        }
    }

    return sum;
}


size_t greedy(size_t k, std::string& s) {
    size_t sum = 0;
    for (auto l = 0; l < s.length(); ++l) {
        for (auto r = l; r < s.length(); ++r) {
            size_t k_local = 0;
            for (size_t i = l; i <= r; ++i) {
                if (s[i] == '1') { k_local++; }
            }
            if (k_local == k) { sum++; }
        }
    }

    return sum;
}


void stress_testing() {

    std::random_device rd;
    std::mt19937 gen(rd());

    const size_t MIN_K = 0, MAX_K = 10;
    std::uniform_int_distribution<> dist_K(MIN_K, MAX_K);

    const size_t MIN_VAL = 0, MAX_VAL = 1;
    std::uniform_int_distribution<> dist_VAL(MIN_VAL, MAX_VAL);

    const int MIN_L = 1, MAX_L = 10;
    std::uniform_int_distribution<> dist_L(MIN_L, MAX_L);

    while (true) {
        std::cout << "======================" << std::endl;

        size_t k = dist_K(gen);
        size_t length = dist_L(gen);

        std::string s;
        for (size_t i = 0; i < length; ++i) {
            auto val = dist_VAL(gen);
            s += (val) ? '1' : '0';
        }

        auto res_greedy = greedy(k, s);
        auto res_fast = optimal(k, s);

        std::cout << "k = " << k << ", s = " << s <<
                  ", res_greedy = " << res_greedy <<", res_fast = " << res_fast << std::endl;

        assert(res_greedy == res_fast);

        s.clear();
    }
}



int main() {

#if MODE == 1

    size_t k = 0;
    std::cin >> k;
    std::cin.ignore();
    if (VERBOSE) { std::cout << "k = " << k << std::endl; }

    std::string s;
    std::getline (std::cin, s);
    if (VERBOSE) { std::cout << "s = " << s << std::endl; }

//    std::cout << greedy(k, s) << std::endl;
    std::cout << optimal(k, s) << std::endl;

#elif MODE == 2
    stress_testing();

#endif

    return 0;
}