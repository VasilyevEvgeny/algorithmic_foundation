#include <iostream>
#include <string>
#include <stack>

class Solution {
public:
    explicit Solution(std::string s) : s_(std::move(s)) {}

    std::string decode() {
        std::stack<char> decoded;
        for (auto& symbol : s_) {
            if (decoded.empty()) { decoded.push(symbol); }
            else {
                if (decoded.top() == symbol) { decoded.pop(); }
                else { decoded.push(symbol); }
            }
        }

        std::string res_inv;
        while (!decoded.empty()) {
            res_inv += decoded.top();
            decoded.pop();
        }

        std::string res;
        for (int64_t i = res_inv.length() - 1; i >= 0; --i) { res += res_inv[i]; }

        return res;
    }


private:
    const std::string s_;

};

int main() {

    std::string s;
    std::getline(std::cin, s);

    std::cout << Solution(s).decode() << std::endl;

    return 0;
}