#include <iostream>

#define VERBOSE false

struct ListNode {
    explicit ListNode(int x) : val(x), next(nullptr) {}

    int val;
    ListNode *next;
};

class Solution {
public:
    static ListNode* middleNode(ListNode* head) {
        int64_t n = 0;
        for (ListNode* node = head; node != nullptr; node = node->next) { n++; }

        if (VERBOSE) { std::cout << "n --> " << n << std::endl; }

        n = n / 2;
        if (VERBOSE) { std::cout << "n /= 2 --> " << n << std::endl; }

        ListNode* node = head;
        for (int64_t i = 0; i < n; ++i) {
            node = node->next;
        }

        return node;
    }
};

int main() {



    return 0;
}