#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#define VERBOSE false

using integer = int64_t;


template <typename T>
std::ostream& operator<< (std::ostream& os, const std::vector<T>& v) {
    for (const auto& e : v) { os << e << " "; }
    return os;
}


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

    std::vector<integer> GetData() const { return data_; }

private:
    std::vector<integer> data_;
};


std::ostream& operator<< (std::ostream& os, const MaxHeap& heap) {
    os << heap.GetData();
    return os;
}


class Solution {
public:
    explicit Solution(const integer N) : N_(N) {}

    std::vector<integer> solve() const {
        MaxHeap heap;
        integer max_1 = log_x(2, N_) + 1;
        integer max_2 = log_x(5, N_) + 1;

        if (VERBOSE) {
            std::cout << "max_1 = " << max_1 << ", 2^max_1 = " << std::pow(2, max_1) << std::endl;
            std::cout << "max_2 = " << max_2 << ", 5^max_2 = " << std::pow(5, max_2) << std::endl;
        }

        for (integer i = 0; i <= max_1; ++i) {
            for (integer j = 0; j <= max_2; ++j) {
                integer val = std::pow(2, i) + std::pow(5, j);
                if (val <= N_) {
                    heap.Add(val);
                }
            }
        }

        auto v = heap.GetData();
        std::sort(v.begin(), v.end());

        return v;
    }

private:

    static integer log_x(double base, double x) {
        return static_cast<integer>(log(x) / log(base));
    }

    const integer N_;

};


int main() {

    integer N = 0;
    std::cin >> N;

    std::cout << Solution(N).solve() << std::endl;

    return 0;
}