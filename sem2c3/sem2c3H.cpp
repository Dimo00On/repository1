#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

void dfs(int now, int prev, std::vector<int>& tin, std::vector<int>& ret, int& time,
         std::vector<std::vector<int>>& edges, std::vector<int>& color,
         std::vector<std::pair<int, int>>& answer) {
    ++time;
    tin[now] = time;
    ret[now] = time;
    color[now] = 1;
    for (int i = 0; i < edges[now].size(); ++i) {
        int to = edges[now][i];
        if (to == prev) {
            continue;
        }
        if (color[to] == 1) {
            ret[now] = std::min(ret[now], tin[to]);
        } else {
            dfs(to, now, tin, ret, time, edges, color, answer);
            ret[now] = std::min(ret[now], ret[to]);
            if (ret[to] == tin[to]) {
                answer.emplace_back(now, to);
            }
        }
    }
}

void findAnswer(std::vector<std::pair<int, int>>& answer, int vertexAmount, int edgesAmount,
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
    std::map<std::pair<int, int>, int> index;
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to;
        std::cin >> from >> to;
        --from;
        --to;
        index.insert(std::make_pair(std::make_pair(from, to), i));
        index.insert(std::make_pair(std::make_pair(to, from), i));
        edges[from].push_back(to);
        edges[to].push_back(from);
    }
    std::vector<std::pair<int, int>> answer;
    findAnswer(answer, vertexAmount, edgesAmount, edges);
    std::cout << answer.size() << '\n';
    std::vector<int> indexes(answer.size());
    for (int i = 0; i < answer.size(); ++i) {
        indexes[i] = index[std::make_pair(answer[i].first, answer[i].second)] + 1;
    }
    sort(indexes.begin(), indexes.end());
    for (int i : indexes) {
        std::cout << i << " ";
    }
}