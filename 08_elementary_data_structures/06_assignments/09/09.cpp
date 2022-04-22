//
// Created by evasilyev on 25.05.2020.
//

#include <iostream>
#include <queue>
#include <vector>

#define VERBOSE false

using size = int64_t;

std::ostream& operator<< (std::ostream& os, const std::queue<size>& q) {
    auto q_copy = q;
    for (size i = 0; i < q.size(); ++i) {
        os << q_copy.front() << " ";
        q_copy.pop();
    }

    return os;
}


class Solution {
public:
    Solution(size n, std::vector<size> first, std::vector<size> second)
    : n_(n), first_(std::move(first)), second_(std::move(second)), max_iterations_(2e5) {
        for (size i = 0; i < n_ / 2; ++i) {
            q_first_.push(first_[i]);
            q_second_.push(second_[i]);
        }
    }

    std::string solve() {
        for (size i = 0; i < max_iterations_; ++i) {
            if (VERBOSE) { std::cout << "==========================" << std::endl; }

            if (VERBOSE) {
                std::cout << "first: " << q_first_ << std::endl;
                std::cout << "second: " << q_second_ << std::endl;
            }

            auto card_first = q_first_.front(); q_first_.pop();
            auto card_second = q_second_.front(); q_second_.pop();

            if (VERBOSE) {
                std::cout << "card_first = " << card_first << std::endl;
                std::cout << "card_second = " << card_second << std::endl;
            }

            if (card_first == 0 && card_second == n_ - 1) { won_first(card_first, card_second); }
            else if (card_first == n_ - 1 && card_second == 0) { won_second(card_first, card_second); }
            else {
                if (card_first < card_second) { won_second(card_first, card_second); }
                else { won_first(card_first, card_second); }
            }

            if (q_first_.empty()) { return "second " + std::to_string(i + 1); }
            else if (q_second_.empty()) { return "first " + std::to_string(i + 1); }
        }

        return "draw";
    }

private:

    void won_first(size card_first, size card_second) {
        q_first_.push(card_first);
        q_first_.push(card_second);

        if (VERBOSE) { std::cout << "won first!" << std::endl; }
    }

    void won_second(size card_first, size card_second) {
        q_second_.push(card_first);
        q_second_.push(card_second);

        if (VERBOSE) { std::cout << "won second!" << std::endl; }
    }

    const size n_;
    std::vector<size> first_, second_;

    std::queue<size> q_first_, q_second_;

    const size max_iterations_;
};


int main() {

    size n = 0;
    std::cin >> n;

    std::vector<size> first(n / 2), second(n / 2);
    for (size i = 0; i < n / 2; ++i) { std::cin >> first[i]; }
    for (size i = 0; i < n / 2; ++i) { std::cin >> second[i]; }

    std::cout << Solution(n, first, second).solve() << std::endl;

    return 0;
}