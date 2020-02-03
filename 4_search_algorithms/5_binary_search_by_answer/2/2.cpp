//
// Created by evasilyev on 03.02.2020.
//

#include <iostream>

using namespace std;

// Forward declaration of isBadVersion API.
bool isBadVersion(int version) {
    return version >= 2;
}

class Solution {

public:
    int firstBadVersion(int n) {
        int l = 1;
        int r = n;
        while (l <= r) {
            int m = l + (r - l) / 2;

            if (isBadVersion(m)) {
                r = m - 1;
            }
            else {
                l = m + 1;
            }
        }

        return r + 1;
    }
};

int main() {

    int n = 0;
    std::cin >> n;

    Solution solution = Solution();
    std::cout << solution.firstBadVersion(n) << std::endl;

    return 0;
}