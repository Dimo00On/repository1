#include <iostream>
#include <vector>

const int kInfinity = 1000000000;

bool isThereBit(int mask, int bit) {
    return static_cast<bool>((mask & (1 << bit)));
}

int findAnswer(int amount, std::vector<std::vector<int>>& road, std::vector<int>& path) {
    std::vector<std::vector<int>> dp(amount);
    std::vector<std::vector<std::pair<int,int>>> prev(amount);
    for (int i = 0; i < amount; ++i) {
        dp[i].resize(1 << amount, kInfinity);
        prev[i].resize(1 << amount, std::make_pair(0, 0));
    }
    int answer = kInfinity;
    for (int i = 0; i < amount; ++i) {
        dp[i][1 << i] = 0;
    }
    for (int i = 0; i < (1 << amount); ++i) {
        for (int j = 0; j < amount; ++j) {
            if (!isThereBit(i, j)) {
                for (int k = 0; k < amount; ++k) {
                    if (isThereBit(i, k)) {
                        if (dp[j][i + (1 << j)] > dp[k][i] + road[k][j]) {
                            dp[j][i + (1 << j)] = dp[k][i] + road[k][j];
                            prev[j][i + (1 << j)] = std::make_pair(k, i);
                        }
                    }
                }
            }
        }
    }
    std::pair<int, int> index;
    for (int i = 0; i < amount; ++i) {
        if (answer > dp[i][(1 << amount) - 1]) {
            answer = dp[i][(1 << amount) - 1];
            index  = std::make_pair(i, (1 << amount) - 1);
        }
    }
    while (index != std::make_pair(0,0)) {
        path.push_back(index.first);
        index = prev[index.first][index.second];
    }
    return answer;
}

int main() {
    int amount;
    std::cin >> amount;
    std::vector<std::vector<int>> road(amount);
    std::vector<int> path;
    for (int i = 0; i < amount; ++i) {
        road[i].resize(amount);
        for (int j = 0; j < amount; ++j) {
            std::cin >> road[i][j];
        }
    }
    std::cout << findAnswer(amount, road, path) << '\n';
    for (int i = 0; i < amount; ++i) {
        std::cout << path[i] + 1 << " ";
    }
}
