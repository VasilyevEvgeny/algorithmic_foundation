#include <iostream>
#include <vector>
#include <random>


class Solution {
public:
    Solution(int n, int a0, int k)
            : n_(n), a0_(a0), k_(k) {
        std::vector<int> numbers(n_);
        fill(numbers, n, a0);

        int element = kth_element(numbers, k_);
        std::cout << element << std::endl;
    }

private:
    const int n_;
    const int a0_;
    const int k_;

    void fill(std::vector<int>& a, int n, int a0) {
        //a[0] = 5;
        //a[1] = 3;
        //a[2] = 6;
        //a[3] = 10;
        //a[4] = 9;
        a.resize(n);
        a[0] = a0;
        for (int i = 1; i < n; ++i) {
            a[i] = (1103515245LL * a[i - 1] + 12345) % (1LL << 31);
        }
    }

    int Partition(std::vector<int>& numbers, int left, int right, int pivot) {
        int less = left;
        for (int i = left; i <= right; ++i) {
            if (numbers[i] < pivot) {
                std::swap(numbers[i], numbers[less]);
                ++less;
            }
        }

        int greater = right;
        for (int i = right; i >= less; --i) {
            if (numbers[i] > pivot) {
                std::swap(numbers[i], numbers[greater]);
                --greater;
            }
        }

        if (less > greater) {
            return less;
        }

        int middle = left + (right - left) / 2;

        return middle < less ? less :
               middle > greater ? greater :
               middle;
    }

    int Pivot(std::vector<int>& numbers, int left, int right) {
        static std::default_random_engine re(20200620);
        std::uniform_int_distribution<int> next_index(left, right);
        return numbers[next_index(re)];
    }

    int kth_element(std::vector<int>& numbers, int k) {
        int left = 0;
        int right = n_ - 1;
        while (left < right) {
            int pivot = Pivot(numbers, left, right);
            int partition_index = Partition(numbers, left, right, pivot);

            if (k < partition_index - left) {
                right = partition_index - 1;
            } else {
                k -= partition_index - left;
                left = partition_index;
            }
        }

        return numbers[left];
    }

};


int main() {

    int n = 0, a0 = 0, k = 0;
    std::cin >> n >> a0 >> k;


    Solution solution(n, a0, k);


    return 0;
}