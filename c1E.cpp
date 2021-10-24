#include <iostream>
#include <vector>

const int MaxSize = 900;
void pleaseSolveThisTask() {
    std::vector <int> firstArray[MaxSize];
    std::vector <int> secondArray[MaxSize];
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int firstArraySize;
    int secondArraySize;
    int stringSize;
    std::cin >> firstArraySize >> secondArraySize >> stringSize;
    for (int i = 0; i < firstArraySize; ++i) {
        for (int j = 0; j < stringSize; ++j) {
            int newElement;
            std::cin >> newElement;
            firstArray[i].push_back(newElement);
        }
    }
    for (int i = 0; i < secondArraySize; ++i) {
        for (int j = 0; j < stringSize; ++j) {
            int newElement;
            std::cin >> newElement;
            secondArray[i].push_back(newElement);
        }
    }
    int commandAmount;
    std::cin >> commandAmount;
    for (int u = 0; u < commandAmount; ++u) {
        int i;
        int j;
        std::cin >> i >> j; // i, j from task description
        --i;
        --j;
        int left = 0;
        int right = stringSize - 1;
        int middle;
        while (right - left > 1) {
            middle = (right + left) / 2;
            if (firstArray[i][middle] > secondArray[j][middle]) {
                right = middle;
            } else {
                left = middle;
            }
        }
        if (std::max(firstArray[i][left], secondArray[j][left]) < std::max(firstArray[i][right], secondArray[j][right])) {
            middle = left;
        } else {
            middle = right;
        }
        std::cout << middle + 1 << '\n';
    }
}
int main(){
    pleaseSolveThisTask();
}
