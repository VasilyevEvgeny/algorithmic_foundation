#include <algorithm>
#include <iostream>
#include <stack>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

class Node {
public:
    Node* left;
    Node* right;
    Node* parent;
    int num;
    int searchKey;
    int heapKey;

    Node() : left(nullptr), right(nullptr), parent(nullptr), num(0),
             searchKey(0), heapKey(0) {}
    Node(int _searchKey, int _heapKey, int _num) : left(nullptr), right(nullptr),
                                                   parent(nullptr), searchKey(_searchKey), heapKey(_heapKey), num(_num) {}
    ~Node() = default;

    void link_left(Node* x) {
        if (x == nullptr) {
            return;
        }
        left = x;
        x->parent = this;
    }

    void link_right(Node* x) {
        if (x == nullptr) {
            return;
        }
        right = x;
        x->parent = this;
    }
};

int get_num(const Node* a) {
    if (!a) {
        return 0;
    } else {
        return a->num;
    }
}

int main() {
    int n;
    cin >> n;
    vector<Node> nodes(n);
    int num = 1;
    for (auto& p : nodes) {
        cin >> p.searchKey;
        cin >> p.heapKey;
        p.num = num++;
    }
    sort(nodes.begin(), nodes.end(), [](const Node& a, const Node& b) {
        return a.searchKey < b.searchKey;
    });
    stack<Node*> cut;
    for (auto& node : nodes) {
        Node* lastDeleted = nullptr;
        while (!cut.empty() && cut.top()->heapKey > node.heapKey) {
            lastDeleted = cut.top();
            cut.pop();
        }
        if (cut.empty()) {
            node.link_left(lastDeleted);
        } else {
            cut.top()->link_right(&node);
            node.link_left(lastDeleted);
        }
        cut.push(&node);
    }
    vector<tuple<int, int, int>> ans(n);
    for (auto& node : nodes) {
        ans[node.num - 1] = { get_num(node.parent), get_num(node.left), get_num(node.right) };
    }
    cout << "YES\n";
    for (const auto& x : ans) {
        int parent_num;
        int left_num;
        int right_num;
        tie(parent_num, left_num, right_num) = x;
        cout << parent_num << ' ' << left_num << ' ' << right_num << '\n';
    }
}