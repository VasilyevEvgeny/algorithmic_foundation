#include <iostream>
#include <vector>
#include <algorithm>

class Solution {
public:

    enum class Color {
        White, Gray, Black
    };

    std::vector<std::vector<int>> g;
    std::vector<int> ans;
    bool impossible = false;
    std::vector<Color> color;

    void dfs(int x) {
        if (color[x] == Color::Gray) {
            impossible = true;
            return;
        }
        if (color[x] == Color::Black) {
            return;
        }
        color[x] = Color::Gray;
        for (int w : g[x]) {
            dfs(w);
        }
        color[x] = Color::Black;
        ans.push_back(x);
    }

    std::vector<int> findOrder(int numCourses,
                          std::vector<std::vector<int>>& prerequisites) {
        g = std::vector<std::vector<int>>(numCourses, std::vector<int>());
        color = std::vector<Color>(numCourses, Color::White);
        for (const std::vector<int>& p : prerequisites) {
            int from = p[1];
            int to = p[0];
            g[from].push_back(to);
        }
        for (size_t i = 0; i < numCourses; ++i) {
            dfs(i);
        }
        if (impossible) {
            ans.clear();
        }
        std::reverse(ans.begin(), ans.end());
        return ans;
    }
};

int main() {

    return 0;
}