//
// Created by evasilyev on 02.06.2020.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

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
    Solution(const size N, std::vector<std::vector<size>> info)
    : N_(N), info_(std::move(info))
    , adjacency_matrix_(std::vector<std::vector<size>>(N_, std::vector<size>(N_, 0))) {}

    void solve() {
        for (size i = 0; i < N_; ++i) {
            for (size j = 1; j < info_[i].size(); ++j) {
                auto vert = info_[i][j];
                adjacency_matrix_[i][vert - 1] = 1;
            }
        }

        std::cout << adjacency_matrix_ << std::endl;
    }

private:
    const size N_;
    const std::vector<std::vector<size>> info_;

    std::vector<std::vector<size>> adjacency_matrix_;

};


int main() {

    size N = 0;
    std::cin >> N;
    std::cin.ignore();

    std::vector<std::vector<size>> info(N);
    for (size i = 0; i < N; ++i) {
        std::string str;
        std::getline(std::cin, str);
        std::istringstream iss(str);
        size var = 0;
        std::vector<size> row;
        while (iss >> var) {
            row.push_back(var);
        }
        info[i] = row;
    }

    Solution(N, info).solve();

    return 0;
}