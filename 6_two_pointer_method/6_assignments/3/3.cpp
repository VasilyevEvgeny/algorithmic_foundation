//
// Created by evasilyev on 27.02.2020.
//

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <cassert>
#include <deque>

using tt = std::chrono::steady_clock;

// 1 - work, 2 - stress testing
#define MODE 1

#define VERBOSE true



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




/*
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
*/



size_t fast(size_t k, std::string& s) {
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
        if (s.length() == 1) { return (k == std::stod(s)) ? 1 : 0; }

        std::deque<size_t> units_coords;
        size_t l = 0, r = 0, n_units = 0, first_unit = 0, last_unit = 0;
        size_t n_it = 0;
        while (l < s.length() && r < s.length()) {
            while (n_units <= k && r < s.length()) {
                if (s[r] == '1') {
                    if (!n_units) { first_unit = r; }
                    if (n_units == k - 1) { last_unit = r; }
                    if (n_units <= k - 1) {
                        ++n_units;
                        units_coords.push_back(r);
                    }
                }
                if (r == s.length() - 1) { break; }
                if (n_units == k && s[r + 1] == '1') { break; }
                ++r;
            }

            if (n_units < k) { break; }

            size_t n_before = first_unit - l;
            size_t n_after = r - last_unit;

            sum += (n_before + 1) * (n_after + 1);

            if (VERBOSE) { std::cout << "l = " << l << ", r = " << r << ", first_unit = " << first_unit <<
                                     ", last_unit = " << last_unit << ", n_before = " << n_before << ", n_after = " << n_after <<
                                     ", units_coords = " << units_coords << ", SUM = " << sum << std::endl; }



            l = first_unit + 1;
            r = std::min(s.length() - 1, r + 1);
            if (l == r && k != 1) { break; }

            --n_units;
            units_coords.pop_front();
            if (!units_coords.empty()) { first_unit = units_coords[0]; }

            if (VERBOSE) { std::cout << "n_units = " << n_units << ", units_coords = " << units_coords <<
            ", l = " << l << ", first_unit = " << first_unit << std::endl; }

            if (n_it == 10) { exit(-1); }
            n_it++;
        }
    }

    return sum;
}





//size_t fast(size_t k, std::string& s) {
//    size_t sum = 0;
//    if (!k) {
//        size_t n = 0;
//        for (auto i = 0; i < s.length(); ++i) {
//            if (s[i] == '0') { n++; }
//            else {
//                sum += (1 + n) * n / 2;
//                if (VERBOSE) { std::cout << "n = " << n << ", sum is changed to " << sum << std::endl; }
//                n = 0;
//            }
//            if (i == s.length() - 1) {
//                sum += (1 + n) * n / 2;
//            }
//        }
//    }
//    else {
//        for (size_t l = 0; l < s.length(); ++l) {
//
//            // set left unit pos
//            if (s[l] == '0') { continue; }
//            size_t r = l;
//            if (VERBOSE) { std::cout << "l = r = " << l << std::endl; }
//
//            // find right unit pos
//            size_t k_local = (s[r] == '1') ? 1 : 0;
//            while (k_local < k && r < s.length() - 1) {
//                r++;
//                if (s[r] == '1') { k_local++; }
//                if (VERBOSE) { std::cout << "k_local = " << k_local << ", l = " << l << ", r = " << r << std::endl; }
//            }
//            if (k_local < k) { break; }
//
//            // find n_before
//            size_t n_before = 0, before = l;
//            if (l) {
//                while (s[before - 1] == '0' && before > 0) {
//                    before--;
//                }
//            }
//            n_before = l - before;
//            if (VERBOSE) { std::cout << "n_before = " << n_before << std::endl; }
//
//            // find n_after
//            size_t n_after = 0, after = r;
//            if (r < s.length() - 1) {
//                while (s[after + 1] == '0' && after < s.length() - 1) {
//                    after++;
//                }
//            }
//            n_after = after - r;
//            if (VERBOSE) { std::cout << "n_after = " << n_after << std::endl; }
//
//            sum += (n_before + 1) * (n_after + 1);
//            if (VERBOSE) { std::cout << "sum = " << sum << std::endl; }
//        }
//    }
//
//    return sum;
//}


//size_t fastest(size_t k, std::vector<size_t>& v) {
//    auto units = get_units(v);
//    if (VERBOSE) { std::cout << "units = " << units << std::endl; }
//
//    size_t sum = 0;
//    if (k > units.size()) {
//        return 0;
//    }
//    else if (!k) {
//        size_t n = 0;
//        for (auto i = 0; i < v.size(); ++i) {
//            if (v[i] == 0) { n++; }
//            else {
//                sum += (1 + n) * n / 2;
//                if (VERBOSE) { std::cout << "n = " << n << ", sum is changed to " << sum << std::endl; }
//                n = 0;
//            }
//            if (i == v.size() - 1) {
//                sum += (1 + n) * n / 2;
//            }
//        }
//    }
//    else {
//        bool out = false;
//        for (auto l = 0, r = 0; l < units.size(); ++l, r = l) {
//            while (r - l < k - 1) {
//                if (r < units.size() - 1) { r++; }
//                else { out = true; break; }
//            }
//            if (out) { break; }
//
//            size_t n_before = 0, n_after = 0;
//
//            if (!l) { n_before = units[0]; }
//            else { n_before = units[l] - units[l - 1] - 1; }
//
//            if (r == units.size() - 1) { n_after = v.size() - units[units.size()-1] - 1; }
//            else { n_after = units[r + 1] - units[r] - 1; }
//
//            sum += (n_before + 1) * (n_after + 1);
//
//            if (VERBOSE) { std::cout << "l = " << l << ", r = " << r << ", n_before = " << n_before << ", n_after = " << n_after << ", sum = " << sum << std::endl;}
//        }
//    }
//
//    return sum;
//}


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

        std::cout << "k = " << k << ", s = " << s << std::endl;

        tt::steady_clock::time_point begin_greedy = tt::now();
        auto res_greedy = greedy(k, s);
        tt::steady_clock::time_point end_greedy = tt::now();
        auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

        tt::steady_clock::time_point begin_fast = tt::now();
        auto res_fast = fast(k, s);
        tt::steady_clock::time_point end_fast = tt::now();
        auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();


        std::cout << std::scientific << "T = " << duration_greedy * 1e-9 << " s :: res_greedy = " << res_greedy << std::endl;
        std::cout << std::scientific << "T = " << duration_fast * 1e-9 << " s :: res_fast = " << res_fast << std::endl;

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
    std::getline(std::cin, s);
    if (VERBOSE) { std::cout << "s = " << s << std::endl; }

    std::cout << greedy(k, s) << std::endl;
    std::cout << fast(k, s) << std::endl;

#elif MODE == 2
    stress_testing();

#endif

    return 0;
}