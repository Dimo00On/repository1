#include <iostream>
#include <vector>
#include <algorithm>

bool comparator(const std::pair<int, int>& first, const std::pair<int, int>& second) {
    return first.second > second.second;
}

void dfs(int now, std::vector<std::vector<int>>& edges, std::vector<int>& color, int& time,
         std::vector<std::pair<int, int>>& tout) {
    color[now] = 1;
    for (int i = 0; i < edges[now].size(); ++i) {
        if (color[edges[now][i]] == 2 || color[edges[now][i]] == 1) {
            continue;
        }
        if (color[edges[now][i]] == 0) {
            dfs(edges[now][i], edges, color, time, tout);
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

void condensation(std::vector<std::vector<int>>& edges, std::vector<std::vector<int>>& reversedEdges,
                  int vertexAmount, int& count, std::vector<int>& parts) {
    std::vector<std::pair<int, int>> tout(vertexAmount);
    for (int i = 0; i < vertexAmount; ++i) {
        tout[i].first = i;
        tout[i].second = 0;
    }
    std::vector<int> color(vertexAmount, 0);
    int time = 0;
    for (int i = 0; i < vertexAmount; ++i) {
        if (color[i] == 0) {
            dfs(i, edges, color, time, tout);
        }
    }
    std::sort(tout.begin(), tout.end(), comparator);
    for (int i = 0; i < vertexAmount; ++i) {
        color[i] = 0;
    }
    for (int i = 0; i < vertexAmount; ++i) {
        if (color[tout[i].first] == 0) {
            dfsR(tout[i].first, reversedEdges, color, parts, count);
            ++count;
        }
    }
}

bool isFoundAnswer(int vertexAmount, int edgesAmount, std::vector<int>& status,
                   std::vector<std::vector<int>>& bulbsControl) {
    std::vector<std::vector<int>> edges(2 * vertexAmount);
    std::vector<std::vector<int>> reversedEdges(2 * vertexAmount);
    for (int i = 0; i < edgesAmount; ++i) {
        if (status[i]) {
            edges[bulbsControl[i][0]].push_back(bulbsControl[i][1]);
            edges[bulbsControl[i][1]].push_back(bulbsControl[i][0]);
            edges[bulbsControl[i][0] + vertexAmount].push_back(bulbsControl[i][1] + vertexAmount);
            edges[bulbsControl[i][1] + vertexAmount].push_back(bulbsControl[i][0] + vertexAmount);
        } else {
            edges[bulbsControl[i][0]].push_back(bulbsControl[i][1] + vertexAmount);
            edges[bulbsControl[i][1]].push_back(bulbsControl[i][0] + vertexAmount);
            edges[bulbsControl[i][0] + vertexAmount].push_back(bulbsControl[i][1]);
            edges[bulbsControl[i][1] + vertexAmount].push_back(bulbsControl[i][0]);
        }
    }
    int count = 0;
    std::vector<int> parts(2 * vertexAmount);
    condensation(edges, edges, 2 * vertexAmount, count, parts);
    for (int i = 0; i < vertexAmount; ++i) {
        if (parts[i] == parts[i + vertexAmount]) {
            return false;
        }
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int edgesAmount, vertexAmount;
    std::cin >> edgesAmount >> vertexAmount;
    std::vector<int> status(edgesAmount);
    for (int i = 0; i < edgesAmount; ++i) {
        std::cin >> status[i];
    }
    std::vector<std::vector<int>> bulbsControl(edgesAmount);
    for (int i = 0; i < vertexAmount; ++i) {
        int amount;
        std::cin >> amount;
        for (int j = 0; j < amount; ++j) {
            int value;
            std::cin >> value;
            --value;
            bulbsControl[value].push_back(i);
        }
    }
    if (isFoundAnswer(vertexAmount, edgesAmount, status, bulbsControl)) {
        std::cout << "YES" << '\n';
    } else {
        std::cout << "NO" << '\n';
    }
}
