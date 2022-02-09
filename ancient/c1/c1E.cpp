#include <iostream>
#include <vector>

const int MaxSize = 900;
int findAnswer(int stringSize, std::vector <int>& firstArray, std::vector <int>& secondArray) {
    int left = 0;
    int right = stringSize - 1;
    int middle;
    while (right - left > 1) {
        middle = (right + left) / 2;
        if (firstArray[middle] > secondArray[middle]) {
            right = middle;
        } else {
            left = middle;
        }
    }
    if (std::max(firstArray[left], secondArray[left]) < std::max(firstArray[right], secondArray[right])) {
        return left;
    } else {
        return right;
    }
}
int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::vector <int> firstArray[MaxSize];
    std::vector <int> secondArray[MaxSize];
    int firstArraySize;
    int secondArraySize;
    int stringSize;
    std::cin >> firstArraySize >> secondArraySize >> stringSize;
    int newElement;
    for (int i = 0; i < firstArraySize; ++i) {
        for (int j = 0; j < stringSize; ++j) {
            std::cin >> newElement;
            firstArray[i].push_back(newElement);
        }
    }
    for (int i = 0; i < secondArraySize; ++i) {
        for (int j = 0; j < stringSize; ++j) {
            std::cin >> newElement;
            secondArray[i].push_back(newElement);
        }
    }
    int commandAmount;
    std::cin >> commandAmount;
    for (int u = 0; u < commandAmount; ++u) {
        int i, j;
        std::cin >> i >> j; // i, j from task description
        int middle = findAnswer(stringSize, firstArray[i - 1], secondArray[j - 1]);
        std::cout << middle + 1 << '\n';
    }
}
