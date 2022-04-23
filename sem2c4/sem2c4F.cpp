#include <iostream>
#include <vector>
#include <queue>

const int kMaxSize = 101;
const int kInfinity = 1e9;

struct Edge{
    int to;
    int mass;
    Edge(int newTo, int newMass) : to(newTo), mass(newMass) {};
};

int findAnswer(std::vector<std::vector<Edge>>& edges, int vertexAmount, int start, int end) {
    int answer = -1;
    std::vector<std::queue<int>> queues(kMaxSize);
    std::vector<bool> used(vertexAmount, false);
    std::vector<int> distance(vertexAmount, kInfinity);
    distance[start] = 0;
    queues[0].push(start);
    bool stop = false;
    for (int j = 0; j < vertexAmount; ++j) {
        int index = 0;
        while (index < kMaxSize && !stop) {
            while (!queues[index].empty()) {
                int newVertex = queues[index].front();
                if (newVertex == end) {
                    answer = distance[end];
                    stop = true;
                    break;
                }
                queues[index].pop();
                if (used[newVertex]) {
                    continue;
                }
                used[newVertex] = true;
                for (Edge &edge: edges[newVertex]) {
                    if (distance[edge.to] > distance[newVertex] + edge.mass) {
                        distance[edge.to] = distance[newVertex] + edge.mass;
                        queues[distance[edge.to] % kMaxSize].push(edge.to);
                    }
                }
            }
            ++index;
        }
        if (stop) {
            break;
        }
    }
    return answer;
}

int main() {
    int vertexAmount, edgesAmount;
    std::cin >> vertexAmount >> edgesAmount;
    int start, end;
    std::cin >> start >> end;
    --start;
    --end;
    std::vector<std::vector<Edge>> edges(vertexAmount);
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        edges[from - 1].emplace_back(to - 1, weight);
        edges[to - 1].emplace_back(from - 1, weight);
    }
    std::cout << findAnswer(edges, vertexAmount, start, end);
}
