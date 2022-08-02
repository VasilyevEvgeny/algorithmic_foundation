#include <iostream>
#include <random>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

mt19937 rnd(1928);

class Tree {
public:
    class Node {
    public:
        Node* left;
        Node* right;
        int searchKey;
        int heapKey;
        int64_t sum;
        Node() : left(nullptr), right(nullptr), searchKey(0), heapKey(rnd()), sum(0) {}
        Node(int _searchKey) : left(nullptr), right(nullptr),
                               searchKey(_searchKey), heapKey(rnd()), sum(_searchKey) {}
        ~Node() = default;
    };

private:
    Node* root;
    void delete_dfs(Node* cur) {
        if (!cur) {
            return;
        }
        if (cur->left) {
            delete_dfs(cur->left);
        }
        if (cur->right) {
            delete_dfs(cur->right);
        }
        delete(cur);
        return;
    }

    int64_t get_sum(Node* x) const {
        if (!x) {
            return 0;
        } else {
            return x->sum;
        }
    }

    void relax(Node* root) const {
        root->sum = get_sum(root->left) + get_sum(root->right) +
                    static_cast<int64_t>(root->searchKey);
    }

    Node* merge(Node* a, Node* b) {
        if (!a) {
            return b;
        }
        if (!b) {
            return a;
        }
        if (a->heapKey >= b->heapKey) {
            a->right = merge(a->right, b);
            relax(a);
            return a;
        } else {
            b->left = merge(a, b->left);
            relax(b);
            return b;
        }
    }

    pair<Node*, Node*> split(Node* r, int key) {
        if (!r) {
            return { nullptr, nullptr };
        }
        if (r->searchKey < key) {
            Node* leftPart;
            Node* rightPart;
            tie(leftPart, rightPart) = split(r->right, key);
            r->right = leftPart;
            relax(r);
            return { r, rightPart };
        } else {
            Node* leftPart;
            Node* rightPart;
            tie(leftPart, rightPart) = split(r->left, key);
            r->left = rightPart;
            relax(r);
            return { leftPart, r };
        }
    }

public:
    Tree() : root(nullptr) {}
    Tree(Node* _root) : root(_root) {}
    ~Tree() {
        delete_dfs(root);
    }

    void insert(int key) {
        Node* lKey;
        Node* eKey;
        Node* gKey;
        tie(lKey, gKey) = split(root, key);
        tie(eKey, gKey) = split(gKey, key + 1);
        if (!eKey) {
            eKey = new Node(key);
        }
        root = merge(lKey, eKey);
        root = merge(root, gKey);
    }

    int64_t sum(int l, int r) {
        Node* left;
        Node* mid;
        Node* right;
        tie(left, mid) = split(root, l);
        tie(mid, right) = split(mid, r + 1);
        int64_t ans = get_sum(mid);
        root = merge(left, mid);
        root = merge(root, right);
        return ans;
    }
};

int main() {
    const int MOD = 1e9;
    Tree cartesianTree;
    int n;
    cin >> n;
    char op;
    int l, r, x;
    int64_t lastResult = 0;
    while (n--) {
        cin >> op;
        if (op == '+') {
            cin >> x;
            cartesianTree.insert((static_cast<int64_t>(x) + lastResult) % MOD);
            lastResult = 0;
        } else {
            cin >> l >> r;
            lastResult = cartesianTree.sum(l, r);
            cout << lastResult << '\n';
        }
    }
}