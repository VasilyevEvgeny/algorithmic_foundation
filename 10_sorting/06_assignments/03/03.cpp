#include <iostream>
#include <deque>

#define MODE 1
#define VERBOSE false


using size = size_t;


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

class Solution {
public:
    Solution(std::deque<size> skates, std::deque<size> students)
    : skates_(std::move(skates))
    , students_(std::move(students)) {

        if (VERBOSE) {
            std::cout << skates_ << std::endl;
            std::cout << students_ << std::endl;
        }

        sort(skates_);
        sort(students_);

        if (VERBOSE) {
            std::cout << "Skates_: " << skates_ << std::endl;
            std::cout << "Students_: " << students_ << std::endl;
        }

        std::cout << solve() << std::endl;
    }

private:
    const size MAX_SIZE_ = 100;

    void sort(std::deque<size>& arr) {
        if (VERBOSE) {
            std::cout << "In sort()..." << std::endl;
        }

        std::deque<size> cnt(MAX_SIZE_);
        for (const auto& e : arr) {
            ++cnt[e - 1];
        }

        if (VERBOSE) {
            std::cout << "cnt: \n" << cnt << std::endl;
        }

        for (size i = 0, j = 0; i < arr.size(); ++j) {
            if (cnt[j]) {
                while (cnt[j]) {
                    arr[i++] = j + 1;
                    --cnt[j];
                }
            }
        }
    }

    size solve() {
        size n = 0;
        while (!students_.empty()) {
            while (!skates_.empty() && skates_[0] < students_[0]) {
                skates_.pop_front();
            }
            if (!skates_.empty()) {
                students_.pop_front();
                skates_.pop_front();
                n += 1;
            }
            else {
                break;
            }
        }

        return n;
    }

    std::deque<size> skates_;
    std::deque<size> students_;
};


int main() {

    size N = 0;
    std::cin >> N;
    std::deque<size> skates(N);
    for (size i = 0; i < N; ++i) {
        std::cin >> skates[i];
    }

    size M = 0;
    std::cin >> M;
    std::deque<size> students(M);
    for (size i = 0; i < M; ++i) {
        std::cin >> students[i];
    }

    Solution solution(skates, students);

    return 0;
}