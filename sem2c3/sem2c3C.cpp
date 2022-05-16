#include <iostream>
#include <vector>

void dfs(int now, std::vector<std::vector<int>>& edges, std::vector <int>& color,
         bool& isCycle, std::vector<int>& answer, bool& isStop) {
    color[now] = 1;
    for (int i = 0; i < edges[now].size(); ++i) {
        if (color[edges[now][i]] == 1) {
            isCycle = true;
            answer.push_back(edges[now][i]);
        }
        if (color[edges[now][i]] == 2) {
            continue;
        }
        if (color[edges[now][i]] == 0) {
            dfs(edges[now][i], edges, color, isCycle, answer, isStop);
        }
        if (isStop) {
            break;
        }
        if (isCycle) {
            if (answer[0] == now) {
                isStop = true;
            } else {
                answer.push_back(now);
            }
            break;
        }
    }
    color[now] = 2;
}

int main() {
    int vertexAmount, edgesAmount;
    std::cin >> vertexAmount >> edgesAmount;
    std::vector<std::vector<int>> edges(vertexAmount);
    std::vector <int> color(vertexAmount, 0);
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to;
        std::cin >> from >> to;
        edges[from - 1].push_back(to - 1);
    }
    bool isCycle = false;
    bool isStop = false;
    std::vector<int> answer;
    for (int i = 0; i < vertexAmount; ++i) {
        if (color[i] == 0) {
            dfs(i, edges, color, isCycle, answer, isStop);
        }
        if (isCycle) {
            break;
        }
    }
    if (isCycle) {
        std::cout << "YES" << '\n';
        for (int i = static_cast<int>(answer.size()) - 1; i >= 0; --i) {
            std::cout << answer[i] + 1 << " ";
        }
    } else {
        std::cout << "NO" << '\n';
    }
    return 0;
}
