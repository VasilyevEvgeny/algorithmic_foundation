#include <iostream>
#include <vector>
#include <queue>

using size = int64_t;


std::ostream& operator << (std::ostream& os, const std::vector<size>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1) {
            os << " ";
        }
    }
    return os;
}


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
    Solution(const size N, const size x, std::vector<std::vector<size>> adjacency_matrix)
    : N_(N), x_(x), adjacency_matrix_(std::move(adjacency_matrix)), distances_(std::vector<size>(N_, -1))
    , used_(std::vector<bool>(N_, false)) {
        distances_[x - 1] = 0;
    }

    void solve() {
        bfs(x_ - 1);
        std::cout << distances_ << std::endl;
    }

private:

    void bfs(size node) {
        used_[node] = true;

        std::queue<size> q;
        q.push(node);

        while (!q.empty()) {
            size front = q.front();
            q.pop();
            for (size i = 0; i < N_; ++i) {
                if (adjacency_matrix_[front][i] && distances_[i] == -1) {
                    q.push(i);
                    distances_[i] = distances_[front] + 1;
                }
            }
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
    const size x_;
    const std::vector<std::vector<size>> adjacency_matrix_;

    std::vector<std::vector<size>> adjacency_list_;
    std::vector<size> distances_;
    std::vector<bool> used_;
};


int main() {

    size N = 0, x = 0;
    std::cin >> N >> x;

    std::vector<std::vector<size>> matrix(N, std::vector<size>(N, 0));
    for (size i = 0; i < N; ++i) {
        for (size j = 0; j < N; ++j) {
            std::cin >> matrix[i][j];
        }
    }

    Solution(N, x, matrix).solve();


    return 0;
}
