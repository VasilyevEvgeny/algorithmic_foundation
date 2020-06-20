//
// Created by evasilyev on 20.06.2020.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <map>

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

    integer SiftDown(integer i) {
        integer left = 2 * i + 1;
        integer right = left + 1;
        if (left < data_.size())  {
            integer next = left;
            if (right < data_.size() && data_[right] > data_[left]) { next = right; }
            if (data_[next] > data_[i]) {
                std::swap(data_[next], data_[i]);
                return SiftDown(next);
            }
        }

        return i;
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
    Solution(MaxHeap heap, const integer M, std::vector<std::pair<integer, integer>> queries)
            : heap_(std::move(heap)), M_(M), queries_(std::move(queries)) {}

    void solve() {
        for (const auto& query : queries_) {
            integer i = query.first - 1;
            integer x = query.second;

            heap_.Subtract(i, x);
            auto idx = heap_.SiftDown(i) + 1;

            std::cout << idx << std::endl;
        }
        std::cout << heap_ << std::endl;
    }

private:
    MaxHeap heap_;
    const integer M_;
    const std::vector<std::pair<integer, integer>> queries_;

};


int main() {

    integer N = 0;
    std::cin >> N;

    std::vector<integer> data(N);
    for (integer i = 0; i < N; ++i) { std::cin >> data[i]; }
    MaxHeap heap(N, data);

    integer M = 0;
    std::cin >> M;

    std::vector<std::pair<integer, integer>> queries(M);
    for (integer i = 0; i < M; ++i) { std::cin >> queries[i].first >> queries[i].second; }

    Solution(heap, M, queries).solve();

    return 0;
}