#include <iostream>
#include <vector>
#include <algorithm>
 
const int SmallArraySize = 5;
 
namespace Sorting{
    int less;
    int equal;
}
 
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
void Partition(int*& array, int pivot, int amount){
    std::vector<int> betterArray;
    using namespace Sorting;
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
        array[i]=betterArray[i];
    }
}
int QuickSelect(int* array, int amount, int needNumber){
    using namespace Sorting;
    if (amount == 1) {
        return 0;
    }
    /*int goodAmount = (amount + SmallArraySize - 1) / SmallArraySize;
    int* medians = new int[goodAmount];
    for (int i = 0; i < amount; i += SmallArraySize) {
        medians[i / SmallArraySize] = findMedian(array, amount, i);
    }*/
    int pivot = amount / 2;//QuickSelect(medians, goodAmount, goodAmount / 2);
    Partition(array, pivot, amount);
    if (needNumber < less) {
        return QuickSelect(array, less, needNumber);
    }
    int shift = less + equal;
    if (needNumber < shift) {
        return pivot;
    }
    return QuickSelect(array + shift, amount - shift, needNumber - shift);
}
void Quicksort(int* array, int amount){
    using namespace Sorting;
    if (amount == 1) {
        return;
    }
    int goodAmount = (amount + SmallArraySize - 1) / SmallArraySize;
    int* medians = new int[goodAmount];
    for (int i = 0; i < amount; i += SmallArraySize) {
        medians[i / SmallArraySize] = findMedian(array, amount, i);
    }
    int pivot = QuickSelect(medians, goodAmount, goodAmount / 2);
    delete[] medians;
    Partition(array, pivot, amount);
    int shift = less + equal;
    if (less > 1) {
        Quicksort(array, less);
    }
    if (amount > shift + 1) {
        Quicksort(array + shift, amount - shift);
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
    Quicksort(array, amount);
    for (int i = 0; i < amount; ++i) {
        std::cout << array[i] << " ";
    }
}
