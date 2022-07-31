#include <iostream>
#include <memory>
#include <vector>

std::ostream& operator<< (std::ostream& os, const std::vector<int>& v) {
    for (const auto& e : v) {
        os << e << " ";
    }

    return os;
}

struct TNode {
    TNode(int value) : value_(value) {}

    int value_;
    std::unique_ptr<TNode> left_;
    std::unique_ptr<TNode> right_;
};

std::unique_ptr<TNode> Add(std::unique_ptr<TNode> tree, int value) {
    if (!tree) {
        return std::make_unique<TNode>(value);
    }

    if (value < tree->value_) {
        tree->left_ = Add(std::move(tree->left_), value);
    } else {
        tree->right_ = Add(std::move(tree->right_), value);
    }

    return std::move(tree);
}

void PreOrderImpl(TNode* tree, std::vector<int>& result) {
    if (!tree) {
        return;
    }

    result.push_back(tree->value_);
    PreOrderImpl(tree->left_.get(), result);
    PreOrderImpl(tree->right_.get(), result);
}

void PreOrder(TNode* tree) {
    std::vector<int> result;
    PreOrderImpl(tree, result);
    std::cout << result << std::endl;
}


void InOrderImpl(TNode* tree, std::vector<int>& result) {
    if (!tree) {
        return;
    }

    InOrderImpl(tree->left_.get(), result);
    result.push_back(tree->value_);
    InOrderImpl(tree->right_.get(), result);
}

void InOrder(TNode* tree) {
    std::vector<int> result;
    InOrderImpl(tree, result);
    std::cout << result << std::endl;
}

void PostOrderImpl(TNode* tree, std::vector<int>& result) {
    if (!tree) {
        return;
    }

    PostOrderImpl(tree->left_.get(), result);
    PostOrderImpl(tree->right_.get(), result);
    result.push_back(tree->value_);
}

void PostOrder(TNode* tree) {
    std::vector<int> result;
    PostOrderImpl(tree, result);
    std::cout << result << std::endl;
}


int main() {

    size_t n = 0;
    std::cin >> n;
    std::vector<int> numbers(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> numbers[i];
    }

    std::unique_ptr<TNode> tree;
    for (const auto& e : numbers) {
        tree = Add(std::move(tree), e);
    }

    PreOrder(tree.get());
    InOrder(tree.get());
    PostOrder(tree.get());

    return 0;
}