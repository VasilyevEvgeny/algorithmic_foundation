//
// Created by evasilyev on 03.06.2020.
//

#include <iostream>
#include <vector>


using size = int64_t;


std::ostream& operator<< (std::ostream& os, const std::vector<std::vector<size>>& v) {
    for (size i_row = 0; i_row < v.size(); ++i_row) {
        for (const auto& e : v[i_row]) {
            os << e << " ";
        }
        if (i_row != v.size() - 1) { std::cout << std::endl; }
    }

    return os;
}


class Solution {
public:
    Solution(const size N, std::vector<std::vector<size>> matrix)
    : N_(N), matrix_(std::move(matrix)), is_adjacency_(true) {

        bool stop = false;
        for (size i = 0; i < N_; ++i) {
            for (size j = i; j < N_; ++j) {
                if (i == j) { if (matrix_[i][i] == 1) { is_adjacency_ = false; stop = true; break; } }
                else { if (matrix_[i][j] != matrix_[j][i]) { is_adjacency_ = false; stop = true; break; } }
            }
            if (stop) { break; }
        }

    }

    std::string solve() const {
        if (is_adjacency_) { return "YES"; } else { return "NO"; }
    }

private:
    const size N_;
    const std::vector<std::vector<size>> matrix_;

    bool is_adjacency_;
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

    std::cout << Solution(N, matrix).solve() << std::endl;

    return 0;
}