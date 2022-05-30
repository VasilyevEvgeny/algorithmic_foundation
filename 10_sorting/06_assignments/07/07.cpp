#include <iostream>
#include <deque>
#include <algorithm>
#include <string>

#define MODE 1
#define VERBOSE false


using size = uint64_t;


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
    Solution(std::deque<std::string> parts)
            : parts_(std::move(parts)) {

        sort(begin(parts_), end(parts_), [](const std::string& lhs, const std::string& rhs) {
            return lhs + rhs > rhs + lhs;
            /*
            size_t i = 0, j = 0;
            for (; i < lhs.size() && j < rhs.size(); ++i, ++j) {
                if (lhs[i] == rhs[j]) {
                    continue;
                }
                else {
                    return lhs[i] > rhs[j];
                }
            }
            if (i == lhs.size()) {
                while (j < rhs.size()) {
                    if (lhs[i - 1] == rhs[j]) {
                        ++j;
                        continue;
                    } else {
                        return lhs[i - 1] > rhs[j];
                    }
                }
            }
            else {
                while (i < lhs.size()) {
                    if (rhs[j - 1] == lhs[i]) {
                        ++i;
                        continue;
                    } else {
                        return rhs[j - 1] < lhs[i];
                    }
                }
            }
            return true;*/
        });

        for (const auto& e : parts_) {
            std::cout << e;
        }
        std::cout << std::endl;
    }

private:
    std::deque<std::string> parts_;
};


int main() {

    size_t N = 0;
    std::cin >> N;

    std::deque<std::string> parts(N);
    for (size_t i = 0; i < N; ++i) {
        std::cin >> parts[i];
    }

    if (VERBOSE) {
        std::cout << "Initial data:" << std::endl;
        for (const auto& e : parts) {
            std::cout << e << std::endl;
        }
    }

    Solution solution(parts);

    return 0;
}