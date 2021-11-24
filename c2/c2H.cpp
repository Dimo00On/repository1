#include <iostream>
#include <vector>

const int PartAmount = 8;
const long long EightByteNumber = 256;
struct BigInteger{
    std::vector<int> part;
    BigInteger(){
        part.resize(PartAmount);
    }
};
void LSD(int step, std::vector<BigInteger>& numbers) {
    int numberAmount = static_cast<int>(numbers.size());
    std::vector<int> count(EightByteNumber, 0);
    for (int i = 0; i < numberAmount; ++i) {
        ++count[numbers[i].part[step]];
    }
    for (long long i = 1; i < EightByteNumber; ++i) {
        count[i] += count[i - 1];
    }
    std::vector<BigInteger> answer(numberAmount);
    for (int i = numberAmount - 1; i >= 0; --i) {
        --count[numbers[i].part[step]];
        answer[count[numbers[i].part[step]]] = numbers[i];
    }
    for (int i = 0; i < numberAmount; ++i) {
        numbers[i] = answer[i];
    }
}
void valueToParts(long long value, std::vector<BigInteger>& numbers, int step) {
    int j = 0;
    while (value > 0) {
        numbers[step].part[j] = static_cast<int>(value % EightByteNumber);
        ++j;
        value /= EightByteNumber;
    }
}
long long partsToValue(std::vector<BigInteger>& numbers, int step) {
    long long value = 0;
    for (int j = PartAmount - 1; j >= 0; --j) {
        value = value * EightByteNumber + numbers[step].part[j];
    }
    return value;
}
int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int numberAmount;
    std::cin >> numberAmount;
    std::vector<BigInteger> numbers(numberAmount);
    for (int i = 0; i < numberAmount; ++i) {
        long long bigNumber;
        std::cin >> bigNumber;
        valueToParts(bigNumber, numbers, i);
    }
    for (int i = 0; i < PartAmount; ++i) {
        LSD(i, numbers);
    }
    for (int i = 0; i < numberAmount; ++i) {
        long long bigNumber = partsToValue(numbers, i);
        std::cout << bigNumber << " ";
    }
}
