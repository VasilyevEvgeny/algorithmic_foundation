#include <iostream>
#include <unordered_set>


int main() {
    size_t n = 0;
    std::cin >> n;

    std::unordered_set<int64_t> set;
    int64_t sum = 0;
    for (size_t i = 0; i < n; ++i) {
        int64_t t = 0, x = 0;
        std::cin >> t;
        if (t == 1) {
            std::cin >> x;
            if (set.find(x) == set.end()) {
                set.insert(x);
                sum += x;
            }
        } else if (t == 2) {
            std::cin >> x;
            if (set.find(x) != set.end()) {
                set.erase(x);
                sum -= x;
            }
        } else {
            std::cout << sum << std::endl;
        }
    }

    return 0;
}