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
    static ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* cur = head;
        while (cur != nullptr) {
            ListNode* tmp = cur->next;
            cur->next = prev;
            prev = cur;
            cur = tmp;
        }
        return prev;
    }
};

int main() {


    return 0;
}