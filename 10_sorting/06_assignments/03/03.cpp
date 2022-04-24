#include <iostream>
#include <vector>

#define MODE 1
#define VERBOSE false


using size = size_t;


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

class Solution {
public:
    Solution(std::vector<size> skates, std::vector<size> students)
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

    void sort(std::vector<size>& arr) {
        if (VERBOSE) {
            std::cout << "In sort()..." << std::endl;
        }

        std::vector<size> cnt(MAX_SIZE_);
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
        for (size i = 0, j = 0; i < students_.size(), j < skates_.size(); ++i, ++j) {

            if (VERBOSE) {
                std::cout << "i = " << i << ", j = " << j << std::endl;
            }

            while (students_[i] > skates_[j] && j < skates_.size()) {
                ++j;

                if (VERBOSE) {
                    std::cout << "j = " << j << std::endl;
                }
            }

            if (j < skates_.size()) {
                ++n;

                if (VERBOSE) {
                    std::cout << "n = " << n << std::endl;
                }
            }
            else {

                if (VERBOSE) {
                    std::cout << "break..." << std::endl;
                }

                break;
            }
        }

        return n;
    }

    std::vector<size> skates_;
    std::vector<size> students_;
};


int main() {

    size N = 0;
    std::cin >> N;
    std::vector<size> skates(N);
    for (size i = 0; i < N; ++i) {
        std::cin >> skates[i];
    }

    size M = 0;
    std::cin >> M;
    std::vector<size> students(M);
    for (size i = 0; i < M; ++i) {
        std::cin >> students[i];
    }

    Solution solution(skates, students);

    return 0;
}