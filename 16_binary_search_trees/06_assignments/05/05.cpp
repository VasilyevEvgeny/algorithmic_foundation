#include <iostream>
#include <set>
#include <stack>
#include <vector>

using namespace std;

class Tree {
public:
    class Node {
    public:
        Node* left;
        Node* right;
        int key;
        Node() : left(nullptr), right(nullptr), key(0) {}
        Node(int _key) : left(nullptr), right(nullptr), key(_key) {}
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

    void get_postorder(Node* v, vector<int>& postorder) const {
        if (v->left) {
            get_postorder(v->left, postorder);
        }
        if (v->right) {
            get_postorder(v->right, postorder);
        }
        postorder.push_back(v->key);
        return;
    }

public:
    Tree() : root(nullptr) {}
    Tree(Node* _root) : root(_root) {}
    ~Tree() {
        delete_dfs(root);
    }

    Tree& operator=(const Tree& r) {
        if (this == &r) {
            return *this;
        }
        root = r.root;
        return *this;
    }

    void get_postorder(vector<int>& res) const {
        res.clear();
        if (!root) {
            return;
        }
        get_postorder(root, res);
        return;
    }

    void build_tree(vector<int>& pre, vector<int>& in) {
        stack<Node*> st;
        Node* last_r = nullptr;
        size_t Ind = 0;
        size_t pInd = 0;
        while (Ind < in.size()) {
            if (!st.empty() && st.top()->key == in[Ind]) {
                last_r = st.top();
                st.pop();
                ++Ind;
                continue;
            }
            size_t startInd = pInd;
            while (pInd < pre.size() && pre[pInd] != in[Ind]) {
                ++pInd;
            }
            while (startInd <= pInd) {
                if (last_r) {
                    last_r->right = new Node(pre[startInd]);
                    st.push(last_r->right);
                    last_r = nullptr;
                } else {
                    if (st.empty()) {
                        root = new Node(pre[startInd]);
                        st.push(root);
                    } else {
                        st.push(st.top()->left = new Node(pre[startInd]));
                    }
                }
                ++startInd;
            }
            last_r = st.top();
            st.pop();
            ++pInd;
            ++Ind;
        }
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> preorder(n);
    vector<int> inorder(n);
    for (int& x : preorder) {
        cin >> x;
    }
    for (int& x : inorder) {
        cin >> x;
    }
    Tree tree;
    tree.build_tree(preorder, inorder);
    vector<int> res;
    tree.get_postorder(res);
    for (int& e : res) {
        cout << e << ' ';
    }
}