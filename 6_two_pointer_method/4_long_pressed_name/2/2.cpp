//
// Created by evasilyev on 19.02.2020.
//

#include <iostream>
#include <string>

class Solution {
public:
    Solution() = default;
    ~Solution() = default;

    static size_t proceed_letter(const std::string& str, size_t pos) {
        do {
            ++pos;
        } while (pos < str.size() && str[pos] == str[pos - 1]);

        return pos;
    }

    static bool isLongPressedName(std::string& name, std::string& typed) {
        size_t t = 0;
        for (size_t n = 0; n < name.size();) {
            if (name[n] != typed[t]) {
                return false;
            }

            auto next_n = proceed_letter(name, n);
            auto next_t = proceed_letter(typed, t);

            if (next_n - n > next_t - t) {
                return false;
            }
            n = next_n;
            t = next_t;
        }

        return t == typed.size();
    }
};

int main() {

    std::string name, typed;
    std::getline(std::cin, name);
    std::getline(std::cin, typed);

    std::cout << std::boolalpha << Solution::isLongPressedName(name, typed) << std::endl;


    return 0;
}