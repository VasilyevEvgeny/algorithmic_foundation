#include <iostream>
#include <memory>
#include <cmath>
#include <vector>

#define VERBOSE false

std::ostream& operator<< (std::ostream& os, const std::vector<int>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i < v.size() - 1) {
            os << " ";
        }
    }

    return os;
}

struct TNode {
    TNode() = default;
    TNode(int key, int parent_key)
            : key_(key)
            , parent_key_(parent_key)
    {}

    int parent_key_ = 0;
    int key_ = 0;
    std::unique_ptr<TNode> parent_;
    std::unique_ptr<TNode> left_;
    std::unique_ptr<TNode> right_;
};

std::vector<int> Contains(TNode* tree, int key) {
    if (!tree) {
        return {-10};
    } else if (key == tree->key_) {
        std::vector<int> res;
        res.push_back(tree->parent_key_);
        if (!tree->left_) {
            res.push_back(-1);
        } else {
            res.push_back(tree->left_->key_);
        }
        if (!tree->right_) {
            res.push_back(-1);
        } else {
            res.push_back(tree->right_->key_);
        }
        return res;
    } else if (key < tree->key_) {
        return Contains(tree->left_.get(), key);
    } else {
        return Contains(tree->right_.get(), key);
    }
}

std::unique_ptr<TNode> Add(std::unique_ptr<TNode> tree,
                           int key, int parent_key) {

    if (!tree) {
        if (VERBOSE) {
            std::cout << "Add " << key << std::endl;
        }
        return std::make_unique<TNode>(key, parent_key);
    }

    if (key < tree->key_) {
        if (VERBOSE) {
            std::cout << "Choose left in node with key = " << tree->key_ << std::endl;
        }
        tree->left_ = Add(std::move(tree->left_), key, tree->key_);
    } else {
        if (VERBOSE) {
            std::cout << "Choose right in node with key = " << tree->key_ << std::endl;
        }
        tree->right_ = Add(std::move(tree->right_), key, tree->key_);
    }

    return std::move(tree);
}



int main() {

    size_t n = 0;
    std::cin >> n;
    std::unique_ptr<TNode> tree;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < std::pow(2, i); ++j) {
            int key = 0;
            std::cin >> key;
            tree = Add(std::move(tree), key, -1);
        }
    }

    size_t m = 0;
    std::cin >> m;
    std::vector<int> queries(m);
    for (size_t i = 0; i < m; ++i) {
        std::cin >> queries[i];
    }

    for (size_t i = 0; i < m; ++i) {
        auto res = Contains(tree.get(), queries[i]);
        std::cout << res << std::endl;
    }

    return 0;
}