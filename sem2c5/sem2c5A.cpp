#include <iostream>
#include <vector>

const int kInfinity = 1e9;

int findAnswer(int vertexAmount, int edgesAmount, std::vector<std::vector<int>>& matrix) {
    int answer = 0;
    std::vector<int> dist(vertexAmount, kInfinity);
    std::vector<bool> used(vertexAmount, false);
    used[0] = true;
    for (int i = 0; i < vertexAmount; ++i) {
        dist[i] = matrix[0][i];
    }
    for (int i = 1; i < vertexAmount; ++i) {
        int minDist = kInfinity;
        int index = 0;
        for (int j = 0; j < vertexAmount; ++j) {
            if (!used[j]) {
                if (minDist > dist[j]) {
                    minDist = dist[j];
                    index = j;
                }
            }
        }
        answer += minDist;
        used[index] = true;
        for (int j = 0; j < vertexAmount; ++j) {
            dist[j] = std::min(dist[j], matrix[index][j]);
        }
    }
    return answer;
}

int main() {
    int vertexAmount, edgesAmount;
    std::cin >> vertexAmount >> edgesAmount;
    std::vector<std::vector<int>> matrix(vertexAmount, std::vector<int>(vertexAmount, kInfinity));
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to, mass;
        std::cin >> from >> to >> mass;
        matrix[from - 1][to - 1] = mass;
        matrix[to - 1][from - 1] = mass;
    }
    std::cout << findAnswer(vertexAmount, edgesAmount, matrix);
}
