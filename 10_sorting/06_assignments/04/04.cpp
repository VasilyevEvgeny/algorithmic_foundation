#include <iostream>
#include <vector>
#include <deque>

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
    Solution(std::deque<size> distances, std::deque<size> tariffs) 
    : distances_(std::move(distances))
    , tariffs_(std::move(tariffs)) {

      counting_sort(distances_, MAX_DISTANCE);
      counting_sort(tariffs_, MAX_TARIFF, true);

      size summ = 0;
      for (size_t i = 0; i < distances_.size(); ++i) {
        summ += distances_[i] * tariffs_[i];
      }

      std::cout << summ << std::endl;

    }

    void counting_sort(std::deque<size>& arr, const size max_element, bool reverse=false) {
        std::deque<size> cnt(max_element);
        for (size_t i = 0; i < arr.size(); ++i) {
          cnt[arr[i] - 1]++;
        }

        if (!reverse) {
          for (size_t i = 0, k = 0; i < cnt.size(); ++i) {
            while (cnt[i]) {
              arr[k++] = i + 1;
              cnt[i]--;
            }
          }
        } else {
          for (size_t i = 0, k = arr.size() - 1; i < cnt.size(); ++i) {
            while (cnt[i]) {
              arr[k--] = i + 1;
              cnt[i]--;
            }
          }
        }
    }

private:
    std::deque<size> distances_;
    std::deque<size> tariffs_;

    const size MAX_DISTANCE = 1000;
    const size MAX_TARIFF = 10000;
};


int main() {

    size N = 0;
    std::cin >> N;
    std::deque<size> distances(N);
    for (size i = 0; i < N; ++i) {
        std::cin >> distances[i];
    }

    std::deque<size> tariffs(N);
    for (size i = 0; i < N; ++i) {
        std::cin >> tariffs[i];
    }

    Solution solution(distances, tariffs);

    return 0;
}