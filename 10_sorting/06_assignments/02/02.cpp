#include <iostream>
#include <vector>

#define MODE 1

using size = size_t;

template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) {
    os << "[ ";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1) {
            os << " ";
        }
    }
    os << " ]";
    return os;
}


class Solution {
public:
    explicit Solution(std::vector<size> arr) : v_(std::move(arr)) {

        MergeSort(0, v_.size() - 1);

        for (const auto e : v_) {
            std::cout << e << " ";
        }
    }

private:

    void MergeSort(const size left, const size right) {
        if (left < right) {
            size middle = left + (right - left) / 2;
            MergeSort(left, middle);
            MergeSort(middle + 1, right);
            Merge(left, middle, right);
        }
    }

    void Merge(const size left, const size middle, const size right) {
        std::vector<size> buf;
        buf.reserve(right - left + 1);

        for (size l = left, r = middle + 1; l <= middle || r <= right;) {
            if (r > right || (v_[l] < v_[r] && l <= middle)) {
                buf.push_back(v_[l]);
                ++l;
            }
            else {
                buf.push_back(v_[r]);
                ++r;
            }
        }

        for (size i = left; i <= right; ++i) {
            v_[i] = buf[i - left];
        }
    }

    std::vector<size> v_;
};


int main() {

#if MODE == 1
    size N = 0;
    std::cin >> N;
    std::vector<size> arr(N);
    for (size i = 0; i < N; ++i) {
        std::cin >> arr[i];
    }

    Solution solution(arr);

#elif MODE == 2

#endif


    return 0;
}