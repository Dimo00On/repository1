#include <iostream>
#include <vector>

const long long kModule = 1000000007;

long long findAnswer(std::vector<int>& number, int length, int maxValue) {
    std::vector<int> index(maxValue + 1);
    std::vector<long long> prefixSum(length + 1);
    prefixSum[0] = 1;
    for (int i = 1; i <= length; ++i) {
        long long newSequences = prefixSum[i - 1];
        if (index[number[i - 1]] != 0) {
            newSequences -= prefixSum[index[number[i - 1]] - 1];
        }
        prefixSum[i] = (prefixSum[i - 1] + newSequences + kModule) % kModule;
        index[number[i - 1]] = i;
    }
    return (prefixSum[length] - prefixSum[0] + kModule) % kModule;
}

int main() {
    int length;
    std::cin >> length;
    std::vector<int> number(length);
    int maxValue;
    for (int i = 0; i < length; ++i) {
        std::cin >> number[i];
        if (maxValue < number[i]) {
            maxValue = number[i];
        }
    }
    std::cout << findAnswer(number, length, maxValue);
}
