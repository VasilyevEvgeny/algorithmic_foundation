#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

class Solution {
public:
    std::vector<std::vector<int>> build_graph(
            const std::vector<std::string>& wordList) {
        size_t n = wordList.size();
        std::vector<std::vector<int>> result(n);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                int diff = 0;
                for (size_t k = 0; k < wordList[i].size();
                     ++k) {
                    if (wordList[i][k] != wordList[j][k]) {
                        ++diff;
                    }
                }
                if (diff == 1) {
                    result[i].push_back(j);
                    result[j].push_back(i);
                }
            }
        }
        return result;
    }

    int bfs(const std::vector<std::vector<int>>& graph,
            int from, int to) {
        std::vector<int> dist(graph.size());

        std::queue<int> q;
        dist[from] = 1;
        q.push(from);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int w : graph[v]) {
                if (dist[w] == 0) {
                    dist[w] = dist[v] + 1;
                    q.push(w);
                }
            }
        }
        return dist[to];
    }

    int ladderLength(std::string beginWord,
                     std::string endWord, std::vector<std::string>& wordList) {
        auto end_pos = std::find(wordList.begin(),
                            wordList.end(), endWord);
        if (end_pos == wordList.end()) {
            return 0;
        }
        int to = end_pos - wordList.begin();
        wordList.push_back(beginWord);
        return bfs(build_graph(wordList),
                   static_cast<int>(wordList.size()) - 1, to);
    }
};

int main() {

    return 0;
}