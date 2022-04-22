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
#define MODE 3

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
    : n_(n), a_(a), k_(k), e_(e), v_(v), sum_of_squares_(0) {
        for (size i = 0; i < n_; ++i) {
            enterprises_.push_back(a_[i]);
            sum_of_squares_ += std::pow(a_[i], 2);
        }

        if (VERBOSE) { std::cout << "Enterprises initial: " << enterprises_ << std::endl; }
    }

    std::string make_evolution() {
        std::string output;

        output += std::to_string(sum_of_squares_);

        for (size i = 0; i < k_; ++i) {

            if (e_[i] == 1) { bankrupt(v_[i]); }
            else if (e_[i] == 2) { divide(v_[i]); }

            output += "\n" + std::to_string(sum_of_squares_);

            if (VERBOSE) {
                std::cout << "enterprises_ = " << enterprises_ << std::endl;
                std::cout << "===========================================" << std::endl;
            }
        }

        if (VERBOSE) { std::cout << "Enterprises after evolution: " << enterprises_ << std::endl; }

        return output;
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
        sum_of_squares_ += std::pow(portions.first, 2);
        if (VERBOSE) { std::cout << "add first portion squared, sum_of_squares --> " << sum_of_squares_ << std::endl; }

        // add second portion
        enterprises_.insert(it, portions.second);
        sum_of_squares_ += std::pow(portions.second, 2);
        if (VERBOSE) { std::cout << "add second portion squared, sum_of_squares --> " << sum_of_squares_ << std::endl; }

        // erase initial element
        sum_of_squares_ -= std::pow(*it, 2);
        enterprises_.erase(it);
        if (VERBOSE) { std::cout << "remove initial squared, sum_of_squares --> " << sum_of_squares_ << std::endl; }
    }

    void bankrupt(size j) {
        if (VERBOSE) { std::cout << "===> go bankrupt! j = " << j << std::endl; }
        if (j == 1) {
            auto it = enterprises_.begin();
            sum_of_squares_ -= std::pow(*it, 2);
            sum_of_squares_ -= std::pow(*std::next(it), 2);
            if (VERBOSE) { std::cout << "j = 1, remove cur and next squared, sum_of_squares --> " << sum_of_squares_ << std::endl; }
            *std::next(it) += *it;
            sum_of_squares_ += std::pow(*std::next(it), 2);
            if (VERBOSE) { std::cout << "j = 1, add next squared, sum_of_squares --> " << sum_of_squares_ << std::endl; }
            enterprises_.erase(it);
        }
        else if (j == enterprises_.size()) {
            auto it = enterprises_.end();
            sum_of_squares_ -= std::pow(*std::prev(std::prev(it)), 2);
            sum_of_squares_ -= std::pow(*std::prev(it), 2);
            if (VERBOSE) { std::cout << "j = size, remove prev-prev and prev squared, sum_of_squares --> " << sum_of_squares_ << std::endl; }
            *std::prev(std::prev(it)) += *std::prev(it);
            sum_of_squares_ += std::pow(*std::prev(std::prev(it)), 2);
            if (VERBOSE) { std::cout << "j = size, add prev squared, sum_of_squares --> " << sum_of_squares_ << std::endl; }
            enterprises_.erase(std::prev(it));
        }
        else {
            auto it = enterprises_.begin();
            std::advance(it, j - 1);

            auto portions = calculate_portions(*it);

            //cur
            sum_of_squares_ -= std::pow(*it, 2);
            if (VERBOSE) { std::cout << "remove cur squared, sum_of_squares --> " << sum_of_squares_ << std::endl; }
            // prev
            sum_of_squares_ -= std::pow(*std::prev(it), 2);
            if (VERBOSE) { std::cout << "remove prev squared, sum_of_squares --> " << sum_of_squares_ << std::endl; }
            *std::prev(it) += portions.first;
            sum_of_squares_ += std::pow(*std::prev(it), 2);
            if (VERBOSE) { std::cout << "add prev squared, sum_of_squares --> " << sum_of_squares_ << std::endl; }

            // next
            sum_of_squares_ -= std::pow(*std::next(it), 2);
            if (VERBOSE) { std::cout << "remove next squared, sum_of_squares --> " << sum_of_squares_ << std::endl; }
            *std::next(it) += portions.second;
            sum_of_squares_ += std::pow(*std::next(it), 2);
            if (VERBOSE) { std::cout << "add next squared, sum_of_squares --> " << sum_of_squares_ << std::endl; }

            enterprises_.erase(it);
        }
    }

    const size n_;
    std::vector<size> a_;
    const size k_;
    std::vector<size> e_;
    std::vector<size> v_;

    std::list<size> enterprises_;
    size sum_of_squares_;

};


class Fast {
public:
    Fast(size n, std::vector<size>& a, size k, std::vector<size>& e, std::vector<size>& v)
    : n_(n), a_(a), k_(k), e_(e), v_(v), sqrt_val_(317), max_group_size_(2 * sqrt_val_), left_bound_(0), right_bound_(0),
    res_squared_sum_(0), number_of_nodes_(0) {

        for (size i = 0; i < sqrt_val_; ++i) {
            groups_.emplace_back(std::list<size>());
            sizes_.emplace_back(0);
        }

        for (size i = 0; i < n_; ++i) {
            auto group_number = number_of_nodes_ / 317;
            if (group_number > right_bound_) { right_bound_ = group_number; }

            groups_[group_number].push_back(a[i]);
            sizes_[group_number]++;
            res_squared_sum_ += pow(a[i], 2);

            number_of_nodes_++;
        }

        if (VERBOSE) {
            std::cout << "groups_ = " << groups_ << std::endl;
            std::cout << "sizes_ = " << sizes_ << std::endl;
            std::cout << "res_squared_sum = " << res_squared_sum_ << std::endl;
            std::cout << "number_of_nodes_ = " << number_of_nodes_ << std::endl;
            std::cout << "left_bound = " << left_bound_ << std::endl;
            std::cout << "right_bound = " << right_bound_ << std::endl;
        }
    }

    std::string make_evolution() {
        std::string output;

        output += std::to_string(res_squared_sum_);

        for (size i = 0; i < k_; ++i) {

            if (e_[i] == 1) { bankrupt(v_[i]); }
            else if (e_[i] == 2) { divide(v_[i]); }

            left_bound_ = find_first_nonempty_group(0, "forward");
            right_bound_ = find_first_nonempty_group(groups_.size()-1, "backward");

            output += "\n" + std::to_string(res_squared_sum_);

            if (VERBOSE) {
                std::cout << "groups_ = " << groups_ << std::endl;
                std::cout << "sizes_ = " << sizes_ << std::endl;
                std::cout << "res_squared_sum = " << res_squared_sum_ << std::endl;
                std::cout << "number_of_nodes_ = " << number_of_nodes_ << std::endl;
                std::cout << "left_bound = " << left_bound_ << std::endl;
                std::cout << "right_bound = " << right_bound_ << std::endl;
            }
        }

        return output;
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

        if (sizes_[group_number] == max_group_size_) {
            auto half = max_group_size_ / 2;

            // insert new
            groups_.insert(groups_.begin() + group_number + 1, std::list<size>());
            sizes_.insert(sizes_.begin() + group_number + 1, half);

            // copy remainder
            auto copy_it = groups_[group_number].begin();
            auto del_it = groups_[group_number].begin();
            for (size i = 0; i < max_group_size_; ++i) {
                if (i < half) { del_it = std::next(del_it); }
                else { groups_[group_number + 1].push_back(*copy_it); }
                copy_it = std::next(copy_it);
            }

            // delete remainder
            groups_[group_number].erase(del_it, groups_[group_number].end());
            sizes_[group_number] = half;
        }
    }

    size find_first_nonempty_group(size start_idx, const std::string& direction) {
        if (direction == "forward") {
            for (size i = start_idx; i < sqrt_val_; ++i) {
                if (std::distance(groups_[i].begin(), groups_[i].end()) > 1) {
                    return i;
                }
            }
            return -1;
        }
        else if (direction == "backward") {
            for (size i = start_idx; i >= 0; --i) {
                if (std::distance(groups_[i].begin(), groups_[i].end()) > 1) {
                    return i;
                }
            }
            return -1;
        }
        else {
            throw std::runtime_error("Wrong direction!");
        }
    }

    bool is_group_unit(size group_number) {
        return std::distance(groups_[group_number].begin(), groups_[group_number].end()) == 2;
    }

    void bankrupt(size j) {
        if (VERBOSE) { std::cout << "===> go bankrupt! j = " << j << std::endl; }

        if (j == 1) {
            auto it = groups_[left_bound_].begin();
            res_squared_sum_ -= std::pow(*it, 2);
            if (VERBOSE) { std::cout << "j = 1, remove cur and next squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }
            std::list<size>::iterator next;
            if (sizes_[left_bound_] == 1) {
                size target = find_first_nonempty_group(left_bound_ + 1, "forward");
                next = groups_[target].begin();
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
            auto it = groups_[right_bound_].end();
            res_squared_sum_ -= std::pow(*std::prev(it), 2);
            if (VERBOSE) { std::cout << "j = size, remove it squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }
            std::list<size>::iterator prevprev;
            if (sizes_[right_bound_] == 1) {
                size target = find_first_nonempty_group(right_bound_ - 1, "backward");
                prevprev = std::prev(groups_[target].end());
                sizes_[right_bound_] = 0;
            }
            else {
                prevprev = std::prev(std::prev(it));
                sizes_[right_bound_]--;
            }
            res_squared_sum_ -= std::pow(*prevprev, 2);
            *prevprev += *std::prev(it);
            res_squared_sum_ += std::pow(*prevprev, 2);
            if (VERBOSE) { std::cout << "j = size, add prev squared, sum_of_squares --> " << res_squared_sum_ << std::endl; }
            groups_[right_bound_].erase(std::prev(it));
            if (groups_[right_bound_].empty()) { right_bound_--; }
        }
        else {
            auto position = find_position(j);
            auto group_number = position.first;
            auto index_in_group = position.second;

            auto it = groups_[group_number].begin();
            std::advance(it, index_in_group);
            auto portions = calculate_portions(*it);

            std::list<size>::iterator prev, next;
            if (index_in_group == 0) {
                assert (group_number != 0);
                auto prev_idx = find_first_nonempty_group(group_number - 1, "backward");
                prev = std::prev(groups_[prev_idx].end());
                if (is_group_unit(group_number)) {
                    auto next_idx = find_first_nonempty_group(group_number, "forward");
                    next = groups_[next_idx].begin();
                }
                else {
                    next = std::next(groups_[group_number].begin());
                }
            }
            else if (index_in_group == sizes_[group_number] - 1)  {
                if (is_group_unit(group_number)) {
                    auto prev_idx = find_first_nonempty_group(group_number, "backward");
                    prev = std::prev(groups_[prev_idx].end());
                }
                else {
                    prev = std::prev(std::prev(groups_[group_number].end()));
                }
                auto next_idx = find_first_nonempty_group(group_number + 1, "forward");
                next = groups_[next_idx].begin();
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
            sizes_[group_number]--;
        }

        number_of_nodes_--;
    }

    const size n_;
    std::vector<size> a_;
    const size k_;
    std::vector<size> e_;
    std::vector<size> v_;

    const size_t sqrt_val_;
    const size_t max_group_size_;
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

        const size MIN_N = 100000, MAX_N = 100000;
        std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

        const size MIN_VAL_A = 1, MAX_VAL_A = 10000;
        std::uniform_int_distribution<> dist_VAL_A(MIN_VAL_A, MAX_VAL_A);

        const size MIN_K = 5000, MAX_K = 5000;
        std::uniform_int_distribution<> dist_K(MIN_K, MAX_K);

        const size MIN_E = 1, MAX_E = 2;
        std::uniform_int_distribution<> dist_E(MIN_E, MAX_E);

        size MIN_V = 50000, MAX_V = 50000;
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

            std::cout << "n = " << n << std::endl;
            std::cout << "k = " << k << std::endl;

            tt::steady_clock::time_point begin_greedy = tt::now();
            auto res_greedy = Greedy(n, a, k, e, v).make_evolution();
            tt::steady_clock::time_point end_greedy = tt::now();
            auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

            std::cout << std::scientific << "T_greedy = " << duration_greedy * 1e-9 << " s" << std::endl;

            tt::steady_clock::time_point begin_fast = tt::now();
            auto res_fast = Fast(n, a, k, e, v).make_evolution();
            tt::steady_clock::time_point end_fast = tt::now();
            auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();

            std::cout << std::scientific << "T_fast = " << duration_fast * 1e-9 << " s" << std::endl;

            assert(res_greedy == res_fast);

//            if (duration_fast * 1e-9 > 1.0) { throw std::runtime_error("Solution is too slow: " + std::to_string(duration_fast * 1e-9) + " s"); }


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

//    std::cout << Greedy(n, a, k, e, v).make_evolution() << std::endl;
    std::cout << Fast(n, a, k, e, v).make_evolution() << std::endl;

#elif MODE == 3
    StressTester::test();

#endif

    return 0;
}