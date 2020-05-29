//
// Created by evasilyev on 25.05.2020.
//

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <deque>
#include <sstream>
#include <algorithm>

using size = int64_t;


class Solution {
public:
    Solution(size q, std::vector<std::string> commands) : q_(q), commands_(std::move(commands)), size_(0) {}

    std::string solve() {
        std::string answer;

        for (auto& command : commands_) {
            std::istringstream iss(command);
            std::string type;
            iss >> type;
            if (type == "push") {
                size x;
                iss >> x;
                queue_.push(x);
                size_++;
            }
            else if (type == "pop") {
                queue_.pop();
                size_--;
            }
            else {
                auto copy_q = queue_;
                size min = 1e10;
                for (size i = 0; i < size_; ++i) {
                    min = std::min(min, copy_q.front());
                    copy_q.pop();
                }
                answer += std::to_string(min) + "\n";
            }
        }

        return answer;
    }


private:
    const size q_;
    const std::vector<std::string> commands_;

    std::queue<size> queue_;
    size size_;
};

int main() {

    std::string q_str;
    std::getline(std::cin, q_str);
    size q = std::stoi(q_str);

    std::vector<std::string> commands(q);
    for (size i = 0; i < q; ++i) { std::getline(std::cin, commands[i]); }

    std::cout << Solution(q, commands).solve();

    return 0;
}