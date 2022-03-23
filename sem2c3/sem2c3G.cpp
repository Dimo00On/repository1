#include <iostream>
#include <vector>
#include <algorithm>

void dfs(int now, std::vector<std::vector<int>>& edges, std::vector<int>& color,
          std::vector<int>& parts, int& number) {
    color[now] = 1;
    for (int i = 0; i < edges[now].size(); ++i) {
        if (color[edges[now][i]] == 0) {
            dfs(edges[now][i], edges, color, parts, number);
        }
    }
    parts[now] = number;
    color[now] = 2;
}

void findAnswer(std::vector<std::vector<int>>& edges,  std::vector<int>& parts,
                int vertexAmount, int& answer) {
    std::vector<int> color(vertexAmount, 0);
    for (int i = 0; i < vertexAmount; ++i) {
        if (color[i] == 0) {
            ++answer;
            dfs(i, edges, color, parts, answer);
        }
    }

}

int main() {
    int vertexAmount, edgesAmount;
    std::cin >> vertexAmount >> edgesAmount;
    std::vector<std::vector<int>> edges(vertexAmount);
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to;
        std::cin >> from >> to;
        edges[from - 1].push_back(to - 1);
        edges[to - 1].push_back(from - 1);
    }
    int answer = 0;
    std::vector<int> parts(vertexAmount);
    findAnswer(edges, parts, vertexAmount, answer);
    std::cout << answer << '\n';
    for (int i = 0; i < vertexAmount; ++i) {
        std::cout << parts[i] << " ";
    }
}