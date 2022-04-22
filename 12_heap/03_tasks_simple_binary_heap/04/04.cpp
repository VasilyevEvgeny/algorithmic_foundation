#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using integer = int64_t;


template <typename T>
std::ostream& operator<< (std::ostream& os, const std::vector<T>& v) {
    for (const auto& e : v) { os << e << " "; }
    return os;
}


class MaxHeap {
public:
    MaxHeap(const integer N, std::vector<integer> data) : N_(N), data_(std::move(data)) {}

    integer Size() const {
        return static_cast<integer>(data_.size());
    }

    void Add(integer val) {
        data_.push_back(val);
        SiftUp(data_.size() - 1);
    }

    void SiftUp(integer i) {
        integer parent = (i - 1) / 2;
        if (data_[i] > data_[parent]) {
            std::swap(data_[i], data_[parent]);
            SiftUp(parent);
        }
    }

    std::vector<integer> GetData() const { return data_; }

    integer GetMax() { return data_[0]; }

public:
    const integer N_;
    std::vector<integer> data_;

};


class Solution {
public:
    Solution(const integer N, MaxHeap heap, const integer M, std::vector<std::string> queries)
    : N_(N), heap_(std::move(heap)), M_(M), queries_(std::move(queries)) {}

    void solve() {
        for (const auto& query : queries_) {
           std::istringstream iss(query);
            integer type = 0;
           iss >> type;
           if (type == 1) {
               if (!heap_.Size()) { std::cout << -1 << std::endl; }
               else { std::cout << heap_.GetMax() << std::endl; }
           }
           else if (type == 2) {
               integer e = 0;
               iss >> e;
               if (heap_.Size() == N_) { std::cout << -1 << std::endl; }
               else {
                   heap_.Add(e);
                   std::cout << heap_.GetData() << std::endl;
               }
           }
        }
    }

private:
    const integer N_;
    MaxHeap heap_;
    const integer M_;
    std::vector<std::string> queries_;
};


int main() {

    integer N = 0, M = 0;
    std::cin >> N >> M;
    std::cin.ignore();

    std::vector<std::string> queries(M);
    for (integer i = 0; i < M; ++i) { std::getline(std::cin, queries[i]); }

    Solution(N, {N, {}}, M, queries).solve();

    return 0;
}