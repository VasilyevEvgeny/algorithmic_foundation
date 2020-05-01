//
// Created by evasilyev on 29.04.2020.
//

struct ListNode {
    explicit ListNode(int x) : val(x), next(nullptr) {}

    int val;
    ListNode *next;
};


class Solution {
public:
    static ListNode* partition(ListNode* head, int x) {
        ListNode *head_less = nullptr, *head_higher = nullptr;
        bool first_less = true, first_higher = true;
        ListNode *less = nullptr, *higher = nullptr;
        for (ListNode* node = head; node; node = node->next) {
            // less
            if (node->val < x) {
                if (first_less) {
                    head_less = node;
                    less = node;
                    first_less = false;
                }
                else {
                    less->next = node;
                    less = node;
                }
            }
            // higher
            if (node->val >= x) {
                if (first_higher) {
                    head_higher = node;
                    higher = node;
                    first_higher = false;
                }
                else {
                    higher->next = node;
                    higher = node;
                }
            }
        }

        if (head_less && head_higher) {
            less->next = head_higher;
            higher->next = nullptr;
        }
        else if (head_less && !head_higher) {
            less->next = nullptr;
        }
        else if (!head_less && head_higher) {
            head_less = head_higher;
            higher->next = nullptr;
        }
        else {
            head_less = nullptr;
        }

        return head_less;
    }
};


int main() {

    return 0;
}