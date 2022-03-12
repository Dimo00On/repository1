#include <iostream>
#include <vector>

int answer(std::vector<int>& first, std::vector<int>& second, int size1, int size2) {
    int dp[size1 + 1][size2 + 1];
    for (int i = 0; i <= size1; ++i) {
        dp[i][0] = 0;
    }
    for (int i = 0; i <= size2; ++i) {
        dp[0][i] = 0;
    }
    for (int i = 1; i <= size1; ++i) {
        for (int j = 1; j <= size2; ++j) {
            dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            if (first[i - 1] == second[j - 1]) {
                dp[i][j] = std::max(dp[i][j], dp[i - 1][j - 1] + 1);
            }
        }
    }
    return dp[size1][size2];
}

int main() {
    int size1, size2;
    std::cin >> size1;
    std::vector<int> first(size1);
    for (int i = 0; i < size1; ++i) {
        std::cin >> first[i];
    }
    std::cin >> size2;
    std::vector<int> second(size2);
    for (int i = 0; i < size2; ++i) {
        std::cin >> second[i];
    }
    std::cout << answer(first, second, size1, size2);
}
