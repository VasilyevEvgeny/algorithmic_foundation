#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

mt19937 rnd(1928);

const int MAX_TIME = 100007;

class Tree {
public:
    class Node {
    public:
        Node* left;
        Node* right;
        int searchKey;
        int heapKey;
        int curTime;
        int minTime;
        Node() : left(nullptr), right(nullptr), searchKey(0), heapKey(rnd()),
                 curTime(MAX_TIME), minTime(MAX_TIME) {}
        Node(int _searchKey, int time) : left(nullptr), right(nullptr),
                                         searchKey(_searchKey), heapKey(rnd()), curTime(time), minTime(time) {}
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

    int get_minTime(Node* x) const {
        if (!x) {
            return MAX_TIME;
        } else {
            return x->minTime;
        }
    }

    void relax(Node* root) const {
        if (!root) {
            return;
        }
        root->minTime = min(root->curTime, min(get_minTime(root->left), get_minTime(root->right)));
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

    void add(int y, int time) {
        Node* lKey;
        Node* eKey;
        Node* gKey;
        Node* curNode = new Node(y, time);
        tie(lKey, gKey) = split(root, y);
        tie(eKey, gKey) = split(gKey, y + 1);
        eKey = merge(eKey, curNode);
        root = merge(lKey, eKey);
        root = merge(root, gKey);
    }

    int get_min_time(int y) {
        Node* leKey;
        Node* gKey;
        tie(leKey, gKey) = split(root, y + 1);
        int res = get_minTime(leKey);
        root = merge(leKey, gKey);
        return res;
    }
};

struct Query {
    int x, y;
    int type;
    int time;
};

int main() {
    Tree cartesianTree;
    int n;
    cin >> n;
    vector<Query> queries(n);
    int type;
    int x, y;
    for (int i = 0; i < n; ++i) {
        cin >> type >> x >> y;
        queries[i] = { x, y, type, i };
    }
    sort(queries.begin(), queries.end(), [](const Query& a, const Query& b) {
        return tie(a.x, a.y, a.type, a.time) < tie(b.x, b.y, b.type, b.time);
    });
    vector<pair<int, string>> ans;
    for (int i = 0; i < n; ++i) {
        auto cur = queries[i];
        if (cur.type == 1) {
            cartesianTree.add(cur.y, cur.time);
        } else {
            int minTime = cartesianTree.get_min_time(cur.y);
            if (minTime < cur.time) {
                ans.push_back({ cur.time, "YES\n" });
            } else {
                ans.push_back({ cur.time, "NO\n" });
            }
        }
    }
    sort(ans.begin(), ans.end());
    for (const auto& x : ans) {
        cout << x.second;
    }
}