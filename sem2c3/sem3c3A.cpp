#include <iostream>
#include <vector>

int main() {
    int vertexAmount, edgeAmount;
    std::cin >> vertexAmount >> edgeAmount;
    std::vector<std::vector<int>> answer(vertexAmount, std::vector<int>(edgeAmount, 0));
    for (int i = 0; i < edgeAmount; ++i) {
        int vertex1, vertex2;
        std::cin >> vertex1 >> vertex2;
        answer[vertex1 - 1][i] = 1;
        answer[vertex2 - 1][i] = 1;
    }
    for (int i = 0; i < vertexAmount; ++i) {
        for (int j = 0; j < edgeAmount; ++j) {
            std::cout << answer[i][j] << " ";
        }
        std::cout << '\n';
    }
    return 0;
}
