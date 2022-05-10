#include <iostream>
#include <vector>

bool augment(int vertex, int vertexAmount, std::vector<bool>& used, std::vector<int>& match,
             std::vector<std::vector<int>>& edges) {
    if (used[vertex]) {
        return false;
    }
    used[vertex] = true;
    for (int to : edges[vertex]) {
        if (match[to] == -1 || augment(match[to], vertexAmount, used, match, edges)) {
            match[to] = vertex;
            return true;
        }
    }
    return false;
}

int findAnswer(int vertexAmount, int edgesAmount,
               std::vector<std::vector<int>>& edges) {
    std::vector<bool> used(vertexAmount, false);
    std::vector<int> match(vertexAmount, -1);
    for (int i = 0; i < vertexAmount; ++i) {
        if (augment(i, vertexAmount, used, match, edges))  {
            for (int j = 0; j < vertexAmount; ++j) {
                used[j] = false;
            }
        }
    }
    edges.clear();
    edges.resize(vertexAmount);
    for (int i = 0; i < vertexAmount; ++i) {
        if (match[i] != -1) {
            edges[i].push_back(match[i]);
        }
    }
    int answer = 0;
    for (int i = 0; i < vertexAmount; ++i) {
        if (edges[i].empty()) {
            ++answer;
        }
    }
    return answer;
}

int main() {
    int vertexAmount, edgesAmount;
    std::cin >> vertexAmount >> edgesAmount;
    std::vector<std::vector<int>> edges(vertexAmount);
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to;
        std::cin >> from >> to;
        --from;
        --to;
        edges[from].push_back(to);
    }
    std::cout << findAnswer(vertexAmount, edgesAmount, edges);
}
