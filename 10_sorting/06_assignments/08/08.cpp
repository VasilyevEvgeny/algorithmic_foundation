#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#define MODE 1
#define VERBOSE false


using size = uint64_t;


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
  Solution(std::vector<size> arr)
      : arr_(std::move(arr)) {

    if (arr_.size() == 1) {
      std::cout << 0 << std::endl;
    }

    MergeSort(0, arr_.size() - 1);

    std::cout << n_inv_ << std::endl;
  }

private:

  void MergeSort(size left, size right) {
    if (left < right) {
      size middle = left + (right - left) / 2;

      MergeSort(left, middle);
      MergeSort(middle + 1, right);
      Merge(left, middle, right);
    }
  }

  void Merge(size left, size middle, size right) {
    std::vector<size> buf;
    buf.reserve(right - left + 1);

    for (size l = left, r = middle + 1; l <= middle || r <= right;) {
      if (r > right || (arr_[l] < arr_[r] && l <= middle)) {
        buf.push_back(arr_[l]);
        ++l;


      }
      else {
        n_inv_++;

        buf.push_back(arr_[r]);
        ++r;
      }
    }

    /*for (size i = left; i <= right; ++i) {
        arr_[i] = buf[i - left];
    }*/

  }


  std::vector<size> arr_;
  size n_inv_ = 0;
};


int main() {

  size_t N = 0;
  std::cin >> N;

  std::vector<size> arr(N);
  for (size_t i = 0; i < N; ++i) {
    std::cin >> arr[i];
  }

  if (VERBOSE) {
    std::cout << "Initial data:" << std::endl;
    for (const auto& e : arr) {
      std::cout << e << " ";
    }
    std::cout << std::endl;
  }

  Solution solution(arr);

  return 0;
}