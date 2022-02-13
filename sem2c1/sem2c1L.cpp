#include <iostream>
#include <vector>
#include <algorithm>


void findPartSum(long long sum, int step, std::vector<long long>& things, int stop, long long weight,
                  std::vector<long long>& partSum, int shift) {
    if (sum > weight) {
        return;
    }
    if (step == stop) {
        partSum.push_back(sum);
        return;
    }
    findPartSum(sum, step + shift, things, stop, weight, partSum, shift);
    findPartSum(sum + things[step], step + shift, things, stop, weight, partSum, shift);
}

int findPosition(std::vector<long long> rightParts, long long weight) {
    int left = -1;
    int right = static_cast<int>(rightParts.size());
    while (right - left > 1) {
        int middle = (left + right) / 2;
        if (rightParts[middle] > weight) {
            right = middle;
        } else {
            left = middle;
        }
    }
    return left;
}

void findAnswer(long long& answer, std::vector<long long>& things, int amount, long long weight) {
    std::vector<long long> leftParts;
    findPartSum(0, 0, things, amount / 2 + 1, weight, leftParts, 1);
    std::vector<long long> rightParts;
    findPartSum(0, amount - 1, things, amount / 2, weight, rightParts, -1);
    std::sort(rightParts.begin(), rightParts.end());
    for (size_t i = 0; i < leftParts.size(); ++i) {
        int index = findPosition(rightParts, weight - leftParts[i]);
        answer += (index + 1);
    }
}

int main() {
    int amount;
    long long weight;
    std::cin >> amount;
    std::vector<long long> things(amount);
    for (int i = 0; i < amount; ++i) {
        std::cin >> things[i];
    }
    std::cin >> weight;
    long long answer = 0;
    findAnswer(answer, things, amount, weight);
    std::cout << answer;
}
