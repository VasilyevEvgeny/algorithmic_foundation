//
// Created by evasilyev on 20.06.2020.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using integer = int64_t;


class MaxHeap {
public:
    MaxHeap() : data_(std::vector<integer>()) {}

    integer Size() { return data_.size(); }

    void Add(integer val) {
        data_.push_back(val);
        SiftUp(Size() - 1);
    }

    integer Extract() {
        integer val = data_[0];
        std::swap(data_[0], data_[Size() - 1]);
        data_.pop_back();
        SiftDown(0);

        return val;
    }

    void SiftUp(integer i) {
        integer parent = (i - 1) / 2;
        if (data_[i] > data_[parent]) {
            std::swap(data_[i], data_[parent]);
            SiftUp(parent);
        }
    }

    void SiftDown(integer i) {
        integer left = 2 * i + 1;
        integer right = left + 1;
        if (left < Size()) {
            integer next = left;
            if (right < Size()) { if (data_[right] > data_[left]) { next = right; } }
            if (data_[next] > data_[i]) {
                std::swap(data_[i], data_[next]);
                SiftDown(next);
            }
        }
    }

private:
    std::vector<integer> data_;
};


class Solution {
public:
    Solution(const integer N, std::vector<std::string> commands, MaxHeap heap)
    : N_(N), commands_(std::move(commands)), heap_(std::move(heap)) {}

    void solve() {
        for (auto& command : commands_) {
            std::istringstream iss(command);
            integer type = 0;
            iss >> type;
            if (type == 0) {
                integer sum = 0;
                iss >> sum;
                heap_.Add(sum);
            }
            else if (type == 1) { std::cout << heap_.Extract() << std::endl; }
        }
    }

private:
    const integer N_;
    const std::vector<std::string> commands_;

    MaxHeap heap_;
};


int main() {

    integer N = 0;
    std::cin >> N;
    std::cin.ignore();

    std::vector<std::string> commands(N);
    for (integer i = 0; i < N; ++i) { std::getline(std::cin, commands[i]); }

    Solution(N, commands, {}).solve();

    return 0;
}