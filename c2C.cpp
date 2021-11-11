#include <iostream>
#include <vector>
#include <string>

const int RandFunctionRange = 32768;
void partition(int left, int right, int pivot, int& less, int& equal, std::vector<int>& array){
    std::vector <int> b;
    for (int i = left; i < right; ++i) {
        if (array[i] < pivot) {
            b.push_back(array[i]);
            ++less;
        }
    }
    for (int i = left; i < right; ++i) {
        if (array[i] == pivot) {
            b.push_back(array[i]);
            ++equal;
        }
    }
    for (int i = left; i < right; ++i) {
        if (array[i] > pivot) {
            b.push_back(array[i]);
        }
    }
    for (int i = left, j = 0; i < right; ++i, ++j) {
        array[i] = b[j];
    }
    b.clear();
}
int QuickSelect(int left, int right, int sortedIndex, std::vector<int>& array) {
    if (right == left + 1) {
        return array[left];
    }
    int indexPivot = (rand() * RandFunctionRange + rand()) % (right - left);
    int pivot = array[indexPivot + left];
    int less = 0;
    int equal = 0;
    partition(left, right, pivot, less, equal, array);
    if (sortedIndex < less) {
        return QuickSelect(left, left + less, sortedIndex, array);
    }
    if (sortedIndex < less + equal) {
        return pivot;
    }
    return QuickSelect(left + less + equal, right, sortedIndex - less - equal, array);
}
void pleaseSolveThisTask(const std::string& uselessString) {
    int size, sortedIndex;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cin >> size >> sortedIndex;
    std::vector<int> array(size);
    for (int i = 0; i < size; ++i) {
        std::cin >> array[i];
    }
    std::cout << QuickSelect(0, size, sortedIndex, array);
}
int main(){
    pleaseSolveThisTask("C task");
}
