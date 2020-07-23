//
// Created by evasilyev on 23.07.2020.
//

#include <iostream>
#include <vector>

using integer = int64_t;

std::ostream& operator<< (std::ostream& os, const std::vector<integer>& v) {
    for (const auto& e : v) { std::cout << e << " "; }

    return os;
}


class Solution {
public:
    Solution(const integer N, std::vector<integer> input) : N_(N), input_(std::move(input)) {}

    std::vector<integer> solve() {
        std::vector<integer> res(input_.begin(), input_.end());
        for (integer i = 0; i < N_; ++i) {
            integer max_val = res[i];
            integer max_idx = i;
            for (integer j = i; j < N_; ++j) {
                if (res[j] > max_val) {
                    max_val = res[j];
                    max_idx = j;
                }
            }
            std::swap(res[i], res[max_idx]);
        }

        return res;
    }

private:
    const integer N_;
    const std::vector<integer> input_;

};


int main() {

    integer N = 0;
    std::cin >> N;

    std::vector<integer> input(N);
    for (size_t i = 0; i < N; ++i) { std::cin >> input[i]; }

    std::cout << Solution(N, input).solve() << std::endl;

    return 0;
}