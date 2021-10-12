#include <iostream>
#include <vector>
 
const int partAmount = 8;
const int eightByteNumber = 256;
 
struct big{
    int part[partAmount];
};
namespace BigNum{
    int amount;
    std::vector<big> numbers;
}
void LSD(int step) {\
    using namespace BigNum;
    std::vector<int> count(eightByteNumber);
    for (int i = 0; i < eightByteNumber; ++i) {
        count[i] = 0;
    }
    for (int i = 0; i < amount; ++i) {
        count[numbers[i].part[step]]++;
    }
    for (int i = 1; i < eightByteNumber; ++i) {
        count[i] += count[i - 1];
    }
    std::vector<big> answer(amount);
    for (int i = amount - 1; i >= 0; --i) {
        count[numbers[i].part[step]]--;
        answer[count[numbers[i].part[step]]] = numbers[i];
    }
    for (int i = 0; i < amount; ++i) {
        numbers[i] = answer[i];
    }
    count.clear();
    answer.clear();
}
int main(){
    using namespace BigNum;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cin >> amount;
    for (int i = 0; i < amount; ++i) {
        long long bigNumb;
        std::cin >> bigNumb;
        big forPushing;
        for (int j = 0; j < partAmount; ++j) {
            forPushing.part[j] = 0;
        }
        numbers.push_back(forPushing);
        int j = 0;
        while (bigNumb > 0) {
            numbers[i].part[j] = bigNumb % eightByteNumber;
            ++j;
            bigNumb /= eightByteNumber;
        }
    }
    for (int i = 0; i < partAmount; ++i) {
        LSD(i);
    }
    for (int i = 0; i < amount; ++i) {
        long long bigNumb = 0;
        for (int j = partAmount - 1; j >= 0; --j) {
            bigNumb = bigNumb * eightByteNumber + numbers[i].part[j];
        }
        std::cout << bigNumb << " ";
    }
    std::cout << std::endl;
}
