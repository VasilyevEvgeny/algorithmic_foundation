//
// Created by evasilyev on 17.06.2020.
//

#include <iostream>
#include <vector>
#include <algorithm>

#define VERBOSE true

using integer = int64_t;


std::ostream& operator<< (std::ostream& os, const std::vector<std::vector<integer>>& v) {
    for (integer i_row = 0; i_row < v.size(); ++i_row) {
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
    Solution(const integer n, std::vector<integer> times, const integer m, std::vector<std::pair<integer, integer>> limitations)
    : n_(n), times_(std::move(times)), m_(m), limitations_(std::move(limitations))
    , adjacency_list_(std::vector<std::vector<integer>>(n_, std::vector<integer>())) {

        convert_limitations_to_adjacency_list();

        if (VERBOSE) { std::cout << "adjacency_list_ =\n" << adjacency_list_ << std::endl; }
    }

    integer solve() {
        std::vector<integer> res;
        for (integer i = 0; i < n_; ++i) {
            res.push_back(dfs(i));
        }

        return *std::max_element(res.begin(), res.end());
    }

private:

    integer dfs(integer node) {
        for (auto& vertex : adjacency_list_[node]) {
            return times_[node] + dfs(vertex);
        }

        return times_[node];
    }

    void convert_limitations_to_adjacency_list() {
        for (integer i = 0; i < m_; ++i) {
            auto first = limitations_[i].first - 1;
            auto second = limitations_[i].second - 1;
            adjacency_list_[first].push_back(second);
        }
    }

    const integer n_;
    std::vector<integer> times_;
    const integer m_;
    std::vector<std::pair<integer, integer>> limitations_;

    std::vector<std::vector<integer>> adjacency_list_;

};


int main() {

    integer n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<integer> times(n);
    for (integer i = 0; i < n; ++i) { std::cin >> times[i]; }

    std::vector<std::pair<integer, integer>> limitations(m);
    for (integer i = 0; i < m; ++i) { std::cin >> limitations[i].first >> limitations[i].second; }

    std::cout << Solution(n, times, m, limitations).solve() << std::endl;

    return 0;
}