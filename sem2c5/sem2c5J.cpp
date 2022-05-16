#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

const int kInfinity = 1e9;

struct Edge {
    int from;
    int to;
    int cap;
    int flow;
    Edge* back;
    ~Edge() {};
    Edge(int from, int to, int cap) : from(from), to(to), cap(cap), flow(0), back(nullptr) {};

};

int dfsFlow(int vertex, int capMin, std::vector<bool>& used,
        std::vector<std::vector<Edge*>>& edges, int end) {
    used[vertex] = true;
    if (vertex == end) {
        return capMin;
    }
    for (auto& edge : edges[vertex]) {
        if (!used[edge->to] && edge->cap > edge->flow) {
            int delta = dfsFlow(edge->to, std::min(capMin, edge->cap - edge->flow), used, edges, end);
            if (delta > 0) {
                edge->flow += delta;
                edge->back->flow -= delta;
                return delta;
            }
        }
    }
    return 0;
}

void dfs(int vertex, std::vector<std::vector<Edge*>>& edges, int end,
         std::vector<int>& path, bool& isFound, std::vector<bool>& used) {
    used[vertex] = true;
    path.push_back(vertex);
    if (vertex == end) {
        isFound = true;
        return;
    }
    for (auto& edge : edges[vertex]) {
        if (edge->flow > 0 && !used[edge->to]) {
            dfs(edge->to, edges, end, path, isFound, used);
            if (isFound) {
                edge->flow -= 1;
                return;
            }
        }
    }
    path.pop_back();
}

std::string findAnswer(int vertexAmount, int start, int end,
                       std::vector<std::vector<Edge*>>& edges, std::vector<int>& goatPath,
                       std::vector<int>& shrexPath) {
    std::vector<bool> used(vertexAmount, false);
    int pushed = dfsFlow(start, kInfinity, used, edges, end);
    if (pushed == 0) {
        return "NO";
    }
    if (pushed == 1) {
        for (int i = 0; i < vertexAmount; ++i) {
            used[i] = false;
        }
        pushed = dfsFlow(start, kInfinity, used, edges, end);
        if (pushed == 0) {
            return "NO";
        }
    }
    bool isFound = false;
    for (int i = 0; i < vertexAmount; ++i) {
        used[i] = false;
    }
    dfs(start, edges, end, goatPath, isFound, used);
    isFound = false;
    for (int i = 0; i < vertexAmount; ++i) {
        used[i] = false;
    }
    dfs(start, edges, end, shrexPath, isFound, used);
    return "YES";
}

int main() {
    int vertexAmount, edgesAmount, start, end;
    std::cin >> vertexAmount >> edgesAmount >> start >> end;
    --start;
    --end;
    std::vector<std::vector<Edge*>> edges(vertexAmount);
    std::vector<std::unordered_map<int, Edge*>> unique(vertexAmount);
    int useless = 0;
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to;
        int cap = 1;
        std::cin >> from >> to;
        if (from == to) {
            ++useless;
            continue;
        }
        --from;
        --to;
        if (unique[from].count(to)) {
            unique[from][to]->cap += cap;
        } else {
            auto newEdge = new Edge(from, to, cap);
            unique[from].insert(std::make_pair(to, newEdge));
            auto newEdgeBack = new Edge(to, from, 0);
            unique[to].insert(std::make_pair(from, newEdgeBack));
            newEdge->back = newEdgeBack;
            newEdgeBack->back = newEdge;
            edges[from].push_back(newEdge);
            edges[to].push_back(newEdgeBack);
        }
    }
    edgesAmount -= useless;
    unique.clear();
    std::vector<int> goatPath;
    std::vector<int> shrexPath;
    std::string answer = findAnswer(vertexAmount, start, end, edges, goatPath, shrexPath);
    std::cout << answer << '\n';
    if (answer == "YES") {
        for (auto i : goatPath) {
            std::cout << i + 1 << " ";
        }
        std::cout << '\n';
        for (auto i : shrexPath) {
            std::cout << i + 1 << " ";
        }
    }
    for (int i = 0; i < vertexAmount; ++i) {
        for (auto& j : edges[i]) {
            delete j;
        }
    }
}
