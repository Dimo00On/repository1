#include <iostream>
#include <vector>

const int kModule = 1e9 + 7;

bool isThereBit(int mask, int bit) {
    return static_cast<bool>(mask & (1 << bit));
}

void findAnswer(long long& answer, std::vector<std::vector<char>>& map, int size, int length) {
    std::vector<std::vector<std::vector<int>>> dp(length - 1);
    for (int i = 0; i < length - 1; ++i) {
        dp[i].resize(size);
        for (int j = 0; j < size; ++j) {
            dp[i][j].resize(1 << (size + 1), 0);
        }
    }
    int maskPlus = 0;
    int maskMinus = 0;
    for (int i = 0; i < size; ++i) {
        if (map[0][i] == '+') {
            maskPlus += (1 << i);
        }
        if (map[0][i] == '-') {
            maskMinus += (1 << i);
        }
    }
    for (int mask = 0; mask < (1 << size); ++mask) {
        int inverseMask = ((1 << (size + 1)) - mask - 1);
        if ((mask | maskPlus) == mask && (inverseMask | maskMinus) == inverseMask) {
            dp[0][0][mask] = (map[1][0] == '+' ? 0 : 1);
            dp[0][0][mask + (1 << size)] = (map[1][0] == '-' ? 0 : 1);
        } else {
            dp[0][0][mask] = 0;
            dp[0][0][mask + (1 << size)] = 0;
        }
    }
    for (int i = 0; i < length - 1; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int mask = 0; mask < (1 << (size + 1)); ++mask) {
                int rightBit = static_cast<int>(isThereBit(mask, size));
                int leftBit = static_cast<int>(isThereBit(mask, j));
                int tempMask = mask - (1 << size) * rightBit - (1 << j) * leftBit;
                tempMask += (1 << j) * rightBit;
                if (j < size - 1) {
                    int downLeftBit = static_cast<int>(isThereBit(mask, j + 1));
                    if (map[i + 1][j + 1] == '+') {
                        dp[i][j + 1][tempMask] = 0;
                    } else {
                        if (leftBit + rightBit + downLeftBit == 2) {
                            dp[i][j + 1][tempMask] += dp[i][j][mask];
                            dp[i][j + 1][tempMask] %= kModule;
                        }
                    }
                    if (map[i + 1][j + 1] == '-') {
                        dp[i][j + 1][tempMask + (1 << size)] = 0;
                    } else {
                        if (leftBit + rightBit + downLeftBit == 1) {
                            dp[i][j + 1][tempMask + (1 << size)] += dp[i][j][mask];
                            dp[i][j + 1][tempMask + (1 << size)] %= kModule;
                        }
                    }
                } else {
                    if (i == length - 2) {
                        break;
                    }
                    if (map[i + 2][0] == '+') {
                        dp[i + 1][0][tempMask] = 0;
                    } else {
                        dp[i + 1][0][tempMask] += dp[i][j][mask];
                        dp[i + 1][0][tempMask] %= kModule;
                    }
                    if (map[i + 2][0] == '-') {
                        dp[i + 1][0][tempMask + (1 << size)] = 0;
                    } else {
                        dp[i + 1][0][tempMask + (1 << size)] += dp[i][j][mask];
                        dp[i + 1][0][tempMask + (1 << size)] %= kModule;
                    }
                }
            }
        }
    }
    for (int i = 0; i < (1 << (size + 1)); ++i) {
        answer += dp[length - 2][size - 1][i];
        answer %= kModule;
    }
}

void solveCase(long long& answer, int count) {
    answer = 1;
    for (int i = 0; i < count; ++i) {
        answer *= 2;
        answer %= kModule;
    }
}

int main() {
    int size, length;
    std::cin >> size >> length;
    std::vector<std::vector<char>> map(length);
    int count = 0;
    for (int i = 0; i < length; ++i) {
        map[i].resize(size);
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < length; ++j) {
            std::cin >> map[j][i];
            if (map[j][i] == '.') {
                ++count;
            }
        }
    }
    long long answer = 0;
    if (length == 1 || size == 1) {
        solveCase(answer, count);
        std::cout << answer << '\n';
        return 0;
    }
    findAnswer(answer, map, size, length);
    std::cout << answer << '\n';
}
