#include <iostream>
#include <vector>

const int MaxLength = 10;
const int NumberSystem = 10;
struct border{ // для хранения границы отрезка. type означает, начало или конец отрезка.
    // value означает координаты соответствующей границы отрезка. partValue означает разбиение числа value по цифрам
    std::vector<int> partValue;
    bool type = false;
    int value = 0;
};
void stabsort(int size, int step, std::vector<border>& answer, std::vector<border>& number) {
    if (step == MaxLength) {
        return;
    }
    if (step) {
        for (int i = 0; i < size; ++i) {
            number[i] = answer[i];
        }
    }
    std::vector<int> count(NumberSystem, 0);
    for (int i = 0; i < size; ++i) {
        ++count[number[i].partValue[step]];
    }
    for (int i = 1; i < NumberSystem; ++i) {
        count[i] += count[i - 1];
    }
    for (int i = size - 1; i >= 0; --i) {
        answer[--count[number[i].partValue[step]]] = number[i];
    }
    stabsort(size, step + 1, answer, number);
}
void pleaseSolveThisTask(const std::string& uselessString) {
    border edgeZero;
    edgeZero.partValue.resize(MaxLength);
    for (int i = 0; i < MaxLength; ++i) {
        edgeZero.partValue[i] = 0;
    }
    int segmentAmount;
    std::cin >> segmentAmount;
    std::vector<border> answer(2 * segmentAmount);
    std::vector<border> number(2 * segmentAmount);
    for (int i = 0; i < 2 * segmentAmount; ++i) {
        number[i] = edgeZero;
        answer[i] = edgeZero;
    }
    for (int i = 0; i < segmentAmount; ++i) {
        int left, right;
        std::cin >> left >> right;
        number[i].type = true;
        number[i].value = left;
        number[i + segmentAmount].value = right;
        int length = 0;
        while (left) {
            number[i].partValue[length] = left % NumberSystem;
            left /= NumberSystem;
            ++length;
        }
        length = 0;
        while (right) {
            number[i + segmentAmount].partValue[length] = right % NumberSystem;
            right /= NumberSystem;
            ++length;
        }
    }
    stabsort(2 * segmentAmount, 0, answer, number);
    int lay = 1;
    int start = answer[0].value;
    int sum = 0;
    for (int i = 1; i < 2 * segmentAmount; ++i) {
        if (lay == 1) {
            lay += (answer[i].type ? 1 : -1);
            sum += (answer[i].value - start);
            continue;
        }
        lay += (answer[i].type ? 1 : -1);
        if (lay == 1) {
            start = answer[i].value;
        }
    }
    std::cout << sum;
}
int main(){
    pleaseSolveThisTask("B task");
}
