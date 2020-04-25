//
// Created by evasilyev on 25.04.2020.
//

#include <iostream>

#define VERBOSE false

struct ListNode {
    explicit ListNode(int x) : val(x), next(nullptr) {}

    int val;
    ListNode *next;
};

class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* r_node;
        for (ListNode* node = head; node != nullptr; node = node->next) {

        }
    }
};

int main() {


    return 0;
}