#include <iostream>
#include <vector>

const int kMaxSize = 36;

long long findAnswer(int power) {
    long long answer = 1;
    std::vector<std::vector<long long>> prepared(power + 1);
    for (int i = 0; i <= power; ++i) {
        prepared[i].resize(i + 1, 0);
    }
    prepared[1][1] = 1;
    for (int newPower = 2; newPower <= power; ++newPower) {
        for (int maxValue = 1; maxValue <= newPower; ++maxValue) {
            prepared[newPower][maxValue] = prepared[newPower][maxValue - 1];
        }
    }
    for (int size = 2; size <= kMaxSize; ++size) {
        for (int newPower = power; newPower >= 1; --newPower) {
            for (int maxValue = 1; maxValue <= newPower; ++maxValue) {
                int lastPower = newPower - std::min(maxValue, newPower);
                prepared[newPower][maxValue] = prepared[newPower][maxValue - 1]
                        + prepared[lastPower][std::min(lastPower, maxValue - 1)];
            }
        }
        for (int start = 1; start <= power / 2; ++start) {
            int newPower = std::max(power - size * (start - 1), 0);
            int maxValue = std::min(std::min(2 * start - 1, power) - start + 1, newPower);
            answer += prepared[newPower][maxValue];
        }
    }
    return answer;
}

int main() {
    int power;
    std::cin >> power;
    std::cout << findAnswer(power) << '\n';
}
