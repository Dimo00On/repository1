#include <iostream>
#include <vector>

const int kInfinity = 1 << 30;

int findDegree(int height) {
    int degree = 0;
    --height;
    while (height > 0) {
        height /= 2;
        ++degree;
    }
    return degree;
}

std::pair<int, int> findLevel(std::vector<std::vector<int>>& dp, int height, int attempts) {
    int left = 1;
    int right = height;
    while (right - left > 1) {
        int middle = (left + right) / 2;
        if (dp[middle][attempts - 1] > dp[height - middle][attempts]) {
            right = middle;
        } else {
            left = middle;
        }
    }
    return std::make_pair(left, right);
}

int findAnswer(int height, int attempts) {
    std::vector<std::vector<int>> dp(height + 1);
    for (int i = 1; i <= height; ++i) {
        dp[i].resize(attempts + 1);
        dp[i][0] = kInfinity;
        dp[i][1] = i - 1;
    }
    dp[1][0] = 0;
    dp[0].resize(attempts + 1);
    for (int i = 0; i <= attempts; ++i) {
        dp[0][i] = 0;
    }
    for (int j = 2; j <= attempts; ++j) {
        for (int i = 1; i <= height; ++i) {
            int minSum = i - 1;
            std::pair<int, int> levelThrow = findLevel(dp, i, j);
            minSum = std::min(std::max(dp[levelThrow.first][j - 1], dp[i - levelThrow.first][j]) + 1, minSum);
            minSum = std::min(std::max(dp[levelThrow.second][j - 1], dp[i - levelThrow.second][j]) + 1, minSum);
            dp[i][j] = minSum;
        }
    }
    return dp[height][attempts];
}

int main() {
    int height;
    int attempts;
    std::cin >> height >> attempts;
    if (height == 1) {
        std::cout << 0;
        return 0;
    }
    if (attempts == 0 && height > 1) {
        std::cout << -1;
        return 0;
    }
    int degree = findDegree(height);
    if (attempts >= degree) {
        std::cout << degree;
    } else {
        std::cout << findAnswer(height, attempts);
    }
}
