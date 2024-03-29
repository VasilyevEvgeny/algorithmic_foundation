#include <iostream>
#include <string>
#include <deque>
#include <chrono>

// 1 - work, 3 - stress testing
#define MODE 1

#define VERBOSE false

using size = int;
using tt = std::chrono::steady_clock;


std::ostream& operator<< (std::ostream& os, const std::deque<size>& q) {
    auto q_copy = q;
    os << "[ ";
    for (size i = 0; i < q.size(); ++i) {
        auto front = q_copy.front();
        os << front << " ";
        q_copy.pop_front();
    }
    os << "]";

    return os;
}


class Solution {
public:
    Solution(const size n, std::string commands) : n_(n), commands_(std::move(commands)), a_size_(0), b_size_(0) {}

    void solve() {
        std::string answer;

        size i = 1;
        for (size c = 0; c < n_; ++c) {
            char symbol = commands_[c];
            if (VERBOSE) { std::cout << "====================================\nsymbol = " << symbol << std::endl; }

            if (symbol == 'a') {
                a_.push_back(i++);
                a_size_++;
            } else if (symbol == 'b') {
                b_.push_back(i++);
                b_size_++;
            } else if (symbol == 'A') {
//                answer += std::to_string(a_.front() % 10);
                std::cout << a_.front() % 10;
                a_.pop_front();
                a_size_--;
            } else if (symbol == 'B') {
//                answer += std::to_string(b_.front() % 10);
                std::cout << b_.front() % 10;
                b_.pop_front();
                b_size_--;
            } else if (symbol == '>') {
                cashbox_closed(a_, a_size_, b_, b_size_);
                a_size_ = 0;
            } else if (symbol == ']') {
                cashbox_closed(b_, b_size_, a_, a_size_);
                b_size_ = 0;
            } else if (symbol == '<') {
                cashbox_opened(a_, a_size_, b_, b_size_);
            } else if (symbol == '[') {
                cashbox_opened(b_, b_size_, a_, a_size_);
            }

            if (VERBOSE) {
                std::cout << "a = " << a_ << ", a_size_ = " << a_size_ << std::endl;
                std::cout << "b = " << b_ << ", b_size_ = " << b_size_ << std::endl;
                std::cout << "answer = " << answer << std::endl;
            }
        }

//        return answer;
    }

private:

    static void cashbox_closed(std::deque<size>& from, size& from_size, std::deque<size>& to, size& to_size) {
        while (!from.empty()) {
            to.push_back(from.back());
            to_size++;
            from.pop_back();
            from_size--;
        }
    }

    static void cashbox_opened(std::deque<size>& just_opened, size& just_opened_size,
                               std::deque<size>& was_opened, size& was_opened_size) {
        while (just_opened_size + 1 < was_opened_size) {
            just_opened.push_back(was_opened.back());
            just_opened_size++;
            was_opened.pop_back();
            was_opened_size--;
        }
    }

    const size n_;
    const std::string commands_;

    std::deque<size> a_, b_;
    size a_size_, b_size_;
};


class StressTester {
public:
    StressTester() = default;

    static void test() {
        std::string commands;
        size n = 100000;
        for (size i = 0; i < n / 4; ++i) { commands += 'a'; }
        for (size i = 0; i < n / 4; ++i) { commands += 'b'; }
        for (size i = 0; i < n / 4; ++i) {
            char symbol;
            if (!(i % 2)) { symbol = '>'; } else { symbol = '<'; }
            commands += symbol;
        }
        for (size i = 0; i < n / 4; ++i) {
            char symbol;
            if (!(i % 2)) { symbol = ']'; } else { symbol = '['; }
            commands += symbol;
        }

        tt::steady_clock::time_point begin = tt::now();
        Solution(n, commands).solve();
        tt::steady_clock::time_point end = tt::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();

        std::cout << std::scientific << "T = " << duration * 1e-9 << " s" << std::endl;

    }

private:


};



int main() {

#if MODE == 1
    size n = 0;
    std::cin >> n;

    std::cin.ignore();

    std::string commands;
    std::getline(std::cin, commands);

    Solution(n, commands).solve();

#elif MODE == 3
    StressTester::test();

#endif

    return 0;
}