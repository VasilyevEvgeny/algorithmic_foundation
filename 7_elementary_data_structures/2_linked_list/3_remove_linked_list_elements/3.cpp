//
// Created by evasilyev on 26.04.2020.
//

#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <cassert>

// 1 - work, 2 - manual testing
#define MODE 2

#define VERBOSE false

template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) {
    os << "[ ";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1) {
            os << " ";
        }
    }
    os << " ]";
    return os;
}

struct ListNode {
    explicit ListNode(int x) : val(x), next(nullptr) {}

    int val;
    ListNode *next;
};

std::vector<ListNode*> make_linked_list(std::vector<int>& values) {
    std::vector<ListNode*> linked_list;
    for (auto& val : values) {
        auto node = new ListNode(val);
        linked_list.push_back(node);
    }
    for (size_t i = 0; i < linked_list.size() - 1; ++i) {
        linked_list[i]->next = linked_list[i + 1];
    }

    return linked_list;
}

std::ostream& operator << (std::ostream& os, ListNode& node) {
    os << "NODE{" << &node << " -> " << node.next << ", val = " << node.val << "}";
    return os;
}

std::ostream& operator << (std::ostream& os, std::vector<ListNode*>& linked_list) {
    for (size_t i = 0; i < linked_list.size(); ++i) {
        os << "#" << std::setw(4) << std::setfill('0') << i << ": " <<  *linked_list[i] << std::endl;
    }

    return os;
}

std::ostream& operator << (std::ostream& os, ListNode* node) {
    size_t i = 0;
    while (node) {
        os << "#" << std::setw(4) << std::setfill('0') << i << ": " <<  *node << std::endl;
        i++;
        if (node->next) { node = node->next; }
        else { break; }
    }

    return os;
}

bool operator == (ListNode& lhs_head, ListNode& rhs_head) {
    ListNode* node_lhs = &lhs_head, *node_rhs = &rhs_head;
    while (node_lhs != nullptr || node_rhs != nullptr) {
        if ((node_lhs == nullptr && node_rhs != nullptr) || (node_lhs != nullptr && node_rhs == nullptr) ) { return false; }
        if (node_lhs->val != node_rhs->val) { return false; }

        node_lhs = node_lhs->next;
        node_rhs = node_rhs->next;
    }

    return true;
}


class Solution {
public:
    static ListNode* removeElements(ListNode* head, int val) {
        ListNode* node = head;
        ListNode* prev;

        while (node) {
            bool flag_break = false;
            if (node->val != val) {
                prev = node;
                if (VERBOSE) { std::cout << "node->val = " << node->val << " is not equal to val! prev --> " << (*prev).val << std::endl;}
            }
            else {
                if (node->next == nullptr) {
                    prev->next = nullptr;
                    if (VERBOSE) { std::cout << "node->next == nullptr! prev->next --> nullptr" << std::endl; }
                    break;
                }
                if (VERBOSE) { std::cout << "node->val = " << node->val << " is equal to val!" << std::endl; }
                while (node->val == val) {
                    node = node-> next;
                    if (VERBOSE) { std::cout << ".....node --> " << (*node).val << std::endl; }
                    if (node->next == nullptr) {
                        flag_break = true;
                        if (VERBOSE) { std::cout << ".....node = " << (*node).val << ", node->next is nullptr! flag_break = true!" << std::endl; }
                        break;
                    }
                }

                if (flag_break) {
                    if (node->val != val) { prev->next = node; }
                    else { prev->next = nullptr; }
                    if (VERBOSE) { std::cout << "prev->next --> nullptr" << std::endl; }
                    break;
                }
                prev->next = node;
                if (VERBOSE) { std::cout << "prev->next --> " << (*prev->next).val << std::endl; }
            }

            if (node->next) {
                node = node->next;
                if (VERBOSE) { std::cout << "node --> " << (*node).val << std::endl; }
            }
            else { break; }
        }

        if (head->val == val) {
            head = head->next;
            if (VERBOSE) { std::cout << "head->val = " << head->val << " is equal to val! head --> " << head << std::endl; }
        }

        return head;
    }
};


class ManualTester {
public:
    ManualTester() = default;

    static void test() {
        std::vector<std::map<std::string, std::string>> test_data {
            // basic
            {{"linked_list", "1"}, {"val", "100"}, {"true", "1"}},
            {{"linked_list", "1 2"}, {"val", "100"}, {"true", "1 2"}},
            {{"linked_list", "1 1 2"}, {"val", "2"}, {"true", "1 1"}},
//            {{"linked_list", "1 1 2"}, {"val", "1"}, {"true", "2"}},
            {{"linked_list", "1 1 2"}, {"val", "100"}, {"true", "1 1 2"}},

            // medium
            {{"linked_list", "1 2 6 3 4 5 6"}, {"val", "6"}, {"true", "1 2 3 4 5"}},
        };

        for (const auto& line : test_data) {
            int val = std::stoi(line.at("val"));

            std::string s = line.at("linked_list");
            std::vector<int> v = str_to_vector(s);
            auto head = make_linked_list(v)[0];
            std::cerr << "linked_list:\n" << head << "val = " << val << "\n" << std::endl;

            std::cout << std::endl;

            auto processed_head = Solution::removeElements(head, val);
            std::cerr << "processed_linked_list:\n" << processed_head << std::endl;

            std::cout << std::endl;

            std::string s_true = line.at("true");
            std::vector<int> v_true = str_to_vector(s_true);
            auto processed_head_true = make_linked_list(v_true)[0];
            std::cerr << "processed_linked_list_true:\n" << processed_head_true << std::endl;

            assert (*processed_head == *processed_head_true);

            std::cerr << "===========================" << std::endl;

            s.clear();
            v.clear();
            s_true.clear();
            v_true.clear();
        }
    }

private:

    static std::vector<int> str_to_vector(const std::string& str) {
        std::stringstream iss(str);
        std::vector<int> res;
        int a_i = 0;
        while (iss >> a_i)
            res.push_back(a_i);

        return res;
    }

};


int main() {

#if MODE == 1
    std::vector<int> values{1, 1, 2};

    auto linked_list = make_linked_list(values);
    std::cout << "Initial:\n" << linked_list[0] << std::endl;

    auto head = Solution::removeElements(linked_list[0], 1);
    std::cout << "Processed:\n" << head << std::endl;

#elif MODE == 2
    ManualTester::test();

#endif

    return 0;
}
