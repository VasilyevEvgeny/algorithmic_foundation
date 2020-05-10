//
// Created by evasilyev on 03.05.2020.
//

#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <list>
#include <cmath>
#include <random>
#include <chrono>
#include <cassert>

// 1 - work, 3 - stress testing
#define MODE 1

#define VERBOSE false

using size = int64_t;
using tt = std::chrono::steady_clock;


std::ostream& operator << (std::ostream& os, const std::list<size>& lst) {
    if (lst.empty()) { os << "nullptr"; return os; }

    os << "nullptr -> ";
    size i = 1;
    for (auto& node : lst) {
        os << "#" << i << ": " << node << " -> ";
        ++i;
    }
    os << "nullptr";

    return os;
}


std::ostream& operator << (std::ostream& os, const std::vector<size>& v) {
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


template <typename T, typename U>
std::ostream& operator << (std::ostream& os, const std::map<T, U>& m) {
    os << "{";
    for (auto& e : m) {
        os << "{" << e.first << " : " << e.second << "}";
    }
    os << "}";
    return os;
}


std::ostream& operator << (std::ostream& os, const std::vector<std::list<size>>& v) {
    os << "########## GROUPS: ##########\n";
    for (size_t i = 0; i < v.size(); ++i) {
        os << "#" << i << " --> " << v[i] << std::endl;
    }
    return os;
}



class Greedy {
public:
    Greedy(size n, std::vector<size>& a, size k, std::vector<size>& e, std::vector<size>& v)
    : n_(n), a_(a), k_(k), e_(e), v_(v) {
        for (size i = 0; i < n_; ++i) {
            enterprises_.push_back(a_[i]);
            sum_of_squares += std::pow(a_[i], 2);
        }

        if (VERBOSE) { std::cout << "Enterprises initial: " << enterprises_ << std::endl; }
    }

    void make_evolution() {

#if MODE == 1
        std::cout << sum_of_squares << std::endl;
#endif
        for (size i = 0; i < k_; ++i) {

            if (e_[i] == 1) { bankrupt(v_[i]); }
            else if (e_[i] == 2) { divide(v_[i]); }

#if MODE == 1
            std::cout  << sum_of_squares << std::endl;
#endif

            if (VERBOSE) {
                std::cout << "enterprises_ = " << enterprises_ << std::endl;
                std::cout << "===========================================" << std::endl;
            }
        }

        if (VERBOSE) { std::cout << "Enterprises after evolution: " << enterprises_ << std::endl; }
    }

private:

    static std::pair<size, size> calculate_portions(size sum) {
        size first = sum / 2;
        size second = sum - first;
        if (VERBOSE) {
            std::cout << ".....first = " << first << std::endl;
            std::cout << ".....second = " << second << std::endl;
        }

        return {first, second};
    }

    void divide(size j) {
        if (VERBOSE) { std::cout << "===> go divide! j = " << j << std::endl; }
        auto it = enterprises_.begin();
        std::advance(it, j - 1);

        auto portions = calculate_portions(*it);

        // add first portion
        enterprises_.insert(it, portions.first);
        sum_of_squares += std::pow(portions.first, 2);
        if (VERBOSE) { std::cout << "add first portion squared, sum_of_squares --> " << sum_of_squares << std::endl; }

        // add second portion
        enterprises_.insert(it, portions.second);
        sum_of_squares += std::pow(portions.second, 2);
        if (VERBOSE) { std::cout << "add second portion squared, sum_of_squares --> " << sum_of_squares << std::endl; }

        // erase initial element
        sum_of_squares -= std::pow(*it, 2);
        enterprises_.erase(it);
        if (VERBOSE) { std::cout << "remove initial squared, sum_of_squares --> " << sum_of_squares << std::endl; }
    }

    void bankrupt(size j) {
        if (VERBOSE) { std::cout << "===> go bankrupt! j = " << j << std::endl; }
        if (j == 1) {
            auto it = enterprises_.begin();
            sum_of_squares -= std::pow(*it, 2);
            sum_of_squares -= std::pow(*std::next(it), 2);
            if (VERBOSE) { std::cout << "j = 1, remove cur and next squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            *std::next(it) += *it;
            sum_of_squares += std::pow(*std::next(it), 2);
            if (VERBOSE) { std::cout << "j = 1, add next squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            enterprises_.erase(it);
        }
        else if (j == enterprises_.size()) {
            auto it = enterprises_.end();
            sum_of_squares -= std::pow(*std::prev(std::prev(it)), 2);
            sum_of_squares -= std::pow(*std::prev(it), 2);
            if (VERBOSE) { std::cout << "j = size, remove prev-prev and prev squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            *std::prev(std::prev(it)) += *std::prev(it);
            sum_of_squares += std::pow(*std::prev(std::prev(it)), 2);
            if (VERBOSE) { std::cout << "j = size, add prev squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            enterprises_.erase(std::prev(it));
        }
        else {
            auto it = enterprises_.begin();
            std::advance(it, j - 1);

            auto portions = calculate_portions(*it);

            //cur
            sum_of_squares -= std::pow(*it, 2);
            if (VERBOSE) { std::cout << "remove cur squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            // prev
            sum_of_squares -= std::pow(*std::prev(it), 2);
            if (VERBOSE) { std::cout << "remove prev squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            *std::prev(it) += portions.first;
            sum_of_squares += std::pow(*std::prev(it), 2);
            if (VERBOSE) { std::cout << "add prev squared, sum_of_squares --> " << sum_of_squares << std::endl; }

            // next
            sum_of_squares -= std::pow(*std::next(it), 2);
            if (VERBOSE) { std::cout << "remove next squared, sum_of_squares --> " << sum_of_squares << std::endl; }
            *std::next(it) += portions.second;
            sum_of_squares += std::pow(*std::next(it), 2);
            if (VERBOSE) { std::cout << "add next squared, sum_of_squares --> " << sum_of_squares << std::endl; }

            enterprises_.erase(it);
        }
    }

    const size n_;
    std::vector<size> a_;
    const size k_;
    std::vector<size> e_;
    std::vector<size> v_;

    std::list<size> enterprises_;
    size sum_of_squares = 0;
};


class Fast {
public:
    Fast(size n, std::vector<size>& a, size k, std::vector<size>& e, std::vector<size>& v)
    : n_(n), a_(a), k_(k), e_(e), v_(v), sqrt_val_(316), left_bound_(0), right_bound_(0),
    res_squared_sum_(0), number_of_nodes_(0) {

        for (size i = 0; i < sqrt_val_; ++i) {
            groups_.emplace_back(std::list<size>());
            sizes_.emplace_back(0);
        }

        for (size i = 0; i < n_; ++i) {
            auto group_number = number_of_nodes_ / 1;
            if (group_number > right_bound_) { right_bound_ = group_number; }

            groups_[group_number].push_back(a[i]);
            sizes_[group_number]++;
            res_squared_sum_ += pow(a[i], 2);

            number_of_nodes_++;
        }

        std::cout << "groups_ = " << groups_ << std::endl;
        std::cout << "sizes_ = " << sizes_ << std::endl;
        std::cout << "res_squared_sum = " << res_squared_sum_ << std::endl;
        std::cout << "number_of_nodes_ = " << number_of_nodes_ << std::endl;
        std::cout << "left_bound = " << left_bound_ << std::endl;
        std::cout << "right_bound = " << right_bound_ << std::endl;

    }

    void make_evolution() {

#if MODE == 1
        std::cout << res_squared_sum_ << std::endl;
#endif
//        divide(3);
        bankrupt(2);

        std::cout << "groups_ = " << groups_ << std::endl;
        std::cout << "sizes_ = " << sizes_ << std::endl;
        std::cout << "res_squared_sum = " << res_squared_sum_ << std::endl;
        std::cout << "number_of_nodes_ = " << number_of_nodes_ << std::endl;
        std::cout << "left_bound = " << left_bound_ << std::endl;
        std::cout << "right_bound = " << right_bound_ << std::endl;


//        for (size i = 0; i < k_; ++i) {
//
//            if (e_[i] == 1) { bankrupt(v_[i]); }
//            else if (e_[i] == 2) { divide(v_[i]); }
//
//#if MODE == 1
//            std::cout  << res_squared_sum << std::endl;
//#endif
//
//            if (VERBOSE) {
//                std::cout << "enterprises_ = " << enterprises_ << std::endl;
//                std::cout << "===========================================" << std::endl;
//            }
//        }
//
//        if (VERBOSE) { std::cout << "Enterprises after evolution: " << enterprises_ << std::endl; }

    }


private:

    static std::pair<size, size> calculate_portions(size sum) {
        size first = sum / 2;
        size second = sum - first;
        if (VERBOSE) {
            std::cout << ".....first = " << first << std::endl;
            std::cout << ".....second = " << second << std::endl;
        }

        return {first, second};
    }

    std::pair<size, size> find_position(size j) {
        size i = 0;
        while (j - sizes_[i] > 0) {
            j -= sizes_[i];
            ++i;
        }

        return {i, j - 1};
    }

    void divide(size j) {
        if (VERBOSE) { std::cout << "===> go divide! j = " << j << std::endl; }
        auto position = find_position(j);
        auto group_number = position.first;
        auto index_in_group = position.second;
        std::cout << "group_number = " << group_number << ", index in group = " << index_in_group << std::endl;

        auto it = groups_[group_number].begin();
        std::advance(it, index_in_group);

        auto portions = calculate_portions(*it);

        // add first portion
        groups_[group_number].insert(it, portions.first);
        res_squared_sum_ += std::pow(portions.first, 2);
        if (VERBOSE) { std::cout << "add first portion squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }

        // add second portion
        groups_[group_number].insert(it, portions.second);
        res_squared_sum_ += std::pow(portions.second, 2);
        if (VERBOSE) { std::cout << "add second portion squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }

        // erase initial element
        res_squared_sum_ -= std::pow(*it, 2);
        groups_[group_number].erase(it);
        if (VERBOSE) { std::cout << "remove initial squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }

        // update other
        sizes_[group_number]++;
        number_of_nodes_++;
    }

    void bankrupt(size j) {
        if (VERBOSE) { std::cout << "===> go bankrupt! j = " << j << std::endl; }

        if (j == 1) {
            auto it = groups_[left_bound_].begin();
            res_squared_sum_ -= std::pow(*it, 2);
            if (VERBOSE) { std::cout << "j = 1, remove cur and next squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }
            std::list<size>::iterator next;
            if (sizes_[left_bound_] == 1) {
                size i_gr = left_bound_ + 1;
                while (std::distance(groups_[i_gr].begin(), groups_[i_gr].end()) > 1) { ++i_gr; };
                next = groups_[i_gr].begin();
                sizes_[left_bound_] = 0;
            }
            else {
                next = std::next(it);
                sizes_[left_bound_]--;
            }
            res_squared_sum_ -= std::pow(*next, 2);
            *next += *it;
            res_squared_sum_ += std::pow(*next, 2);
            if (VERBOSE) { std::cout << "j = 1, add next squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }
            groups_[left_bound_].erase(it);
            if (groups_[left_bound_].empty()) { left_bound_++; }
        }
        else if (j == number_of_nodes_) {
//            std::cout << "here!" << std::endl;
            auto it = groups_[right_bound_].end();
            std::cout << "it = " << *it << std::endl;
            res_squared_sum_ -= std::pow(*std::prev(it), 2);
            if (VERBOSE) { std::cout << "j = size, remove it squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }
            std::list<size>::iterator prevprev;
            if (sizes_[right_bound_] == 1) {
                size i_gr = right_bound_ - 1;
                while (std::distance(groups_[i_gr].begin(), groups_[i_gr].end()) > 1 && i_gr > 0) { --i_gr; }
                prevprev = std::prev(groups_[i_gr].end());
                sizes_[right_bound_] = 0;
            }
            else {
                prevprev = std::prev(std::prev(it));
                sizes_[right_bound_]--;
            }
            res_squared_sum_ -= std::pow(*prevprev, 2);
            std::cout << "prevprev = " << *prevprev << std::endl;

            *prevprev += *std::prev(it);

            std::cout << "prevprev upd = " << *prevprev << std::endl;

            res_squared_sum_ += std::pow(*prevprev, 2);
            if (VERBOSE) { std::cout << "j = size, add prev squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }
            groups_[right_bound_].erase(std::prev(it));

            if (groups_[right_bound_].empty()) { right_bound_--; }

        }
        else {
            auto position = find_position(j);
            auto group_number = position.first;
            auto index_in_group = position.second;
            std::cout << "group_number = " << group_number << ", index in group = " << index_in_group << std::endl;

            auto it = groups_[group_number].begin();
            std::advance(it, index_in_group);
            auto portions = calculate_portions(*it);

            std::list<size>::iterator prev, next;
            if (index_in_group == 0) {
                assert (group_number != 0);
                prev = std::prev(groups_[group_number-1].end());
                next = std::next(groups_[group_number].begin());
            }
            else if (index_in_group != sizes_[group_number] - 1)  {
                prev = std::prev(std::prev(groups_[group_number].end()));
                next = groups_[group_number + 1].begin();
            }
            else {
                prev = std::prev(it);
                next = std::next(it);
            }

            // cur
            res_squared_sum_ -= std::pow(*it, 2);
            if (VERBOSE) { std::cout << "remove cur squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }
            // prev
            res_squared_sum_ -= std::pow(*prev, 2);
            if (VERBOSE) { std::cout << "remove prev squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }
            *prev += portions.first;
            res_squared_sum_ += std::pow(*prev, 2);
            if (VERBOSE) { std::cout << "add prev squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }

            // next
            res_squared_sum_ -= std::pow(*next, 2);
            if (VERBOSE) { std::cout << "remove next squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }
            *next += portions.second;
            res_squared_sum_ += std::pow(*next, 2);
            if (VERBOSE) { std::cout << "add next squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }

            groups_[group_number].erase(it);
        }

        number_of_nodes_--;
    }

    const size n_;
    std::vector<size> a_;
    const size k_;
    std::vector<size> e_;
    std::vector<size> v_;

    const size_t sqrt_val_;
    std::vector<std::list<size>> groups_;
    std::vector<size> sizes_;
    size res_squared_sum_;
    size number_of_nodes_;
    size left_bound_;
    size right_bound_;

};


class StressTester {
public:
    StressTester() = default;

    static void test() {
        std::random_device rd;
        std::mt19937 gen(rd());

        const size MIN_N = 50000, MAX_N = 50000;
        std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

        const size MIN_VAL_A = 500, MAX_VAL_A = 1000;
        std::uniform_int_distribution<> dist_VAL_A(MIN_VAL_A, MAX_VAL_A);

        const size MIN_K = 100000, MAX_K = 100000;
        std::uniform_int_distribution<> dist_K(MIN_K, MAX_K);

        const size MIN_E = 2, MAX_E = 2;
        std::uniform_int_distribution<> dist_E(MIN_E, MAX_E);

        const size MIN_V = 45000, MAX_V = 45000;
        std::uniform_int_distribution<> dist_V(MIN_V, MAX_V);

        while (true) {
            std::cout << "======================" << std::endl;

            size n = dist_N(gen);
            std::vector<size> a(n);
            for (size i = 0; i < n; ++i) { a[i] = dist_VAL_A(gen); }

            size k = dist_K(gen);
            std::vector<size> e(k), v(k);
            for (size i = 0; i < k; ++i) {
                e[i] = dist_E(gen);
                v[i] = dist_V(gen);
            }

            std::cout << "n = " << n << std::endl; //<< ", a = " << a << std::endl;
            std::cout << "k = " << k << std::endl; //", ev = " << ev << std::endl;

            tt::steady_clock::time_point begin_greedy = tt::now();
            Greedy(n, a, k, e, v).make_evolution();
            tt::steady_clock::time_point end_greedy = tt::now();
            auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

            std::cout << std::scientific << "T = " << duration_greedy * 1e-9 << " s" << std::endl;

//            if (duration * 1e-9 > 3.0) { throw std::runtime_error("Solution is too slow: " + std::to_string(duration * 1e-9) + " s"); }

            a.clear();
            e.clear();
            v.clear();
        }
    }

private:

};


int main() {

#if MODE == 1
    size n = 0;
    std::cin >> n;

    std::vector<size> a(n);
    for (size i = 0; i < n; ++i) { std::cin >> a[i]; }

    size k = 0;
    std::cin >> k;

    std::vector<size> e(k), v(k);
    for (size i = 0; i < k; ++i) {
        size e_cur = 0, v_cur = 0;
        std::cin >> e_cur;
        std::cin >> v_cur;
        e[i] = e_cur;
        v[i] = v_cur;
    }

    if (VERBOSE) {
        std::cout << "e = " << e << std::endl;
        std::cout << "v = " << v << std::endl;
    }

//    Greedy(n, a, k, e, v).make_evolution();
    Fast(n, a, k, e, v).make_evolution();

#elif MODE == 3
    StressTester::test();

#endif

    return 0;
}