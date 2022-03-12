#include <iostream>
#include <vector>

const int kMaxSize = 11;

long long answer(int power) {
    std::vector<long long> dp[kMaxSize][power + 1];
    for (int newPower = 2; newPower <= power; ++newPower) {
        for (int j = 3; j < kMaxSize; ++j) {
            int maxStart = newPower / ((1 << j) - 1);
            dp[j][newPower].resize(maxStart + 1, 0);
            if (j == 3) {
                for (int start = 1; start <= maxStart; ++start) {
                    dp[j][newPower][start] = (newPower - start) / 3 - 2 * start + 1;
                }
                continue;
            }
            for (int start = 1; start <= maxStart; ++start) {
                for (int i = 2 * start; i < dp[j - 1][newPower - start].size(); ++i) {
                    dp[j][newPower][start] += dp[j - 1][newPower - start][i];
                }
            }
        }
    }
    long long sum = 0;
    for (int j = 3; j < kMaxSize; ++j) {
        int lastStart = power / ((1 << j) - 1);
        for (int start = 1; start <= lastStart; ++start) {
            sum += dp[j][power][start];
        }
    }
    sum += 1; //j = 1
    sum += power / 3; //j = 2
    return sum;
}

int main() {
    int power;
    std::cin >> power;
    std::cout << answer(power);
}
