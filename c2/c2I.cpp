#include <iostream>
#include <vector>
#include <algorithm>

const int SmallArraySize = 5;
int findMedian(int*& array, int amount, int step){
    int size = std::min(amount - step, SmallArraySize);
    std::vector<int> sorted(size);
    for (int i = 0; i < size; ++i) {
        sorted[i] = array[step + i];
    }
    std::sort(sorted.begin(), sorted.end());
    if (step + SmallArraySize <= amount) {
        return array[step + (SmallArraySize / 2)];
    }
    return array[step];
}
void partition(int*& array, int pivot, int amount, int& less, int& equal){
    std::vector<int> betterArray;
    less = 0;
    equal = 0;
    for (int i = 0; i < amount; ++i) {
        if (array[i] < array[pivot]) {
            betterArray.push_back(array[i]);
            ++less;
        }
    }
    for (int i = 0; i < amount; ++i) {
        if (array[i] == array[pivot]) {
            betterArray.push_back(array[i]);
            ++equal;
        }
    }
    for (int i = 0; i < amount; ++i) {
        if (array[i] > array[pivot]) {
            betterArray.push_back(array[i]);
        }
    }
    for (int i = 0; i < amount; ++i) {
        array[i] = betterArray[i];
    }
}
int quickSelect(int* array, int amount, int needNumber, int& less, int& equal){
    if (amount == 1) {
        return 0;
    }
    int pivot = amount / 2;
    partition(array, pivot, amount, less, equal);
    if (needNumber < less) {
        return quickSelect(array, less, needNumber, less, equal);
    }
    int shift = less + equal;
    if (needNumber < shift) {
        return pivot;
    }
    return quickSelect(array + shift, amount - shift, needNumber - shift, less, equal);
}
void quicksort(int* array, int amount, int& less, int& equal){
    if (amount == 1) {
        return;
    }
    int goodAmount = (amount + SmallArraySize - 1) / SmallArraySize;
    int* medians = new int[goodAmount];
    for (int i = 0; i < amount; i += SmallArraySize) {
        medians[i / SmallArraySize] = findMedian(array, amount, i);
    }
    int pivot = quickSelect(medians, goodAmount, goodAmount / 2, less, equal);
    delete[] medians;
    partition(array, pivot, amount, less, equal);
    int shift = less + equal;
    if (less > 1) {
        quicksort(array, less, less, equal);
    }
    if (amount > shift + 1) {
        quicksort(array + shift, amount - shift, less, equal);
    }
}
int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int amount;
    std::cin >> amount;
    int* array = new int[amount];
    int number;
    for (int i = 0; i < amount; ++i) {
        std::cin >> number;
        array[i] = number;
    }
    int less = 0;
    int equal = 0;
    quicksort(array, amount, less, equal);
    for (int i = 0; i < amount; ++i) {
        std::cout << array[i] << " ";
    }
}
