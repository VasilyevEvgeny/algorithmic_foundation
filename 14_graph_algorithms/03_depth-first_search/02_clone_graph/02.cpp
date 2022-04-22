#include <vector>


class Node {
public:
    int val;
    std::vector<Node*> neighbors;

    Node() {
        val = 0;
        neighbors = std::vector<Node*>();
    }

    Node(int _val) {
        val = _val;
        neighbors = std::vector<Node*>();
    }

    Node(int _val, std::vector<Node*> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
};

class Solution {
public:
    Solution()
            : n_(101), nodes_(std::vector<Node*>(n_, nullptr)) {}


    Node* dfs(Node* node) {
        int val = node->val;
        if (nodes_[val]) { return nodes_[val]; }
        else {
            nodes_[val] = new Node;
            nodes_[val]->val = val;

            for (Node* neighbor : node->neighbors) {
                nodes_[val]->neighbors.push_back(dfs(neighbor));
            }

            return nodes_[val];
        }

    }

    Node* cloneGraph(Node* node) {
        if (!node) { return nullptr; }
        else       { return dfs(node); }
    }

private:
    const size_t n_;
    std::vector<Node*> nodes_;

};

int main() {

    return 0;
}