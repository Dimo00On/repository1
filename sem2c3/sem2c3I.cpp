#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

void dfs(int now, int prev, std::vector<int>& tin, std::vector<int>& ret, int& time,
         std::vector<std::vector<int>>& edges, std::vector<int>& color,
         std::set<int>& answer) {
    ++time;
    tin[now] = time;
    ret[now] = time;
    color[now] = 1;
    int count = 0;
    for (int i = 0; i < edges[now].size(); ++i) {
        int to = edges[now][i];
        if (to == prev || to == now) {
            continue;
        }
        if (color[to] == 1) {
            ret[now] = std::min(ret[now], tin[to]);
        } else {
            ++count;
            dfs(to, now, tin, ret, time, edges, color, answer);
            ret[now] = std::min(ret[now], ret[to]);
            if (ret[to] >= tin[now] && prev != -1) {
                answer.insert(now);
            }
        }
    }
    if (prev == -1 && count >= 2) {
        answer.insert(now);
    }
}

void findAnswer(std::set<int>& answer, int vertexAmount, int edgesAmount,
                std::vector<std::vector<int>>& edges) {
    std::vector<int> tin(vertexAmount);
    std::vector<int> ret(vertexAmount);
    std::vector<int> color(vertexAmount, 0);
    int time = 0;
    for (int i = 0; i < vertexAmount; ++i) {
        if (color[i] == 0) {
            dfs(i, -1, tin, ret, time, edges, color, answer);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int vertexAmount, edgesAmount;
    std::cin >> vertexAmount >> edgesAmount;
    std::vector<std::vector<int>> edges(vertexAmount);
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to;
        std::cin >> from >> to;
        --from;
        --to;
        edges[from].push_back(to);
        edges[to].push_back(from);
    }
    std::set<int> answer;
    findAnswer(answer, vertexAmount, edgesAmount, edges);
    std::cout << answer.size() << '\n';
    for (int i : answer) {
        std::cout << i + 1 << " ";
    }
}