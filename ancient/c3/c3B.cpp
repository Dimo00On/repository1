#include <iostream>
#include <vector>

int leftSide(int index) {
    return (index & (index + 1));
}
int rightSide(int index) {
    return (index | (index + 1));
}
void buildFenwick(int numberAmount, std::vector<int>& numbers, std::vector<int>& sum) {
    std::vector<int> plusMinusSuffix(numberAmount + 1);
    plusMinusSuffix[numberAmount] = 0;
    for (int i = numberAmount; i >= 1; --i) {
        plusMinusSuffix[i - 1] = numbers[i - 1] - plusMinusSuffix[i];
    }
    for (int i = 0; i < numberAmount; ++i) {
        if ((i + 1 - leftSide(i)) % 2) {
            sum[i] = plusMinusSuffix[leftSide(i)] + plusMinusSuffix[i + 1];
        } else {
            sum[i] = plusMinusSuffix[leftSide(i)] - plusMinusSuffix[i + 1];
        }
    }
}
void update(int index, int value, int numberAmount, std::vector<int>& numbers, std::vector<int>& sum) {
    for (int i = index; i < numberAmount; i = rightSide(i)) {
        if ((leftSide(i) - index) % 2) {
            sum[i] -= (value - numbers[index]);
        } else {
            sum[i] += (value - numbers[index]);
        }
    }
    numbers[index] = value;
}
int getPrefix(int index, std::vector<int>& sum) {
    int answer = 0;
    for (int i = index; i >= 0; i = leftSide(i) - 1) {
        if ((i - leftSide(i)) % 2) {
            answer = sum[i] + answer;
        } else {
            answer = sum[i] - answer;
        }
    }
    return answer;
}
int getSum(int left, int right, std::vector<int>& sum) {
    int answerRight = getPrefix(right, sum);
    int answerLeft = getPrefix(left - 1, sum);
    if ((leftSide(left - 1) - left) % 2) {
        return -(answerRight - answerLeft);
    }
    return answerRight - answerLeft;
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int numberAmount;
    std::cin >> numberAmount;
    std::vector<int> numbers(numberAmount);
    for (int i = 0; i < numberAmount; ++i) {
        std::cin >> numbers[i];
    }
    std::vector<int> sum(numberAmount);
    buildFenwick(numberAmount, numbers, sum);
    int commandAmount;
    std::cin >> commandAmount;
    for (int i = 0; i < commandAmount; ++i) {
        int command;
        std::cin >> command;
        if (command) {
            int left, right;
            std::cin >> left >> right;
            std::cout << getSum(left - 1, right - 1, sum) << '\n';
        } else {
            int index, value;
            std::cin >> index >> value;
            update(index - 1, value, numberAmount, numbers, sum);
        }
    }
}
