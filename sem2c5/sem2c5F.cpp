#include <iostream>
#include <vector>

class Solver {
private:
    int vertexAmount;
    std::vector<bool> used;
    std::vector<int> match;
    std::vector<std::vector<int>> edges;
    int answer = 0;

public:
    explicit Solver(int vertexAmount);
    void add(int from, int to);
    int findAnswer();
    bool augment(int vertex);
};

Solver::Solver(int vertexAmount) : vertexAmount(vertexAmount),
    used(vertexAmount, false), match(vertexAmount, -1), edges(vertexAmount) {}


void Solver::add(int from, int to) {
    --from;
    --to;
    edges[from].push_back(to);
}

bool Solver::augment(int vertex) {
    if (used[vertex]) {
        return false;
    }
    used[vertex] = true;
    for (int to : edges[vertex]) {
        if (match[to] == -1 || augment(match[to])) {
            match[to] = vertex;
            return true;
        }
    }
    return false;
}

int Solver::findAnswer() {
    for (int i = 0; i < vertexAmount; ++i) {
        if (augment(i))  {
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
    Solver solver(vertexAmount);
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to;
        std::cin >> from >> to;
        solver.add(from, to);
    }
    std::cout << solver.findAnswer();
}
