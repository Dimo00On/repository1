#include <iostream>
#include <vector>
#include <string>

const int MaxHeapSize = 500000;
struct Binary{
    long long value[MaxHeapSize + 1];
    int operation[MaxHeapSize + 1];
    int size = 0;
    std::vector<int> position;
    void siftUp(int index);
    void insert(int value, int operation);
    void siftDown(int index);
    void extractMin();
    void decreaseKey(int hisOperation, int delta);
};
void Binary::siftUp(int index) {
    while (index != 1) {
        if (value[index] < value[index / 2]) {
            std::swap(position[operation[index]], position[operation[index / 2]]);
            std::swap(value[index], value[index / 2]);
            std::swap(operation[index], operation[index / 2]);
            index /= 2;
        } else {
            break;
        }
    }
}
void Binary::siftDown(int index) {
    while (2 * index <= size) {
        int newIndex = 2 * index;
        if (newIndex + 1 <= size && value[newIndex + 1] < value[newIndex]) {
            ++newIndex;
        }
        if (value[newIndex] < value[index]) {
            std::swap(position[operation[index]], position[operation[newIndex]]);
            std::swap(value[index], value[newIndex]);
            std::swap(operation[index], operation[newIndex]);
            index = newIndex;
        } else {
            break;
        }
    }
}
void Binary::decreaseKey(int hisOperation, int delta) {
    value[position[hisOperation]] -= delta;
    siftUp(position[hisOperation]);
}
void Binary::insert(int newValue, int newOperation) {
    ++size;
    value[size] = newValue;
    operation[size] = newOperation;
    position[newOperation] = size;
    siftUp(size);
}
void Binary::extractMin(){
    value[1] = value[size];
    operation[1] = operation[size];
    --size;
    position[operation[1]] = 1;
    siftDown(1);
}
void workingWithHeap(){
    Binary heap;
    int commandAmount;
    std::cin >> commandAmount;
    heap.position.push_back(0);
    for (int i = 1; i <= commandAmount; ++i) {
        std::string command;
        std::cin >> command;
        heap.position.push_back(0);
        if (command == "insert") {
            int value;
            std::cin >> value;
            heap.insert(value, i);
        }
        if (command == "getMin") {
            std::cout << heap.value[1] << '\n';
        }
        if (command == "extractMin") {
            heap.extractMin();
        }
        if (command == "decreaseKey") {
            int operation;
            int delta;
            std::cin >> operation >> delta;
            heap.decreaseKey(operation, delta);
        }
    }
}
int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    workingWithHeap();
}
