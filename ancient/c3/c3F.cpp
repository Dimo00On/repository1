#include <iostream>
#include <vector>

int leftSide(int index) {
    return (index & (index + 1));
}
int rightSide(int index) {
    return (index | (index + 1));
}
void preparation(int skySize, std::vector<std::vector<std::vector<int>>>& sum){
    for (int i = 0; i < skySize; ++i) {
        sum[i].resize(skySize);
        for (int j = 0; j < skySize; ++j) {
            sum[i][j].resize(skySize);
            for (int u = 0; u < skySize; ++u) {
                sum[i][j][u] = 0;
            }
        }
    }
}
void update(int x, int y, int z, int value, std::vector<std::vector<std::vector<int>>>& sum, int skySize) {
    for (int i = x; i < skySize; i = rightSide(i)) {
        for (int j = y; j < skySize; j = rightSide(j)) {
            for (int u = z; u < skySize; u = rightSide(u)) {
                sum[i][j][u] += value;
            }
        }
    }
}
int prefix(int x, int y, int z, std::vector<std::vector<std::vector<int>>>& sum) {
    int ans = 0;
    for (int i = x; i >= 0; i = leftSide(i) - 1) {
        for (int j = y; j >= 0; j = leftSide(j) - 1) {
            for (int u = z; u >= 0; u = leftSide(u) - 1) {
                ans += sum[i][j][u];
            }
        }
    }
    return ans;
}
int findSum(int xLeft, int xRight, int yLeft, int yRight, int zLeft, int zRight, std::vector<std::vector<std::vector<int>>>& sum) {
    int ans = prefix(xRight, yRight, zRight, sum);
    ans -= prefix(xLeft, yRight, zRight, sum);
    ans -= prefix(xRight, yLeft, zRight, sum);
    ans -= prefix(xRight, yRight, zLeft, sum);
    ans += prefix(xLeft, yLeft, zRight, sum);
    ans += prefix(xLeft, yRight, zLeft, sum);
    ans += prefix(xRight, yLeft, zLeft, sum);
    ans -= prefix(xLeft, yLeft, zLeft, sum);
    return ans;
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int skySize;
    std::cin >> skySize;
    std::vector<std::vector<std::vector<int>>> sum(skySize);
    preparation(skySize, sum);
    int command = 0;
    while (command != 3) {
        std::cin >> command;
        if (command == 1) {
            int x, y, z, stars;
            std::cin >> x >> y >> z >> stars;
            update(x, y, z, stars, sum, skySize);
        }
        if (command == 2) {
            int xLeft, xRight, yLeft, yRight, zLeft, zRight;
            std::cin >> xLeft >> yLeft >> zLeft >> xRight >> yRight >> zRight;
            std::cout << findSum(xLeft - 1, xRight, yLeft - 1, yRight, zLeft - 1, zRight, sum) << '\n';
        }
    }
}
