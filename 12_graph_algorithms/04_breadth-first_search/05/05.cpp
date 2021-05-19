//
// Created by evasilyev on 11.06.2020.
//

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <iomanip>

#define VERBOSE false

using integer = int64_t;


std::pair<integer, integer> operator+ (const std::pair<integer, integer>& p, integer var) {
    return {p.first + var, p.second + var};
}


std::ostream& operator<< (std::ostream& os, const std::pair<integer, integer>& p) {
    std::cout << p.first << " " << p.second;

    return os;
}


std::ostream& operator<< (std::ostream& os, const std::queue<std::pair<integer, integer>>& q) {
    auto q_copy = q;
    os << "[ ";
    for (size_t i = 0; i < q.size(); ++i) {
        os << "{" << q_copy.front() << "} ";
        q_copy.pop();
    }
    os << "]";

    return os;
}


std::ostream& operator<< (std::ostream& os, const std::vector<std::pair<integer, integer>>& v) {
    os << "{ ";
    for (const auto& e : v) {
        os << "(" << e.first << "," << e.second << "), ";
    }
    os << "}";

    return os;
}


std::ostream& operator<< (std::ostream& os, const std::vector<std::vector<integer>>& vv) {
    for (const auto& row : vv) {
        for (const auto& e : row) {
            if (e == -1) { os << "XXX "; }
            else { os << std::setw(3) << e << " ";  }
        }
        os << std::endl;
    }

    return os;
}


class Solution {
public:
    Solution(const integer N, const integer x1, const integer y1, const integer x2, const integer y2)
    : N_(N), x1_(x1 - 1), y1_(y1 - 1), x2_(x2 - 1), y2_(y2 - 1)
    , distances_(std::vector<std::vector<integer>>(N_, std::vector<integer>(N_, -1)))
    , used_(std::vector<std::vector<bool>>(N_, std::vector<bool>(N_, false)))
    , start_(std::make_pair(x1_, y1_)), end_(std::make_pair(x2_, y2_)) {}

    void solve() {
        distances_[x1_][y1_] = 0;
        used_[start_.first][start_.second] = true;
        std::queue<std::pair<integer, integer>> q;
        q.push(start_);
        while (!q.empty()) {
            auto front = q.front();
            q.pop();
            auto possible_moves = compose_possible_moves(front);
            for (const auto& move : possible_moves) {
                if (!used_[move.first][move.second]) {
                    used_[move.first][move.second] = true;
                    distances_[move.first][move.second] = distances_[front.first][front.second] + 1;
                    q.push(move);
                    if (move == end_) {
                        std::queue<std::pair<integer, integer>> q_empty;
                        std::swap(q, q_empty);
                        break;
                    }
                }
            }

            if (VERBOSE) {
                std::cout << "front --> " << front << std::endl;
                std::cout << "possible moves --> " << possible_moves << std::endl;
                std::cout << "q --> " << q << std::endl;
                std::cout << "distances_:\n" << distances_ << std::endl;
            }
        }

        auto cur_cell = end_;
        auto prev_move = end_;
        std::vector<std::pair<integer, integer>> path = {cur_cell};
        auto d = 1;
        while (cur_cell != start_) {
            auto possible_moves = compose_possible_moves(cur_cell);
            for (const auto& move : possible_moves) {
                if (distances_[move.first][move.second] == distances_[end_.first][end_.second] - d && move != prev_move) {
                    prev_move = cur_cell;
                    cur_cell = move;
                    path.push_back(cur_cell);
                    d++;
                    if (VERBOSE) {
                        std::cout << "prev_move --> " << prev_move << std::endl;
                        std::cout << "cur_cell --> " << cur_cell << std::endl;
                        std::cout << "path --> " << path << std::endl;
                        std::cout << "d --> " << d << std::endl;
                    }
                    break;
                }
            }
        }

        std::cout << path.size() << std::endl;
        for (auto it = path.rbegin(); it != path.rend(); ++it) { std::cout << *it + 1 << std::endl; }
    }

private:

    std::vector<std::pair<integer, integer>> compose_possible_moves(const std::pair<integer, integer>& cell) const {
        std::vector<std::pair<integer, integer>> possible_moves;

        // right part
        possible_moves.emplace_back(cell.first + 1, cell.second + 2);
        possible_moves.emplace_back(cell.first + 2, cell.second + 1);
        possible_moves.emplace_back(cell.first + 2, cell.second - 1);
        possible_moves.emplace_back(cell.first + 1, cell.second - 2);

        // left part
        possible_moves.emplace_back(cell.first - 1, cell.second + 2);
        possible_moves.emplace_back(cell.first - 2, cell.second + 1);
        possible_moves.emplace_back(cell.first - 2, cell.second - 1);
        possible_moves.emplace_back(cell.first - 1, cell.second - 2);

        for (auto it = possible_moves.begin(); it != possible_moves.end();) {
            if ((*it).first < 0 || (*it).first >= N_ ||
            (*it).second < 0 || (*it).second >= N_) { it = possible_moves.erase(it); }
            else { ++it; }
        }

        return possible_moves;
    }

    const integer N_;
    const integer x1_, y1_;
    const integer x2_, y2_;
    const std::pair<integer, integer> start_;
    const std::pair<integer, integer> end_;

    std::vector<std::vector<integer>> distances_;
    std::vector<std::vector<bool>> used_;

};


int main() {

    integer N = 0;
    std::cin >> N;

    integer x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    std::cin >> x1 >> y1;
    std::cin >> x2 >> y2;

    Solution(N, x1, y1, x2, y2).solve();

    return 0;
}
