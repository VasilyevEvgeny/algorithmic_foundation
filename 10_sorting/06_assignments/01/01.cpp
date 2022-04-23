#include <iostream>
#include <vector>
#include <algorithm>

#define MODE 1

#define VERBOSE false

using size = size_t;

class Solution {
public:
    explicit Solution(std::vector<size> v) : v_(std::move(v)) {
        std::sort(v_.begin(), v_.end(),
                  [](const size& lhs, const size& rhs){
            size lhs_last_digit = lhs % 10;
            size rhs_last_digit = rhs % 10;
            if (lhs_last_digit != rhs_last_digit) {
                return lhs_last_digit < rhs_last_digit;
            }
            else {
                return lhs < rhs;
            }
        });
    }

    void print() {
        for (const auto e : v_) {
            std::cout << e << " ";
        }
    }

private:
    std::vector<size> v_;

};



int main() {

#if MODE == 1
    size_t N = 0;
    std::cin >> N;

    std::vector<size_t> arr(N);
    for (size_t i = 0; i < N; ++i) {
        std::cin >> arr[i];
    }

    Solution solution(arr);
    solution.print();

#elif MODE == 2


#endif



    return 0;
}

