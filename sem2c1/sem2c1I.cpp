#include <iostream>
#include <vector>

int findAnswer(int size1, int size2, std::vector<int>& first, std::vector<int>& second) {
    int dp[size1 + 1][size2 + 1];
    for (int i = 0; i <= size2; ++i) {
        dp[0][i] = 0;
    }
    for (int i = 0; i <= size1; ++i) {
        dp[i][0] = 0;
    }
    for (int i = 1; i <= size1; ++i) {
        std::vector<int> previous(size2 + 1);
        previous[0] = 0;
        previous[1] = 0;
        for (int j = 1; j < size2; ++j) {
            previous[j + 1] = previous[j];
            if (second[j] < first[i]) {
                previous[j + 1] = std::max(previous[j], dp[i - 1][j]);
            }
        }
        for (int j = 1; j <= size2; ++j) {
            if (first[i] != second[j]) {
                dp[i][j] = dp[i - 1][j];
            } else {
                dp[i][j] = previous[j] + 1;
            }
        }
    }
    int answer = 0;
    for (int i = 0; i <= size1; ++i) {
        for (int j = 0; j <= size2; ++j) {
            answer = std::max(answer, dp[i][j]);
        }
    }
    return answer;
}

int main() {
    int size1, size2;
    std::cin >> size1 >> size2;
    std::vector<int> first(size1 + 1);
    std::vector<int> second(size2 + 1);
    for (int i = 1; i <= size1; ++i) {
        std::cin >> first[i];
    }
    for (int i = 1; i <= size2; ++i) {
        std::cin >> second[i];
    }
    int answer = findAnswer(size1, size2, first, second);
    std::cout << answer;
}
