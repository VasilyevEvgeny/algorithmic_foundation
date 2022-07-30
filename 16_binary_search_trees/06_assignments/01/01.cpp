#include <iostream>
#include <memory>
#include <vector>

#define VERBOSE true

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v) {
    for (const auto& e : v) {
        os << e << " ";
    }

    return os;
}

struct TNode {
    int number_;
    TNode* left_;
    TNode* right_;
};

void PreOrderImpl(TNode* tree, std::vector<int>& result) {
    if (!tree) {
        return;
    }
    result.push_back(tree->number_);
    PreOrderImpl(tree->left_, result);
    PreOrderImpl(tree->right_, result);
}

std::vector<int> PreOrder(TNode& tree) {
    std::vector<int> result;
    PreOrderImpl(&tree, result);

    return result;
}

void InOrderImpl(TNode* tree, std::vector<int>& result) {
    if (!tree) {
        return;
    }
    InOrderImpl(tree->left_, result);
    result.push_back(tree->number_);
    InOrderImpl(tree->right_, result);
}

std::vector<int> InOrder(TNode& tree) {
    std::vector<int> result;
    InOrderImpl(&tree, result);

    return result;
}

void PostOrderImpl(TNode* tree, std::vector<int>& result) {
    if (!tree) {
        return;
    }
    PostOrderImpl(tree->left_, result);
    PostOrderImpl(tree->right_, result);
    result.push_back(tree->number_);
}

std::vector<int> PostOrder(TNode& tree) {
    std::vector<int> result;
    PostOrderImpl(&tree, result);

    return result;
}


int main() {

    size_t n = 0;
    std::cin >> n;

    std::vector<TNode> tree(n);
    for (size_t i = 0; i < n; ++i) {
        tree[i].number_ = i + 1;
        int l = 0, r = 0;
        std::cin >> l >> r;
        if (l) {
            tree[i].left_ = &tree[l - 1];
        }
        if (r) {
            tree[i].right_ = &tree[r - 1];
        }
    }

    int max_len = 0;
    int i_max = 0;
    for (size_t i = 0; i < n; ++i) {
        auto result = InOrder(tree[i]);
        if (result.size() > max_len) {
            max_len = result.size();
            i_max = i;
        }
    }

    std::cout << PreOrder(tree[i_max]) << std::endl;
    std::cout << InOrder(tree[i_max]) << std::endl;
    std::cout << PostOrder(tree[i_max]) << std::endl;

    return 0;
}