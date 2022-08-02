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
        int cntZeoroes;
        int size;
        int pos;
        Node() : left(nullptr), right(nullptr), searchKey(0), heapKey(rnd()),
                 cntZeoroes(1), size(1), pos(0) {}
        Node(int _searchKey, int _pos) : left(nullptr), right(nullptr),
                                         searchKey(_searchKey), heapKey(rnd()), size(1), pos(_pos) {
            if (searchKey == 0) {
                cntZeoroes = 1;
            } else {
                cntZeoroes = 0;
            }
        }
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

    int get_cntZeroes(Node* x) const {
        if (!x) {
            return 0;
        } else {
            return x->cntZeoroes;
        }
    }

    int get_size(Node* x) const {
        if (!x) {
            return 0;
        } else {
            return x->size;
        }
    }

    void relax(Node* root) const {
        if (!root) {
            return;
        }
        root->cntZeoroes = get_cntZeroes(root->left) + get_cntZeroes(root->right);
        if (root->searchKey == 0) {
            ++root->cntZeoroes;
        }
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

    pair<Node*, Node*> split(Node* r, int cnt) {
        if (!r) {
            return { nullptr, nullptr };
        }
        if (get_size(r->left) < cnt) {
            cnt -= (get_size(r->left) + 1);
            Node* leftPart;
            Node* rightPart;
            tie(leftPart, rightPart) = split(r->right, cnt);
            r->right = leftPart;
            relax(r);
            return { r, rightPart };
        } else {
            Node* leftPart;
            Node* rightPart;
            tie(leftPart, rightPart) = split(r->left, cnt);
            r->left = rightPart;
            relax(r);
            return { leftPart, r };
        }
    }

    int get_kth_zero(Node* r, int k) {
        if (!r || get_cntZeroes(r) < k) {
            return -1;
        }
        if (get_cntZeroes(r->left) < k) {
            k -= get_cntZeroes(r->left);
            if (r->searchKey == 0) {
                k--;
            }
            if (k == 0) {
                return r->pos;
            } else {
                return get_kth_zero(r->right, k);
            }
        } else {
            return get_kth_zero(r->left, k);
        }
    }

public:
    Tree() : root(nullptr) {}
    Tree(Node* _root) : root(_root) {}
    ~Tree() {
        delete_dfs(root);
    }

    void update(int pos, int val) {
        Node* leftPart;
        Node* rightPart;
        Node* posNode;
        tie(leftPart, rightPart) = split(root, pos);
        tie(posNode, rightPart) = split(rightPart, 1);
        if (!posNode) {
            posNode = new Node(val, pos);
        } else {
            posNode->searchKey = val;
            relax(posNode);
        }
        root = merge(leftPart, posNode);
        root = merge(root, rightPart);
    }

    int get_kth_zero(int l, int r, int k) {
        Node* leftPart;
        Node* rightPart;
        Node* segPart;
        tie(leftPart, rightPart) = split(root, r + 1);
        tie(leftPart, segPart) = split(leftPart, l);
        int res = get_kth_zero(segPart, k);
        root = merge(leftPart, segPart);
        root = merge(root, rightPart);
        return res;
    }
};

int main() {
    Tree cartesianTree;
    int n;
    cin >> n;
    int cur;
    for (int i = 0; i < n; ++i) {
        cin >> cur;
        cartesianTree.update(i, cur);
    }
    int queries;
    cin >> queries;
    char operation;
    int l, r, k;
    int pos, newVal;
    while (queries--) {
        cin >> operation;
        if (operation == 'u') {
            cin >> pos >> newVal;
            cartesianTree.update(pos - 1, newVal);
        } else {
            cin >> l >> r >> k;
            int res = cartesianTree.get_kth_zero(l - 1, r - 1, k);
            if (res >= 0) {
                ++res;
            }
            cout << res << ' ';
        }
    }
}