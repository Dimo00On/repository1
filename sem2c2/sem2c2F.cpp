#include <iostream>
#include <vector>

const int kInfinity = 1e9;

bool isThereBit(int mask, int bit) {
    return static_cast<bool>(mask & (1 << bit));
}

void findAnswer(int& answer, int shopAmount, int productAmount, std::vector<std::vector<int>>& prices,
              std::vector<int>& road) {
    std::vector<int> sum(1 << productAmount, kInfinity);
    std::vector<std::vector<int>> maskSum(shopAmount);
    for (int i = 0; i < shopAmount; ++i) {
        maskSum[i].resize(1 << productAmount, 0);
        for (int k = 0; k < (1 << productAmount); ++k) {
            for (int j = 0; j < productAmount; ++j) {
                if (isThereBit(k, j)) {
                    maskSum[i][k] += prices[i][j];
                }
            }
        }
    }
    std::vector<int> bestSum(1 << productAmount, kInfinity);
    for (int i = 0; i < shopAmount; ++i) {
        for (int j = 0; j < (1 << productAmount); ++j) {
            if (bestSum[j] > maskSum[i][j] + road[i]) {
                bestSum[j] = maskSum[i][j] + road[i];
            }
        }
    }
    sum[0] = 0;
    for (int j = 0; j < (1 << productAmount); ++j) {
        int submask = j;
        while (submask > 0) {
            sum[j] = std::min(sum[j], sum[j - submask] + bestSum[submask]);
            submask = (submask - 1) & j;
        }
    }
    answer = sum[(1 << productAmount) - 1];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int shopAmount, productAmount;
    std::cin >> shopAmount >> productAmount;
    std::vector<int> road(shopAmount);
    std::vector<std::vector<int>> prices(shopAmount);
    for (int i = 0; i < shopAmount; ++i) {
        prices[i].resize(productAmount);
        std::cin >> road[i];
        for (int j = 0; j < productAmount; ++j) {
            std::cin >> prices[i][j];
        }
    }
    int answer = kInfinity;
    findAnswer(answer, shopAmount, productAmount, prices, road);
    std::cout << answer;
}
