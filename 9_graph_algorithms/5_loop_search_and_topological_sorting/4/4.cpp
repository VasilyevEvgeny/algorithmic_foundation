#include <iostream>
#include <vector>
#include <algorithm>

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
    : N_(N), M_(M), edges_(std::move(edges)), colors_(std::vector<Color>(N_, Color::White))
    , adjacency_list_(std::vector<std::vector<size>>(N_, std::vector<size>()))
    , ancestors_(std::vector<size>(N_, -1))
    , cycle_start_(-1), cycle_end_(-1) {

        convert_edges_to_adjacency_list();

        if (VERBOSE) { std::cout << "adjacency_list_ =\n" << adjacency_list_ << std::endl; }
    }

    void solve() {
        for (size i = 0; i < N_; ++i) { if (dfs(i)) { break; }  }

        if (cycle_start_ == -1) { std::cout << "NO" << std::endl; }
        else {
            std::vector<size> cycle;
            for (size node = cycle_end_; node != cycle_start_; node = ancestors_[node]) { cycle.push_back(node); }
            cycle.push_back(cycle_start_);
            std::reverse(cycle.begin(), cycle.end());
            std::cout << "YES" << std::endl;
            for (const auto& vertex : cycle) { std::cout << vertex + 1 << " "; }
            std::cout << std::endl;
        }
    }

private:

    bool dfs(size node) {
        colors_[node] = Color::Gray;
        if (VERBOSE) { std::cout << "node #" << node << " --> GRAY" << std::endl; }
        for (auto& vertex : adjacency_list_[node]) {
            if (colors_[vertex] == Color::White) {
                ancestors_[vertex] = node;
                if (VERBOSE) { std::cout << "vertex #" << vertex << " is WHITE, go dfs(" << vertex << ")" << std::endl; }
                if (dfs(vertex)) { return true; }
            }
            else if (colors_[vertex] == Color::Gray) {
                cycle_start_ = vertex;
                cycle_end_ = node;
                if (VERBOSE) { std::cout << "vertex #" << vertex << " is GRAY, cycle_start_ --> " << vertex <<
                ", cycle_end_ --> " << node << std::endl; }
                return true;
            }
        }
        colors_[node] = Color::Black;
        if (VERBOSE) { std::cout << "node #" << node << " --> BLACK" << std::endl; }

        return false;
    }

    void convert_edges_to_adjacency_list() {
        for (size i = 0; i < M_; ++i) {
            auto first = edges_[i].first - 1;
            auto second = edges_[i].second - 1;
            adjacency_list_[first].push_back(second);
        }
    }

    const size N_;
    const size M_;
    const std::vector<std::pair<size, size>> edges_;

    std::vector<std::vector<size>> adjacency_list_;
    enum class Color { White, Gray, Black };
    std::vector<Color> colors_;
    std::vector<size> ancestors_;
    size cycle_start_;
    size cycle_end_;
};


int main() {

    size N = 0, M = 0;
    std::cin >> N >> M;

    std::vector<std::pair<size, size>> edges(M);
    for (size i = 0; i < M; ++i) {
        size first = 0, second = 0;
        std::cin >> first >> second;
        edges[i] = {first, second};
    }

    Solution(N, M, edges).solve();

    return 0;
}
