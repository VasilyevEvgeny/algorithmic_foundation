#include <iostream>
#include <vector>

#define VERBOSE false

using size = int64_t;


std::ostream& operator<< (std::ostream& os, const std::vector<std::vector<size>>& v) {
    for (size i_row = 0; i_row < v.size(); ++i_row) {
        os << "#" << i_row << " -> ";
        for (const auto& e : v[i_row]) {
            os << e << " ";
        }
        if (i_row != v.size() - 1) { std::cout << std::endl; }
    }

    return os;
}


class Solution {
public:
    Solution(const size N, std::vector<std::vector<size>> adjacency_matrix)
    : N_(N), adjacency_matrix_(std::move(adjacency_matrix)), used_(std::vector<bool>(N_, false))
    , adjacency_list_(std::vector<std::vector<size>>(N, std::vector<size>())) {

        convert_to_adjacency_list();

        if (VERBOSE) { std::cout << "adjacency_list_ =\n" << adjacency_list_ << std::endl; }
    }

    void solve() {
        for (size i = 0; i < N_; ++i) {
            if (!used_[i]) { dfs(i); }
        }

        std::cout << "NO" << std::endl;
    }


private:

    void dfs(size node, size prev = -1) {
        if (VERBOSE) { std::cout << "node = " << node << ", used_[" << node << "] = " << used_[node] << std::endl; }
        used_[node] = true;
        if (VERBOSE) { std::cout << "used_[" << node << "] --> " << used_[node] << std::endl; }

        for (const auto& vertex : adjacency_list_[node]) {
            if (!used_[vertex]) {
                if (VERBOSE) { std::cout << ".....!used_[" << vertex << "], vertex = " << vertex << ", node = " << node <<
                ", call dfs(" << vertex << ", " << node << ")" << std::endl; }
                dfs(vertex, node);
            }
            else if (vertex != prev) {
                if (VERBOSE) { std::cout << ".....vertex != prev : " << vertex << " != " << prev << std::endl; }
                std::cout << "YES" << std::endl;
                exit(0);
            }

            if (VERBOSE) { std::cout << ".....else nothing branch" << std::endl; }
        }
    }

    void convert_to_adjacency_list() {
        for (size i = 0; i < N_; ++i) {
            for (size j = 0; j < N_; ++j) {
                if (adjacency_matrix_[i][j]) { adjacency_list_[i].push_back(j); }
            }
        }
    }


    const size N_;
    const std::vector<std::vector<size>> adjacency_matrix_;

    std::vector<std::vector<size>> adjacency_list_;
    std::vector<bool> used_;
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