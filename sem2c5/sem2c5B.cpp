#include <iostream>
#include <vector>
#include <algorithm>

const int kInfinity = 1e9;

struct Edge {
    int from;
    int to;
    int mass;
    Edge(int from, int to, int mass) : from(from), to(to), mass(mass) {};
};

bool comparator(const Edge& first, const Edge& second) {
    return first.mass < second.mass;
}

int get(int vertex, std::vector<int>& prev) {
    if (prev[vertex] == -1) {
        return vertex;
    }
    prev[vertex] = get(prev[vertex], prev);
    return prev[vertex];
}

void unite(int first, int second, std::vector<int>& size, std::vector<int>& prev) {
    first = get(first, prev);
    second = get(second, prev);
    if (size[first] > size[second]) {
        prev[second] = first;
        size[first] += size[second];
        return;
    }
    prev[first] = second;
    size[second] += size[first];
}

int findAnswer(int vertexAmount, int edgesAmount, std::vector<Edge>& edges) {
    int answer = 0;
    std::vector<int> size(vertexAmount, 1);
    std::vector<int> prev(vertexAmount, -1);
    std::sort(edges.begin(), edges.end(), comparator);
    for (Edge& edge : edges) {
        int from = get(edge.from, prev);
        int to = get(edge.to, prev);
        if (from != to) {
            answer += edge.mass;
            unite(from, to, size, prev);
        }
    }
    return answer;
}

int main() {
    int vertexAmount, edgesAmount;
    std::cin >> vertexAmount >> edgesAmount;
    std::vector<Edge> edges;
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to, mass;
        std::cin >> from >> to >> mass;
        edges.emplace_back(from - 1, to - 1, mass);
    }
    std::cout << findAnswer(vertexAmount, edgesAmount, edges);
}
