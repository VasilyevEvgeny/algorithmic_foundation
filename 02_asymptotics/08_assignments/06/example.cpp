#include <vector>

std::vector<int> primes(int x) {
    std::vector<int> result;
    int i = 2;
    while (i * i <= x) {
        while (x % i == 0) {
            result.push_back(i);
            x /= i;
        }
        ++i;
    }
    if (x != 1) {
        result.push_back(x);
    }
    return result;
}