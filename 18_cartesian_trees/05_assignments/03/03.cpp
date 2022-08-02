#include <algorithm>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>
#include <random>
#include <string>

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
        int size;
        Node() : left(nullptr), right(nullptr), searchKey(0), heapKey(rnd()), size(1) {}
        Node(int _searchKey) : left(nullptr), right(nullptr),
                               searchKey(_searchKey), heapKey(rnd()), size(1) {}
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

    int get_size(Node* x) const {
        if (!x) {
            return 0;
        } else {
            return x->size;
        }
    }

    void relax(Node* root) const {
        root->size = get_size(root->left) + get_size(root->right) + 1;
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

    pair<Node*, Node*> split_size(Node* r, int rightSize) {
        if (!r) {
            return { nullptr, nullptr };
        }
        if (get_size(r->right) >= rightSize) {
            Node* leftPart;
            Node* rightPart;
            tie(leftPart, rightPart) = split_size(r->right, rightSize);
            r->right = leftPart;
            relax(r);
            return { r, rightPart };
        } else {
            Node* leftPart;
            Node* rightPart;
            tie(leftPart, rightPart) = split_size(r->left, rightSize - 1 - get_size(r->right));
            r->left = rightPart;
            relax(r);
            return { leftPart, r };
        }
    }

    pair<Node*, Node*> split_key(Node* r, int key) {
        if (!r) {
            return { nullptr, nullptr };
        }
        if (r->searchKey < key) {
            Node* leftPart;
            Node* rightPart;
            tie(leftPart, rightPart) = split_key(r->right, key);
            r->right = leftPart;
            relax(r);
            return { r, rightPart };
        } else {
            Node* leftPart;
            Node* rightPart;
            tie(leftPart, rightPart) = split_key(r->left, key);
            r->left = rightPart;
            relax(r);
            return { leftPart, r };
        }
    }

    int get_min_value(Node* r) {
        while (r->left) {
            r = r->left;
        }
        return r->searchKey;
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
        tie(lKey, gKey) = split_key(root, key);
        tie(eKey, gKey) = split_key(gKey, key + 1);
        eKey = new Node(key);
        root = merge(lKey, eKey);
        root = merge(root, gKey);
    }

    void del(int key) {
        Node* lKey;
        Node* eKey;
        Node* gKey;
        tie(lKey, gKey) = split_key(root, key);
        tie(eKey, gKey) = split_key(gKey, key + 1);
        delete_dfs(eKey);
        root = merge(lKey, gKey);
    }

    int max(int k) {
        Node* left;
        Node* right;
        tie(left, right) = split_size(root, k);
        int res = get_min_value(right);
        root = merge(left, right);
        return res;
    }
};

int main() {
    Tree cartesianTree;
    int n;
    int operation, k;
    cin >> n;
    while (n--) {
        cin >> operation >> k;
        if (operation == 1) {
            cartesianTree.insert(k);
        } else if (operation == 0) {
            cout << cartesianTree.max(k) << '\n';
        } else {
            cartesianTree.del(k);
        }
    }
}