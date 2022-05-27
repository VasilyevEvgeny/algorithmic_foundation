#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

#define MODE 1
#define VERBOSE false


using size = uint64_t;


template <typename T>
std::ostream& operator << (std::ostream& os, const std::deque<T>& v) {
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
  Solution(const size S, std::deque<size> users)
      : S_(S)
      , users_(std::move(users)) {

    counting_sort(users_, MAX_DATA);

    if (VERBOSE) {
      std::cout << users_ << std::endl;
    }

    size s = 0;
    bool over = false;
    for (size_t i = 0; i < users_.size(); ++i) {
      s += users_[i];

      if (VERBOSE) {
        std::cout << s << std::endl;
      }

      if (s > S_) {
        over = true;

        if (VERBOSE) {
          std::cout << "over = " << over << std::endl;
        }

        if (i) {
          std::cout << i << std::endl;
        }
        else {
          std::cout << 0 << std::endl;
        }
        break;
      }
    }

    if (!over) {
      std::cout << users_.size() << std::endl;
    }

  }

  void counting_sort(std::deque<size>& arr, const size max_element) {
    std::deque<size> cnt(max_element);
    for (size_t i = 0; i < arr.size(); ++i) {
      cnt[arr[i] - 1]++;
    }

    for (size_t i = 0, k = 0; i < cnt.size(); ++i) {
      while (cnt[i]) {
        arr[k++] = i + 1;
        cnt[i]--;
      }
    }
}

private:
  std::deque<size> users_;
  const size S_;

  const size MAX_DATA = 300;
};


int main() {

  size S = 0, N = 0;
  std::cin >> S >> N;

  if (VERBOSE) {
    std::cout << "S =" << S << ", N =" << N << std::endl;
  }

  std::deque<size> users(N);
  for (size i = 0; i < N; ++i) {
    std::cin >> users[i];
  }

  if (VERBOSE) {
    std::cout << users << std::endl;
  }

  Solution solution(S, users);

  return 0;
}