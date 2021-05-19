//
// Created by evasilyev on 19.05.2020.
//

#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <random>
#include <cassert>
#include <chrono>

// 1 - work, 3 - stress testing
#define MODE 1

#define VERBOSE false

using tt = std::chrono::steady_clock;
using size = int64_t;


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


std::ostream& operator << (std::ostream& os, const std::list<size>& lst) {
    if (lst.empty()) { os << "nullptr"; return os; }

    os << "nullptr -> ";
    size i = 1;
    for (auto& node : lst) {
        os << "#" << i << ": {" << node << "} -> ";
        ++i;
    }
    os << "nullptr";

    return os;
}


std::ostream& operator << (std::ostream& os, const std::vector<std::pair<char, size>>& vv) {
    for (auto& item : vv) { os << item.first << " " << item.second << std::endl; }

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
    Greedy(size N, std::vector<std::pair<char, size>> commands) : N_(N), commands_(std::move(commands)), size_(0) {}

    std::string solve() {
        std::string answer;

        for (auto command : commands_) {
            auto symbol = command.first;
            auto n = command.second;

            if (VERBOSE) {
                std::cout << "=================================" << std::endl;
                std::cout << "symbol = " << symbol << ", n = " << n << std::endl;
            }

            if (symbol == '+') {
                queue_.push_back(n);
                size_++;

                if (VERBOSE) { std::cout << "pushed back!" << std::endl; }
            }
            else if (symbol == '*') {
                auto it = queue_.begin();
                for (size i = 0; i < (size_ + 1) / 2; ++i) { it = std::next(it); }
                queue_.insert(it, n);
                size_++;

                if (VERBOSE) { std::cout << "inserted in the middle!" << std::endl; }
            }
            else {
                answer += std::to_string(queue_.front()) + "\n";
                queue_.pop_front();
                size_--;

                if (VERBOSE) { std::cout << "poped from queue!" << std::endl; }
            }

            if (VERBOSE) {
                std::cout << "queue --> " << queue_ << std::endl;
                std::cout << "size_ --> " << size_ << std::endl;
            }
        }

        return answer;
    }

private:
    const size N_;
    const std::vector<std::pair<char, size>> commands_;

    std::list<size> queue_;
    size size_;
};


class Fast {
public:
    Fast(size N, std::vector<std::pair<char, size>> commands)
    : N_(N), commands_(std::move(commands)), sqrt_val_(317), max_group_size_(2 * sqrt_val_), size_(0) {
        groups_.emplace_back(std::list<size>());
        sizes_.emplace_back(0);
    }

    std::string solve() {
        std::string answer;

        size left = 0, right = 0;
        for (auto& command : commands_) {
            auto symbol = command.first;
            auto n = command.second;

            if (VERBOSE) {
                std::cout << "=================================" << std::endl;
                std::cout << "symbol = " << symbol << ", n = " << n << std::endl;
            }

            if (VERBOSE) { std::cout << "left = " << left << ", right = " << right << std::endl; }

            if (symbol == '+') {
                groups_[right].push_back(n);
                sizes_[right]++;
                size_++;

                if (sizes_[right] == max_group_size_) {
                    add_group(right);
                    if (VERBOSE) { std::cout << "added new group!" << std::endl; }
                }

                if (VERBOSE) { std::cout << "pushed back!" << std::endl; }
            }
            else if (symbol == '*') {
                auto middle = find_group_with_middle(left, right);
                auto middle_group_idx = middle.first;
                auto middle_node_idx = middle.second;

                auto it = groups_[middle_group_idx].begin();
                for (size i = 0; i < middle_node_idx; ++i) { it = std::next(it); }

                groups_[middle_group_idx].insert(it, n);
                sizes_[middle_group_idx]++;
                size_++;

                if (sizes_[middle_group_idx] == max_group_size_) {
                    add_group(middle_group_idx);
                    if (VERBOSE) { std::cout << "added new group!" << std::endl; }
                }

                if (VERBOSE) { std::cout << "inserted in the middle!" << std::endl; }
            }
            else {
                answer += std::to_string(groups_[left].front()) + "\n";
                groups_[left].pop_front();
                sizes_[left]--;
                size_--;

                if (VERBOSE) { std::cout << "poped from queue!" << std::endl; }
            }

            auto bounds = find_bounds();
            left = bounds.first;
            right = bounds.second;

            if (VERBOSE) {
                std::cout << "groups_ --> " << groups_ << std::endl;
                std::cout << "sizes_ --> " << sizes_ << std::endl;
                std::cout << "size_ --> " << size_ << std::endl;
            }
        }

        return answer;
    }


private:

    void add_group(size group_idx) {
        auto copy_it = groups_[group_idx].begin();
        auto del_it = groups_[group_idx].begin();
        size half = max_group_size_ / 2;

        auto it = groups_.begin();
        for (size i = 0; i < group_idx + 1; ++i) { it = std::next(it); }
        groups_.insert(it, std::list<size>());

        for (size i = 0; i < max_group_size_; ++i) {
            if (i < half) { del_it = std::next(del_it); }
            else { groups_[group_idx + 1].push_back(*copy_it); }
            copy_it = std::next(copy_it);
        }

        groups_[group_idx].erase(del_it, groups_[group_idx].end());

        sizes_[group_idx] = half;
        auto it_sizes_ = sizes_.begin() + group_idx;
        sizes_.insert(it_sizes_, half);
    }

    std::pair<size, size> find_bounds() {
        size left = 0, right = 0;
        for (size i = 0; i < groups_.size(); ++i) { if (!groups_[i].empty()) { left = i; break; } }
        for (size i = groups_.size() - 1; i >= 0; --i) { if (!groups_[i].empty()) { right = i; break; } }

        return {left, right};
    }

    std::pair<size, size> find_group_with_middle(size left, size right) {
        size middle = (size_ + 1) / 2;
        size sum = 0;
        for (size i = left; i <= right; ++i) {
            sum += sizes_[i];
            if (sum >= middle) { return {i, middle - (sum - sizes_[i])}; }
        }

        return {-1, -1};
    }

    const size N_;
    const std::vector<std::pair<char, size>> commands_;

    std::vector<std::list<size>> groups_;
    std::vector<size> sizes_;
    size size_;
    const size sqrt_val_;
    const size max_group_size_;

};


class StressTester {
public:
    StressTester() = default;

    static void test() {

        std::random_device rd;
        std::mt19937 gen(rd());

        const size MIN_N = 1, MAX_N = 10000;
        std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);

        const size MIN_SYMBOL = 1, MAX_SYMBOL = 3;
        std::uniform_int_distribution<> dist_SYMBOL(MIN_SYMBOL, MAX_SYMBOL);

        while (true) {
            std::cout << "=======================================" << std::endl;

            size pos = 1;
            size N = dist_N(gen);
            std::vector<std::pair<char, size>> commands;
            for (size i = 0; i < N; ++i) {

                // symbol
                char symbol = 0;
                auto symbol_type = dist_SYMBOL(gen);

                if (symbol_type == 1) { symbol = '+'; }
                else if (symbol_type == 2) { symbol = '*'; }
                else { symbol = '-'; }

                if (i <= N / 2 && symbol_type == 3) { symbol = '*'; symbol_type = 2; }

                // pos
                size cur_pos;
                if (symbol_type != 3) { cur_pos = pos++; }
                else { cur_pos = -1; }

                commands.emplace_back(symbol, cur_pos);
            }

            std::cout << "N = " << N << std::endl;
//            std::cout << "commands:\n" << commands << std::endl;

            tt::steady_clock::time_point begin_greedy = tt::now();
            auto res_greedy = Greedy(N, commands).solve();
            tt::steady_clock::time_point end_greedy = tt::now();
            auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

//            std::cout << res_greedy << std::endl;
            std::cout << std::scientific << "T_greedy = " << duration_greedy * 1e-9 << " s" << std::endl;

            std::cout << "---------------" << std::endl;

            tt::steady_clock::time_point begin_fast = tt::now();
            auto res_fast = Fast(N, commands).solve();
            tt::steady_clock::time_point end_fast = tt::now();
            auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();

//            std::cout << res_greedy << std::endl;
            std::cout << std::scientific << "T_fast = " << duration_fast * 1e-9 << " s" << std::endl;

            assert(res_greedy == res_fast);
            assert(duration_fast * 1e-9 < 2.0);
        }
    }

private:

};


int main () {

#if MODE == 1

    size N = 0;
    std::cin >> N;
    std::cin.ignore();

    std::vector<std::pair<char, size>> commands;
    for (size i = 0; i < N; ++i) {
        char symbol = 0; size n = -1;
        std::string symbol_str, n_str;
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        iss >> symbol;
        if (symbol != '-') {
            iss >> n_str;
            n = std::stoi(n_str);
        }
        commands.emplace_back(symbol, n);
    }

//    std::cout << Greedy(N, commands).solve();
    std::cout << Fast(N, commands).solve();

#elif MODE == 3
    StressTester::test();

#endif

    return 0;
}