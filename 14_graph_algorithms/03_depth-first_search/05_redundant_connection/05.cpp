//
// Created by evasilyev on 08.06.2020.
//

#include <iostream>
#include <vector>


class Solution {
public:

    int n, best_edge, last_vertex;
    std::vector<bool> used;
    std::vector<std::vector<int>> g, num;

    bool dfs(int x, int prev) {
        used[x] = true;
        for (size_t i = 0; i < g[x].size(); ++i) {
            if (g[x][i] != prev) {
                if (used[g[x][i]]) {
                    last_vertex = g[x][i];
                    best_edge = num[x][i];
                    return true;
                }
                if (dfs(g[x][i], x)) {
                    if (best_edge < num[x][i]) {
                        best_edge = num[x][i];
                    }
                    return x != last_vertex;
                }
            }
        }
        return false;
    }

    std::vector<int> findRedundantConnection(std::vector<std::vector<int>>& edges) {
        n = edges.size();
        used = std::vector<bool>(n, false);
        g = std::vector<std::vector<int>>(n, std::vector<int>());
        num = std::vector<std::vector<int>>(n, std::vector<int>());
        for (size_t i = 0; i < edges.size(); ++i) {
            int from = edges[i][0] - 1;
            int to = edges[i][1] - 1;
            g[from].push_back(to);
            g[to].push_back(from);
            num[from].push_back(i);
            num[to].push_back(i);
        }
        dfs(0, -1);
        return edges[best_edge];
    }
};


int main() {

    return 0;
}