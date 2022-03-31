#include <iostream>
#include <vector>

struct Edge {
    int to;
    bool used = false;

    Edge(int end) : to(end) {}
};

void euler(int now, std::vector<int>& queue, std::vector<std::vector<Edge>>& edges) {
    for (int i = 0; i < edges[now].size(); ++i) {
        if (edges[now][i].used) {
            continue;
        }
        edges[now][i].used = true;
        euler(edges[now][i].to, queue, edges);
    }
    queue.push_back(now);
}

void findAnswer(std::vector<std::pair<int, int>>& answer, std::vector<std::vector<Edge>>& edges, int start) {
    std::vector<int> queue;
    euler(start, queue, edges);
    for (size_t i = 1; i < queue.size(); ++i) {
        answer.emplace_back(queue[queue.size() - i], queue[queue.size() - i - 1]);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int vertexAmount, start;
    int edgesAmount = 0;
    std::cin >> vertexAmount >> start;
    if (vertexAmount == 0 || start <= 0) {
        return 0;
    }
    --start;
    std::vector<std::vector<Edge>> edges(vertexAmount);
    for (int i = 0; i < vertexAmount; ++i) {
        for (int j = 0; j < vertexAmount; ++j) {
            int value;
            std::cin >> value;
            if (!value && i != j) {
                edges[i].emplace_back(j);
                ++edgesAmount;
            }
        }
    }
    std::vector<std::pair<int, int>> answer;
    findAnswer(answer, edges, start);
    for (auto & i : answer) {
        std::cout << i.first + 1 << " " << i.second + 1 << '\n';
    }
}
