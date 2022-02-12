#include <iostream>
#include <vector>

int answer(std::vector<int>& gold, int weight, int amount) {
    bool dp[weight + 1][amount + 1];
    for (int i = amount; i >= 0; --i) {
        dp[0][i] = true;
    }
    for (int j = 1; j <= weight; ++j) {
        dp[j][0] = false;
    }
    for (int i = 1; i <= amount; ++i) {
        for (int j = 1; j <= weight; ++j) {
            dp[j][i] = dp[j][i - 1];
            if (j >= gold[i - 1]) {
                dp[j][i] = dp[j][i] || dp[j - gold[i - 1]][i - 1];
            }
        }
    }
    for (int i = weight; i >= 0; --i) {
        if (dp[i][amount]) {
            return i;
        }
    }
    return 0;
}

int main() {
    int amount;
    int weight;
    std::cin >> weight >> amount;
    std::vector<int> gold(amount);
    for (int i = 0; i < amount; ++i) {
        std::cin >> gold[i];
    }
    std::cout << answer(gold, weight, amount);
}
