//
// Created by evasilyev on 27.02.2020.
//

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <cassert>
#include <deque>
#include <map>

using tt = std::chrono::steady_clock;

// 1 - work, 2 - manual testing, 3 - stress testing
#define MODE 1

#define VERBOSE false


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


bool is_in_units_coords(size_t pos, std::deque<size_t>& units_coords) {
    auto size = units_coords.size();
    if (!size) { return false; }
    else { return pos == units_coords[size - 1]; }
}

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
//        size_t n_it = 0;
        while (l < s.length() && r < s.length()) {
            r = std::max(l, r);

            // initialize coefficients
            while (n_units <= k && r < s.length()) {
                if (n_units < k) {
                    if (s[r] == '1' && !is_in_units_coords(r, units_coords)) {
                        if (!n_units) { first_unit = r; }
                        if (n_units == k - 1) { last_unit = r; }
                        ++n_units;
                        units_coords.push_back(r);

                        if (VERBOSE) { std::cout << "s[" << r << "] = 1, n_units = " << n_units << ", units_coords = " << units_coords << std::endl; }
                    }
                    if (r < s.length() - 1) { ++r; }
                    else { break; }
                    if (VERBOSE) { std::cout << "1 -> increase r to " << r << std::endl; }
                }
                else {
                    if (VERBOSE) { std::cout << "n_units = k = " << n_units << "!" << std::endl; }
                    if (s[r] == '0') {
                        if (r < s.length() - 1) { ++r; }
                        else { break; }
                        if (VERBOSE) { std::cout << "2 -> increase r to " << r << std::endl; }
                    }
                    else {
                        --r;
                        if (VERBOSE) { std::cout << "s[" << r << "] = 1, make r = " << r << " and break!" << std::endl; }
                        break;
                    }
                }
            }
            if (n_units < k) { break; }

            size_t n_before = first_unit - l;
            size_t n_after = r - last_unit;

            sum += (n_before + 1) * (n_after + 1);

            if (VERBOSE) { std::cout << "===============> l = " << l << ", r = " << r << ", first_unit = " << first_unit <<
                                     ", last_unit = " << last_unit << ", n_before = " << n_before << ", n_after = " << n_after <<
                                     ", units_coords = " << units_coords << ", SUM = " << sum << std::endl; }

            l = first_unit + 1;
            --n_units;
            units_coords.pop_front();
            if (!units_coords.empty()) { first_unit = units_coords[0]; }

            if (VERBOSE) { std::cout << "==> n_units = " << n_units << ", units_coords = " << units_coords <<
            ", l = " << l << ", first_unit = " << first_unit << std::endl; }

//            if (n_it == 10) { exit(-1); }
//            n_it++;
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


void manual_testing() {
    std::vector<std::map<std::string, std::string>> test_data = {

            // simple
            {{"k", "1"}, {"s", "0"}, {"true", "0"}},
            {{"k", "1"}, {"s", "1"}, {"true", "1"}},
            {{"k", "1"}, {"s", "11"}, {"true", "2"}},
            {{"k", "1"}, {"s", "111"}, {"true", "3"}},
            {{"k", "2"}, {"s", "1"}, {"true", "0"}},
            {{"k", "2"}, {"s", "11"}, {"true", "1"}},
            {{"k", "2"}, {"s", "111"}, {"true", "2"}},
            {{"k", "2"}, {"s", "011"}, {"true", "2"}},
            {{"k", "2"}, {"s", "110"}, {"true", "2"}},

            // k = 1
            {{"k", "2"}, {"s", "0001000"}, {"true", "0"}},
            {{"k", "5"}, {"s", "100110010"}, {"true", "0"}},
            {{"k", "1"}, {"s", "0001"}, {"true", "4"}},
            {{"k", "1"}, {"s", "1010"}, {"true", "6"}},
            {{"k", "1"}, {"s", "0010"}, {"true", "6"}},
            {{"k", "1"}, {"s", "01000"}, {"true", "8"}},
            {{"k", "1"}, {"s", "011011"}, {"true", "7"}},
            {{"k", "1"}, {"s", "000010100001"}, {"true", "25"}},

            // medium
            {{"k", "2"}, {"s", "10011010"}, {"true", "9"}},
    };

    for (const auto& line : test_data) {
        size_t k = std::stoi(line.at("k"));
        std::string s = line.at("s");

        size_t res_true = std::stoi(line.at("true"));
        auto pred_greedy = greedy(k, s);
        auto pred_fast = greedy(k, s);

        std::cerr << "k = " << k << ", s = " << s << ", true = " << res_true <<  ", pred_greedy = " << pred_greedy <<
        ", pred_fast = " << pred_fast << std::endl;

        assert(res_true == pred_greedy);
        assert(res_true == pred_fast);
    }
}


void stress_testing() {

    std::random_device rd;
    std::mt19937 gen(rd());

    const size_t MIN_K = 0, MAX_K = 1000;
    std::uniform_int_distribution<> dist_K(MIN_K, MAX_K);

    const size_t MIN_VAL = 0, MAX_VAL = 1;
    std::uniform_int_distribution<> dist_VAL(MIN_VAL, MAX_VAL);

    const int MIN_L = 1, MAX_L = 1000;
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
        auto res_greedy = 0; //greedy(k, s);
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

//    std::cout << greedy(k, s) << std::endl;
    std::cout << fast(k, s) << std::endl;

#elif MODE == 2
    manual_testing();

#elif MODE == 3
    stress_testing();

#endif

    return 0;
}