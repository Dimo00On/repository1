#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

bool comparator(const std::pair<int, int>& first, const std::pair<int, int>& second) {
    return first.second > second.second;
}

void dfs(int now, std::vector<std::vector<int>>& edges, std::vector<int>& color, bool& isCycle, int& time,
         std::vector<std::pair<int, int>>& tout) {
    color[now] = 1;
    for (int i = 0; i < edges[now].size(); ++i) {
        if (color[edges[now][i]] == 2 || color[edges[now][i]] == 1) {
            continue;
        }
        if (color[edges[now][i]] == 0) {
            dfs(edges[now][i], edges, color, isCycle, time, tout);
        }
    }
    color[now] = 2;
    ++time;
    tout[now].second = time;
}

void dfsR(int now, std::vector<std::vector<int>>& edges, std::vector<int>& color, std::vector<int>& parts, int& count) {
    color[now] = 1;
    for (int i = 0; i < edges[now].size(); ++i) {
        if (color[edges[now][i]] == 0) {
            dfsR(edges[now][i], edges, color, parts, count);
        }
    }
    parts[now] = count;
    color[now] = 2;
}

void findAnswer(std::vector<std::vector<int>>& edges, std::vector<std::vector<int>>& reversedEdges,
                int vertexAmount, int& answer) {
    std::vector<std::pair<int, int>> tout(vertexAmount);
    for (int i = 0; i < vertexAmount; ++i) {
        tout[i].first = i;
        tout[i].second = 0;
    }
    std::vector<int> color(vertexAmount, 0);
    bool isCycle = false;
    int time = 0;
    for (int i = 0; i < vertexAmount; ++i) {
        if (color[i] == 0) {
            dfs(i, edges, color, isCycle, time, tout);
        }
    }
    std::sort(tout.begin(), tout.end(), comparator);
    for (int i = 0; i < vertexAmount; ++i) {
        color[i] = 0;
    }
    int count = 0;
    std::vector<int> parts(vertexAmount);
    for (int i = 0; i < vertexAmount; ++i) {
        if (color[tout[i].first] == 0) {
            ++count;
            dfsR(tout[i].first, reversedEdges, color, parts, count);
        }
    }
    std::set<std::pair<int, int>> uniqueEdges;
    for (int i = 0; i < vertexAmount; ++i) {
        for (int j = 0; j < edges[i].size(); ++j) {
            std::pair<int, int> newEdge = std::make_pair(parts[i], parts[edges[i][j]]);
            if (newEdge.first == newEdge.second) {
                continue;
            }
            if (!uniqueEdges.count(newEdge)) {
                uniqueEdges.insert(newEdge);
                ++answer;
            }
        }
    }
}

int main() {
    int vertexAmount, edgesAmount;
    std::cin >> vertexAmount >> edgesAmount;
    std::vector<std::vector<int>> edges(vertexAmount);
    std::vector<std::vector<int>> reversedEdges(vertexAmount);
    std::set<std::pair<int, int>> uniqueEdges;
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to;
        std::cin >> from >> to;
        if (from == to) {
            continue;
        }
        std::pair<int, int> newEdge = std::make_pair(from, to);
        if (!uniqueEdges.count(newEdge)) {
            uniqueEdges.insert(newEdge);
            edges[from - 1].push_back(to - 1);
            reversedEdges[to - 1].push_back(from - 1);
        }
    }
    int answer = 0;
    findAnswer(edges, reversedEdges, vertexAmount, answer);
    std::cout << answer;
}