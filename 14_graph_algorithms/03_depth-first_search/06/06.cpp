#include <iostream>
#include <vector>
#include <map>

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
    Solution(const size N, const size M, std::vector<std::pair<size, size>> edges)
    : N_(N), M_(M), edges_(std::move(edges))
    , adjacency_list_(std::vector<std::vector<size>>(N, std::vector<size>()))
    , used_(std::vector<bool>(N, false)) {

        convert_edges_to_adjacency_list();

        if (VERBOSE) { std::cout << "adjacency_list_ =\n" << adjacency_list_ << std::endl; }
    }

    size solve() {
        size n_components = 0;
        for (size i = 0; i < N_; ++i) {
            if (!used_[i]) {
                dfs(i);
                n_components++;
            }
        }

        return n_components;
    }

private:

    void convert_edges_to_adjacency_list() {
        for (size i = 0; i < M_; ++i) {
            auto first = edges_[i].first - 1;
            auto second = edges_[i].second - 1;
            adjacency_list_[first].push_back(second);
            adjacency_list_[second].push_back(first);
        }
    }

    void dfs(size v) {
        used_[v] = true;
        for (const auto vertex : adjacency_list_[v]) {
            if (!used_[vertex]) { dfs(vertex); }
        }
    }

    const size N_;
    const size M_;
    const std::vector<std::pair<size, size>> edges_;

    std::vector<std::vector<size>> adjacency_list_;
    std::vector<bool> used_;
};


int main() {

    size N = 0, M = 0;
    std::cin >> N >> M;

    std::vector<std::pair<size, size>> edges;
    for (size i = 0; i < M; ++i) {
        size first = 0, second = 0;
        std::cin >> first >> second;
        edges.emplace_back(first, second);
    }

    std::cout << Solution(N, M, edges).solve() << std::endl;

    return 0;
}