//
// Created by evasilyev on 25.05.2020.
//

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <sstream>

using size = int64_t;

class Solution {
public:
    Solution(size q, std::vector<std::string> commands) : q_(q), commands_(std::move(commands)), size_(0) {}

    void solve() {
        std::string answer;

        for (auto& command : commands_) {
            std::istringstream iss(command);
            std::string type;
            iss >> type;
            if (type == "push") {
                size x;
                iss >> x;
                queue_.push(x);
                if (!size_) { mins_.push(x); }
                else {
                    auto min = std::min(x, mins_.back());
                    mins_.push(min);
                }
                size_++;
            }
            else if (type == "pop") {
                queue_.pop();
                mins_.pop();
                size_--;
            }
            else {
                std::cout << mins_.back() << std::endl;
            }
        }
    }


private:
    const size q_;
    const std::vector<std::string> commands_;

    std::queue<size> queue_;
    std::queue<size> mins_;
    size size_;
};



int main() {

    std::string q_str;
    std::getline(std::cin, q_str);
    size q = std::stoi(q_str);

    std::vector<std::string> commands(q);
    for (size i = 0; i < q; ++i) { std::getline(std::cin, commands[i]); }

    Solution(q, commands).solve();

    return 0;
}