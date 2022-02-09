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
void prepare(int segmentAmount, std::vector<border>& answer, std::vector<border>& number) {
    border edgeZero;
    edgeZero.partValue.resize(MaxLength);
    for (int i = 0; i < MaxLength; ++i) {
        edgeZero.partValue[i] = 0;
    }
    for (int i = 0; i < 2 * segmentAmount; ++i) {
        number[i] = edgeZero;
        answer[i] = edgeZero;
    }
}
void valueToParts(int value, std::vector<border>& number, int step) {
    int length = 0;
    while (value) {
        number[step].partValue[length] = value % NumberSystem;
        value /= NumberSystem;
        ++length;
    }
}
void fillArray(int left, int right, std::vector<border>& number, int step, int segmentAmount) {
    number[step].type = true;
    number[step].value = left;
    number[step + segmentAmount].value = right;
    valueToParts(left, number, step);
    valueToParts(right, number, step + segmentAmount);
}
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
int findAnswer(std::vector<border>& answer, int segmentAmount) {
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
    return sum;
}
int main(){
    int segmentAmount;
    std::cin >> segmentAmount;
    std::vector<border> answer(2 * segmentAmount);
    std::vector<border> number(2 * segmentAmount);
    prepare(segmentAmount, answer, number);
    for (int i = 0; i < segmentAmount; ++i) {
        int left, right;
        std::cin >> left >> right;
        fillArray(left, right, number, i, segmentAmount);
    }
    stabsort(2 * segmentAmount, 0, answer, number);
    int sum = findAnswer(answer, segmentAmount);
    std::cout << sum;
}
