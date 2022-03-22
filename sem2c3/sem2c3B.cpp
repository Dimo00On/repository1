#include <iostream>
#include <vector>
#include <set>

void dfs(int now, int parent, std::set<int>& allParents, std::vector<std::vector<int>>& edges,
         std::vector<std::vector<int>>& checkParent, std::vector<std::vector<bool>>& answers) {
    for (int i = 0; i < checkParent[now].size(); ++i) {
        if (allParents.count(checkParent[now][i])) {
            answers[now][i] = true;
        }
    }
    allParents.insert(now);
    for (int i = 0; i < edges[now].size(); ++i) {
        if (edges[now][i] != parent) {
            dfs(edges[now][i], now, allParents,  edges, checkParent, answers);
        }
    }
    allParents.erase(now);
}

int main() {
    int amount;
    std::cin >> amount;
    std::vector<int> vertexes(amount);
    std::vector<std::vector<int>> edges(amount, std::vector<int>(0));
    for (int i = 0; i < amount; ++i) {
        int value;
        std::cin >> value;
        vertexes[i] = value - 1;
        if (value != 0) {
            edges[i].push_back(value - 1);
            edges[value - 1].push_back(i);
        }
    }
    int questions;
    std::cin >> questions;
    std::vector<std::vector<int>> checkParent(amount);
    std::vector<std::vector<bool>> answers(amount);
    std::vector<std::pair<int, int>> order(questions);
    for (int i = 0; i < questions; ++i) {
        int parent, son;
        std::cin >> parent >> son;
        checkParent[son - 1].push_back(parent - 1);
        answers[son - 1].push_back(false);
        order[i] = std::make_pair(son - 1, checkParent[son - 1].size() - 1);
    }
    for (int i = 0; i < amount; ++i) {
        if (vertexes[i] == -1) {
            std::set<int> allParents;
            dfs(i, -1, allParents, edges, checkParent, answers);
        }
    }
    for (int i = 0; i < questions; ++i) {
        std::cout << static_cast<int>(answers[order[i].first][order[i].second]) << '\n';
    }
    return 0;
}
