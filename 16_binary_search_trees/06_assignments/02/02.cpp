#include <iostream>
#include <vector>
#include <memory>
#include <string>

#define VERBOSE false

std::ostream& operator<< (std::ostream& os, const std::vector<int>& v) {
    for (const auto& e : v) {
        os << e << " ";
    }

    return os;
}

struct TNode {
    int key_;
    TNode* left_;
    TNode* right_;
};

void InOrderImpl(TNode* tree, std::vector<int>& result) {
    if (!tree) {
        return;
    }

    InOrderImpl(tree->left_, result);
    result.push_back(tree->key_);
    InOrderImpl(tree->right_, result);
}

std::vector<int> InOrder(TNode* tree) {
    std::vector<int> result;
    InOrderImpl(tree, result);

    return result;
}



int main() {

    size_t n = 0;
    std::cin >> n;

    std::vector<TNode> nodes(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> nodes[i].key_;
    }

    for (size_t i = 0; i < n; ++i) {
        size_t l = 0, r = 0;
        std::cin >> l >> r;
        if (l) {
            nodes[i].left_ = &nodes[l - 1];
        }
        if (r) {
            nodes[i].right_ = &nodes[r - 1];
        }

    }

    std::string res = "NO";
    for (size_t i = 0; i < n; ++i) {
        auto result = InOrder(&nodes[i]);
        if (VERBOSE) {
            std::cout << result << std::endl;
        }
        if (result.size() == n) {
            bool is_ok = true;
            for (size_t j = 1; j < n; ++j) {
                if (result[j] < result[j - 1]) {
                    is_ok = false;
                    break;
                }
            }
            if (is_ok) {
                res = "YES";
            }
        }
    }

    std::cout << res << std::endl;

    return 0;
}