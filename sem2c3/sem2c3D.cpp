#include <iostream>
#include <vector>
#include <algorithm>

void dfs(int now, std::vector<std::vector<int>>& edges, std::vector<int>& color, bool& isCycle,
         std::vector<int>& answer) {
    color[now] = 1;
    for (int i = 0; i < edges[now].size(); ++i) {
        if (color[edges[now][i]] == 1) {
            isCycle = true;
        }
        if (color[edges[now][i]] == 2) {
            continue;
        }
        if (color[edges[now][i]] == 0) {
            dfs(edges[now][i], edges, color, isCycle, answer);
        }
        if (isCycle) {
            break;
        }
    }
    color[now] = 2;
    answer.push_back(now);
}

int main() {
    int vertexAmount, edgesAmount;
    std::cin >> vertexAmount >> edgesAmount;
    std::vector<std::vector<int>> edges(vertexAmount);
    std::vector<int> color(vertexAmount, 0);
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to;
        std::cin >> from >> to;
        edges[from - 1].push_back(to - 1);
    }
    bool isCycle = false;
    std::vector<int> answer;
    for (int i = 0; i < vertexAmount; ++i) {
        if (color[i] == 0) {
            dfs(i, edges, color, isCycle, answer);
        }
        if (isCycle) {
            break;
        }
    }
    if (isCycle) {
        std::cout << -1;
    } else {
        std::reverse(answer.begin(), answer.end());
        for (auto i : answer) {
            std::cout << i + 1 << " ";
        }
    }
}