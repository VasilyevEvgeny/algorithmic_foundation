#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <map>

#define VERBOSE true

using size = int64_t;


std::ostream& operator<< (std::ostream& os, const std::stack<std::pair<size, size>>& st) {
    auto st_copy = st;
    os << "[ ";
    for (size i = 0; i < st.size(); ++i) {
        auto top = st_copy.top();
        os << "{" << top.first << ", " << top.second << "} ";
        st_copy.pop();
    }
    os << "]";

    return os;
}


class Greedy {
public:
    Greedy(size N, std::vector<size> numbers, size K, std::vector<size> rounds)
    : N_(N), numbers_(std::move(numbers)), K_(K), rounds_(std::move(rounds)) {}

    size solve() {
        size sum = 0;

        for (size i = 0; i < K_; ++i) {
            size a = rounds_[i];
            size max = -1e8;
            for (size j = 0; j < N_ - a + 1; ++j) {
                auto cur_min = *std::min_element(numbers_.begin() + j, numbers_.begin() + j + a);
                max = std::max(max, cur_min);

                if (VERBOSE) { std::cout << "[ " << numbers_[j] << "..." << numbers_[j + a - 1] << "], cur_min = " << cur_min << ", max --> " << max << std::endl; }
            }
            sum += max;
            if (VERBOSE) { std::cout << "sum --> " << sum << std::endl; }
        }

        return sum;
    }

private:
    const size N_;
    const std::vector<size> numbers_;

    const size K_;
    const std::vector<size> rounds_;
};


class StackWithMin {
public:
    StackWithMin() = default;

    void push(size val) {
        if (st_.empty()) { st_.push({val, val}); }
        else {
            auto min_element = std::min(val, st_.top().second);
            st_.push({val, min_element});
        }
    }

    void pop() {
        st_.pop();
    }

    size min() {
        return st_.top().second;
    }

    bool empty() {
        return st_.empty();
    }

    std::pair<size, size> top() {
        return st_.top();
    }

private:
    std::stack<std::pair<size, size>> st_;

};


std::ostream& operator<< (std::ostream& os, const StackWithMin& st) {
    auto st_copy = st;
    os << "[ ";
    while (!st_copy.empty()) {
        auto top = st_copy.top();
        os << "{" << top.first << ", " << top.second << "} ";
        st_copy.pop();
    }
    os << "]";

    return os;
}


class QueueWithMin {
public:
    QueueWithMin() = default;

    void push(size val) {
        st1_.push(val);
    }

    void pop() {
        if (st2_.empty()) {
            while (!st1_.empty()) {
                st2_.push(st1_.top().first);
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

    size front() { return st2_.top().first; }
    size back() { return st1_.top().first; }

    StackWithMin st1() const { return st1_; }
    StackWithMin st2() const { return st2_; }

private:
    StackWithMin st1_;
    StackWithMin st2_;
};


std::ostream& operator<< (std::ostream& os, const QueueWithMin& q) {
    os << "( st1 = " << q.st1() << " :: st2 = " << q.st2() << " )";

    return os;
}


class Fast {
public:
    Fast(size N, std::vector<size> numbers, size K, std::vector<size> rounds)
            : N_(N), numbers_(std::move(numbers)), K_(K), rounds_(std::move(rounds)) {}

    size solve() {
        size sum = 0;

        for (size i = 0; i < K_; ++i) {
            size a = rounds_[i];
            QueueWithMin q;

            for (size j = 0; j < a; ++j) { q.push(numbers_[j]); }

            size max = q.min();

            if (VERBOSE) {
                std::cout << "first push: " << q << std::endl;
                std::cout << "max = " << q.min() << std::endl;
            }

            for (size j = a; j < N_; ++j) {
                q.push(numbers_[j]);
                q.pop();
                max = std::max(max, q.min());

                if (VERBOSE) {
                    std::cout << q << ", max = " << q.min() << std::endl;
                }
            }

            sum += max;
        }

        return sum;
    }

private:
    const size N_;
    const std::vector<size> numbers_;

    const size K_;
    const std::vector<size> rounds_;
};



int main() {

    size N = 0, K = 0;
    std::cin >> N >> K;

    std::vector<size> numbers(N);
    for (size i = 0; i < N; ++i) { std::cin >> numbers[i]; }

    std::vector<size> rounds(K);
    for (size i = 0; i < K; ++i) { std::cin >> rounds[i]; }

//    std::cout << Greedy(N, numbers, K, rounds).solve() << std::endl;
    std::cout << Fast(N, numbers, K, rounds).solve() << std::endl;

    return 0;
}