#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

const int kAlphabetStart = 97;
const int kMagicSizeOfChessBoard = 64;
const int kMagicLineSize = 8;

void addVertex(std::vector<int>& parent, std::queue<int>& vertexes, std::vector<bool>& used, int vertex, int from) {
    if (used[vertex]) {
        return;
    }
    used[vertex] = true;
    parent[vertex] = from;
    vertexes.push(vertex);
}

void findAnswer(std::vector<int>& answer, int from, int to) {
    std::vector<bool> used(kMagicSizeOfChessBoard, false);
    std::vector<int> parent(kMagicSizeOfChessBoard);
    std::queue<int> vertexes;
    vertexes.push(from);
    parent[from] = -1;
    used[from] = true;
    while (vertexes.front() != to) {
        int newVertex = vertexes.front();
        vertexes.pop();
        int nextVertex;
        int newLine = newVertex / kMagicLineSize;
        int newColumn = newVertex % kMagicLineSize;
        if (newLine >= 1) {
            if (newColumn >= 2) {
                nextVertex = newVertex - 2 - kMagicLineSize;
                addVertex(parent, vertexes, used, nextVertex, newVertex);
            }
            if (newColumn <= kMagicLineSize - 3) {
                nextVertex = newVertex + 2 - kMagicLineSize;
                addVertex(parent, vertexes, used, nextVertex, newVertex);
            }
        }
        if (newLine <= kMagicLineSize - 2) {
            if (newColumn >= 2) {
                nextVertex = newVertex - 2 + kMagicLineSize;
                addVertex(parent, vertexes, used, nextVertex, newVertex);
            }
            if (newColumn <= kMagicLineSize - 3) {
                nextVertex = newVertex + 2 + kMagicLineSize;
                addVertex(parent, vertexes, used, nextVertex, newVertex);
            }
        }
        if (newLine >= 2) {
            if (newColumn >= 1) {
                nextVertex = newVertex - 1 - 2 * kMagicLineSize;
                addVertex(parent, vertexes, used, nextVertex, newVertex);
            }
            if (newColumn <= 6) {
                nextVertex = newVertex + 1 - 2 * kMagicLineSize;
                addVertex(parent, vertexes, used, nextVertex, newVertex);
            }
        }
        if (newLine <= kMagicLineSize - 3) {
            if (newColumn >= 1) {
                nextVertex = newVertex - 1 + 2 * kMagicLineSize;
                addVertex(parent, vertexes, used, nextVertex, newVertex);
            }
            if (newColumn <= kMagicLineSize - 2) {
                nextVertex = newVertex + 1 + 2 * kMagicLineSize;
                addVertex(parent, vertexes, used, nextVertex, newVertex);
            }
        }
    }
    answer.push_back(to);
    while (parent[to] != -1) {
        to = parent[to];
        answer.push_back(to);
    }
    std::reverse(answer.begin(), answer.end());
}

int main() {
    char start, end;
    int numStart, numEnd;
    std::cin >> start >>  numStart;
    std::cin >> end >> numEnd;
    std::vector<int> answer;
    int from = kMagicLineSize * (start - kAlphabetStart) + numStart - 1;
    int to = kMagicLineSize * (end - kAlphabetStart) + numEnd - 1;
    findAnswer(answer, from, to);
    for (int i : answer) {
        char letter = static_cast<char>(i / kMagicLineSize + kAlphabetStart);
        int num = i % kMagicLineSize + 1;
        std::cout << letter << num << '\n';
    }
}
