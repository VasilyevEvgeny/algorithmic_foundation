//
// Created by evasilyev on 30.05.2020.
//

#include <iostream>
#include <vector>
#include <stack>
#include <map>

#define VERBOSE false

using size = int64_t;


class AdvancedStack {
public:
    AdvancedStack() : size_(0) {}

    void push(size val) {
        if (st_.empty()) { st_.push({{"val", val}, {"min", val}, {"max", val}}); }
        else {
            auto min_element = std::min(val, st_.top().at("min"));
            auto max_element = std::max(val, st_.top().at("max"));
            st_.push({{"val", val}, {"min", min_element}, {"max", max_element}});
        }
        size_++;
    }

    void pop() {
        st_.pop();
        size_--;
    }

    size min() {
        return st_.top().at("min");
    }

    size max() {
        return st_.top().at("max");
    }

    bool empty() {
        return st_.empty();
    }

    std::map<std::string, size> top() {
        return st_.top();
    }

    size len() const {
        return size_;
    }

private:
    std::stack<std::map<std::string, size>> st_;
    size size_;

};


std::ostream& operator<< (std::ostream& os, const AdvancedStack& st) {
    auto st_copy = st;
    os << "[ ";
    while (!st_copy.empty()) {
        auto top = st_copy.top();
        os << "{" << top.at("val") << " -> " << top.at("min") << ", " << top.at("max") << "} ";
        st_copy.pop();
    }
    os << "]";

    return os;
}


class AdvancedQueue {
public:
    AdvancedQueue() = default;

    void push(size val) {
        st1_.push(val);
    }

    void pop() {
        if (st2_.empty()) {
            while (!st1_.empty()) {
                st2_.push(st1_.top().at("val"));
                st1_.pop();
            }
        }
        st2_.pop();
    }

    size min() {
        if (st1_.empty() and !st2_.empty()) {
            return st2_.min();
        }
        else if (!st1_.empty() and st2_.empty()) {
            return st1_.min();
        }
        else {
            return std::min(st1_.min(), st2_.min());
        }
    }

    size max() {
        if (st1_.empty() and !st2_.empty()) {
            return st2_.max();
        }
        else if (!st1_.empty() and st2_.empty()) {
            return st1_.max();
        }
        else {
            return std::max(st1_.max(), st2_.max());
        }
    }

    size len() { return st1_.len() + st2_.len(); }

    AdvancedStack st1() const { return st1_; }
    AdvancedStack st2() const { return st2_; }

private:
    AdvancedStack st1_;
    AdvancedStack st2_;
};


std::ostream& operator<< (std::ostream& os, const AdvancedQueue& q) {
    os << "( st1 = " << q.st1() << " :: st2 = " << q.st2() << " )";

    return os;
}


class Solution {
public:
    Solution(size N, std::vector<size> measurements, size M, std::vector<size> variability)
    : N_(N), measurements_(std::move(measurements)), M_(M), variability_(std::move(variability)) {}

    std::string solve () {
        std::string answer;

        for (auto& k : variability_) {
            AdvancedQueue q;

            q.push(measurements_[0]);
            size idx_left = 1, idx_right = 1;
            size idx_left_max = 1, idx_right_max = 1, delta_max = 0;

            if (VERBOSE) { std::cout << "q = " << q << ", idx_left = " << idx_left << ", idx_right = "
            << idx_right << ", delta_max = " << delta_max << std::endl; }

            for (size i = 1; i < N_; ++i) {
                q.push(measurements_[i]);
                idx_right++;

                if (VERBOSE) { std::cout << "pushed " << measurements_[i] << "! idx_right --> " << idx_right << std::endl; }

                auto delta_cur = q.max() - q.min();
                if (VERBOSE) { std::cout << "delta_cur = " << delta_cur << std::endl; }

                if (delta_cur <= k && delta_cur > delta_max) {
                    delta_max = idx_right - idx_left;
                    idx_left_max = idx_left;
                    idx_right_max = idx_right;
                    if (VERBOSE) {
                        std::cout << "idx_left_max = " << idx_left_max << ", idx_right_max --> " << idx_right_max
                        << ", delta_max --> " << delta_max << std::endl; }
                }
                else {
                    while (q.max() - q.min() > k && q.len() > 1) {
                        q.pop();
                        idx_left++;
                        if (VERBOSE) { std::cout << ".....q --> " << q << ", idx_left --> " << idx_left << std::endl; }
                    }
                }

                if (VERBOSE) { std::cout << "q = " << q << ", idx_left = " << idx_left << ", idx_right = "
                                         << idx_right << ", delta_max = " << delta_max << std::endl; }
            }
            answer += std::to_string(idx_left_max) + " " + std::to_string(idx_right_max) + "\n";
        }


        return answer;
    }

private:
    const size N_;
    const std::vector<size> measurements_;
    const size M_;
    const std::vector<size> variability_;
};


int main() {

    size N = 0;
    std::cin >> N;

    std::vector<size> measurements(N);
    for (size i = 0; i < N; ++i) { std::cin >> measurements[i]; }

    size M = 0;
    std::cin >> M;

    std::vector<size> variability(M);
    for (size i = 0; i < M; ++i) { std::cin >> variability[i]; }

    std::cout << Solution(N, measurements, M, variability).solve();

    return 0;
}