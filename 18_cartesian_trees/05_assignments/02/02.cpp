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
        Node() : left(nullptr), right(nullptr), searchKey(0), heapKey(rnd()) {}
        Node(int _searchKey) : left(nullptr), right(nullptr),
                               searchKey(_searchKey), heapKey(rnd()) {}
        ~Node() = default;
    };

private:
    static const int INF = 2e9;
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

    Node* merge(Node* a, Node* b) {
        if (!a) {
            return b;
        }
        if (!b) {
            return a;
        }
        if (a->heapKey >= b->heapKey) {
            a->right = merge(a->right, b);
            return a;
        } else {
            b->left = merge(a, b->left);
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
            return { r, rightPart };
        } else {
            Node* leftPart;
            Node* rightPart;
            tie(leftPart, rightPart) = split(r->left, key);
            r->left = rightPart;
            return { leftPart, r };
        }
    }

    Node* find_left(Node* root) {
        if (!root) {
            return nullptr;
        }
        while (root->left) {
            root = root->left;
        }
        return root;
    }

    Node* find_right(Node* root) {
        if (!root) {
            return nullptr;
        }
        while (root->right) {
            root = root->right;
        }
        return root;
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

    void del(int key) {
        Node* lKey;
        Node* eKey;
        Node* gKey;
        tie(lKey, gKey) = split(root, key);
        tie(eKey, gKey) = split(gKey, key + 1);
        delete_dfs(eKey);
        root = merge(lKey, gKey);
    }

    bool exist(int key) {
        Node* lKey;
        Node* eKey;
        Node* gKey;
        tie(lKey, gKey) = split(root, key);
        tie(eKey, gKey) = split(gKey, key + 1);
        bool res = true;
        if (!eKey) {
            res = false;
        }
        root = merge(lKey, eKey);
        root = merge(root, gKey);
        return res;
    }

    int greater(int key) {
        ++key;
        Node* lKey;
        Node* gKey;
        tie(lKey, gKey) = split(root, key);
        int res = INF;
        Node* elem = find_left(gKey);
        if (elem) {
            res = elem->searchKey;
        }
        root = merge(lKey, gKey);
        return res;
    }

    int less(int key) {
        Node* lKey;
        Node* gKey;
        tie(lKey, gKey) = split(root, key);
        int res = INF;
        Node* elem = find_right(lKey);
        if (elem) {
            res = elem->searchKey;
        }
        root = merge(lKey, gKey);
        return res;
    }
};

int main() {
    const int INF = 2e9;
    Tree cartesianTree;
    string operation;
    int key;
    while (cin >> operation >> key) {
        if (operation == "insert") {
            cartesianTree.insert(key);
        } else if (operation == "delete") {
            cartesianTree.del(key);
        } else if (operation == "exists") {
            if (cartesianTree.exist(key)) {
                cout << "true\n";
            } else {
                cout << "false\n";
            }
        } else if (operation == "next") {
            int res = cartesianTree.greater(key);
            if (res == INF) {
                cout << "none\n";
            } else {
                cout << res << '\n';
            }
        } else if (operation == "prev"){
            int res = cartesianTree.less(key);
            if (res == INF) {
                cout << "none\n";
            } else {
                cout << res << '\n';
            }
        }
    }
}