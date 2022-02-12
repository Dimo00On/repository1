#include <iostream>
#include <vector>

const int kInfinity = -(1 << 31);

int findPos(int index, int amount, std::vector<int>& ends, std::vector<int>& numbers) {
    int left = 1;
    int right = amount;
    while (right - left > 1) {
        int middle = (left + right) / 2;
        if (numbers[ends[middle]] < numbers[index]) {
            right = middle;
        } else {
            left = middle;
        }
    }
    if (numbers[ends[left]] < numbers[index]) {
        return left;
    } else {
        return right;
    }
}

int findMaxLengthAndAnswer(std::vector<int>& ends, std::vector<int>& numbers, int amount, std::vector<int>& last,
                  std::vector<int>& answer) {
    for (int i = 1; i <= amount; ++i) {
        int index = findPos(i, amount, ends, numbers);
        last[i] = ends[index - 1];
        ends[index] = i;
    }
    int length;
    for (int i = amount; i >= 1; --i) {
        if (ends[i] > 0) {
            length = i;
            answer.push_back(ends[i]);
            break;
        }
    }
    int index = answer[0];
    while (last[index] != 0) {
        answer.push_back(last[index]);
        index = last[index];
    }
    return length;
}

int main() {
    int amount;
    std::cin >> amount;
    std::vector<int> numbers(amount + 1);
    numbers[0] = kInfinity;
    for (int i = 1; i <= amount; ++i) {
        std::cin >> numbers[i];
    }
    std::vector<int> ends(amount + 1, 0);
    std::vector<int> last(amount + 1, 0);
    std::vector<int> answer;
    int length = findMaxLengthAndAnswer(ends, numbers, amount, last, answer);
    std::cout << length << '\n';
    for (int i = length - 1; i >= 0; --i) {
        std::cout << answer[i] << " ";
    }
}
