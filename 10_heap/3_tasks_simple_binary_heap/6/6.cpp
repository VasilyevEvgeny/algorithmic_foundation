//
// Created by evasilyev on 20.06.2020.
//

#include <iostream>
#include <vector>

using integer = int64_t;

template <typename T>
std::ostream& operator<< (std::ostream& os, const std::vector<T>& v) {
    for (const auto& e : v) { os << e << " "; }
    return os;
}


class MaxHeap {
public:
    MaxHeap(const integer N, std::vector<integer> data) : N_(N), data_(std::move(data)) {}

    void Subtract(integer i, integer x) { data_[i] -= x; }

    void SiftDown(integer i) {
        integer left = 2 * i + 1;
        integer right = left + 1;
        if (left < data_.size())  {
            integer next = left;
            if (right < data_.size() && data_[right] > data_[left]) { next = right; }
            if (data_[next] > data_[i]) {
                std::swap(data_[next], data_[i]);
                SiftDown(next);
            }
        }
    }

    void ExtractMax() {
        std::swap(data_[0], data_[data_.size() - 1]);
        data_.pop_back();
        SiftDown(0);
    }

    std::vector<integer> GetData() const { return data_; }

private:
    const integer N_;
    std::vector<integer> data_;
};


std::ostream& operator<< (std::ostream& os, const MaxHeap& heap) {
    os << heap.GetData();
    return os;
}


class Solution {
public:
    explicit Solution(const integer N, MaxHeap heap) : N_(N), heap_(std::move(heap)) {}

    void solve() {
        for (integer i = 0; i < N_ - 1; ++i) {
            heap_.ExtractMax();
            std::cout << heap_ << std::endl;
        }
    }


private:
    const integer N_;
    MaxHeap heap_;

};


int main() {

    integer N = 0;
    std::cin >> N;

    std::vector<integer> data(N);
    for (integer i = 0; i < N; ++i) { std::cin >> data[i]; }

    Solution(N, {N, data}).solve();

    return 0;
}