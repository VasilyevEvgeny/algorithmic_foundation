//
// Created by evasilyev on 03.05.2020.
//

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <cmath>

#define VERBOSE false

using size = int64_t;


template <typename T>
std::ostream& operator << (std::ostream& os, const std::list<T>& lst) {
    if (lst.empty()) { os << "nullptr" << std::endl; }

    os << "nullptr -> ";
    size i = 1;
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        os << "#" << i << ": " << *it << " -> ";
        ++i;
    }
    os << "nullptr";

    return os;
}


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


template <typename T, typename U>
std::ostream& operator << (std::ostream& os, const std::map<T, U>& m) {
    os << "{";
    for (auto& e : m) {
        os << "{" << e.first << " : " << e.second << "}";
    }
    os << "}";
    return os;
}



class Enterprises {
public:
    Enterprises(size n, std::vector<size>& a, size k, std::vector<std::map<std::string, size>>& ev)
    : n_(n), a_(a), k_(k), ev_(ev) {
        for (auto& e : a) {
            enterprises_.push_back(e);
            sum_of_squares += std::pow(e, 2);
        }

        if (VERBOSE) { std::cout << "Enterprises initial: " << enterprises_ << std::endl; }
    }

    void make_evolution() {
        std::cout << sum_of_squares << std::endl;

        for (auto& item : ev_) {
            auto e = item.at("e");
            auto v = item.at("v");

            if (e == 1) { bankrupt(v); }
            else if (e == 2) { divide(v); }

            std::cout  << sum_of_squares << std::endl;

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
//            auto it = enterprises_.begin();
            auto it = enterprises_.begin() + sizeof(std::list<size>) * j;
//            std::advance(it, j - 1);

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

//    std::list<size>::iterator find_binary(size j) {
//        auto l_it = enterprises_.begin();
//        auto r_it = enterprises_.end();
//        while (l_it != r_it) {
//            auto
//        }
//    }

    const size n_;
    std::vector<size> a_;
    const size k_;
    std::vector<std::map<std::string, size>> ev_;

    std::list<size> enterprises_;
    size sum_of_squares = 0;
};


int main() {

    size n = 0;
    std::cin >> n;

    std::vector<size> a(n, 0);
    for (size i = 0; i < n; ++i) { std::cin >> a[i]; }

    size k = 0;
    std::cin >> k;

    std::vector<std::map<std::string, size>> ev;
    for (size i = 0; i < k; ++i) {
        std::map<std::string, size> cur;
        std::cin >> cur["e"];
        std::cin >> cur["v"];
        ev.push_back(cur);
    }

    if (VERBOSE) { std::cout << "ev = " << ev << std::endl; }

    Enterprises enterprises(n, a, k, ev);
    enterprises.make_evolution();

    return 0;
}