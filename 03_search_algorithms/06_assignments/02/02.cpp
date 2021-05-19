//
// Created by evasilyev on 06.02.2020.
//

#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1)
            os << ", ";
    }
    os << "]\n";
    return os;
}

int linear(std::vector<int>& numbers) {
    int min = numbers[0], idx_min = 0, max = numbers[1], idx_max = 0;
    for (size_t i = 0; i < numbers.size(); ++i) {
        if (numbers[i] < min) {
            min = numbers[i];
            idx_min = i;
            continue;
        }
        if (numbers[i] > max) {
            max = numbers[i];
            idx_max = i;
            continue;
        }
    }

//    std::cout << "min = " << min << ", idx_min = " << idx_min << std::endl;
//    std::cout << "max = " << max << ", idx_max = " << idx_max << std::endl;

    return std::abs(idx_max - idx_min) - 1;

}


int main() {

    int n = 0;
    std::cin >> n;

    auto numbers = std::vector<int>(n, 0);

    for (size_t i = 0; i < n; ++i) {
        std::cin >> numbers[i];
    }

//    std::cout << numbers;

    std::cout << linear(numbers);

    return 0;
}