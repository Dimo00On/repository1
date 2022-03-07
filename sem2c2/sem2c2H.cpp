#include <iostream>
#include <vector>

bool isThereBit(int mask, int bit) {
    return static_cast<bool>(mask & (1 << bit));
}

int size(int mask) {
    int count = 0;
    while (mask > 0) {
        count += (mask % 2);
        mask /= 2;
    }
    return count;
}

int findAnswer(int amount, std::vector<std::vector<bool>>& network) {
    std::vector<std::vector<bool>> dp(amount + 1);
    for (int i = 0; i <= amount; ++i) {
        dp[i].resize(1 << amount, false);
    }
    dp[amount][0] = true;
    for (int j = 0; j < (1 << amount); ++j) {
        for (int i = 0; i <= amount; ++i) {
            if (!dp[i][j]) {
                continue;
            }
            for (int k = 0; k < amount; ++k) {
                if (isThereBit(j, k)) {
                    continue;
                }
                if (i == amount) {
                    dp[k][j + (1 << k)] = true;
                } else {
                    if (network[i][k]) {
                        dp[amount][j + (1 << k)] = true;
                    }
                }
            }
        }
    }
    int answer = 0;
    for (int j = 0; j < (1 << amount); ++j) {
        if (dp[amount][j]) {
            answer = std::max(answer, size(j));
        }
    }
    return answer;
}

int main() {
    int amount;
    std::cin >> amount;
    std::vector<std::vector<bool>> network(amount);
    for (int i = 0; i < amount; ++i) {
        network[i].resize(amount);
        for (int j = 0; j < amount; ++j) {
            char value;
            std::cin >> value;
            network[i][j] = (value == 'Y');
        }
    }
    std::cout << findAnswer(amount, network);
}
