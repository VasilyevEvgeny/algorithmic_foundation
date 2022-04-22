//
// Created by evasilyev on 02.06.2020.
//

#include <iostream>
#include <vector>
#include <algorithm>


using size = int64_t;


class Solution {
public:
    Solution(const size n, const size m, std::vector<std::vector<size>> clients)
    : n_(n), m_(m), clients_(std::move(clients)), employees_(std::vector<size>(m_, 0)) {}

    void solve() {

        for (size i = 0; i < n_; ++i) {
            auto t_min = employees_[0];
            auto t_min_idx = 0;
            for (size j = 1; j < m_; ++j) {
                if (employees_[j] < t_min) {
                    t_min = employees_[j];
                    t_min_idx = j;
                }
            }

            clients_[i][0] = std::max(clients_[i][0], t_min) + clients_[i][2];
            employees_[t_min_idx] = clients_[i][0];
        }

        std::sort(clients_.begin(), clients_.end());

        size last = 0;
        std::vector<size> answer(n_, 0);
        for (size i = 0; i < n_; ++i) {
            last = std::max(clients_[i][0], last) + clients_[i][3];
            answer[clients_[i][1]] = last;
        }

        for (auto& line : answer) {
            std::cout << line << std::endl;
        }
    }

private:
    const size n_;
    const size m_;
    std::vector<std::vector<size>> clients_;
    std::vector<size> employees_;
};


int main() {

    size n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<std::vector<size>> clients;
    for (size i = 0; i < n; ++i) {
        size t = 0, a = 0, b = 0;
        std::cin >> t >> a >> b;
        clients.push_back({t, i, a, b});
    }

    Solution(n, m, clients).solve();

    return 0;
}