#include <iostream>
#include <vector>

using size = int64_t;


class Solution {
public:
    Solution(const size N, std::vector<std::vector<size>> matrix) : N_(N), matrix_(std::move(matrix)) {}

    void solve() {
        std::vector<size> sources, sinks;

        // find sources
        for (size col = 0; col < N_; ++col) {
            size sum = 0;
            for (size row = 0; row < N_; ++row) { sum += matrix_[row][col]; }
            if (!sum) { sources.push_back(col + 1); }
        }

        print_answer(sources);

        // find sinks
        for (size row = 0; row < N_; ++row) {
            size sum = 0;
            for (size col = 0; col < N_; ++col) { sum += matrix_[row][col]; }
            if (!sum) { sinks.push_back(row + 1); }
        }

        print_answer(sinks);
    }

private:

    static void print_answer(std::vector<size>& v) {
        std::cout << v.size() << " ";
        for (auto& e : v) { std::cout << e << " "; }
        std::cout << std::endl;
    }

    const size N_;
    const std::vector<std::vector<size>> matrix_;
};


int main() {

    size N = 0;
    std::cin >> N;

    std::vector<std::vector<size>> matrix(N, std::vector<size>(N, 0));
    for (size i = 0; i < N; ++i) {
        for (size j = 0; j < N; ++j) {
            std::cin >> matrix[i][j];
        }
    }

    Solution(N, matrix).solve();

    return 0;
}