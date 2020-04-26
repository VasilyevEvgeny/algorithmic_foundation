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
    // iterative
    static ListNode* reverseListIterative(ListNode* head) {
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

    // recursive
    static ListNode* reverseListRecursive(ListNode* head) {
        if (head == nullptr || head->next == nullptr) { return head; }
        ListNode* p = reverseListRecursive(head->next);
        head->next->next = head;
        head->next = nullptr;
        return p;
    }
};

int main() {


    return 0;
}