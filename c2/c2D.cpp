#include <iostream>
#include <vector>
#include <string>

const int MaxHeapSize = 100000;
struct Binary{
private:
    std::vector<long long> value;
    std::vector<int> operation;
    int size;
    std::vector<int> position;
public:
    Binary() = default;
    explicit Binary(int maxSize) {
        value.resize(maxSize + 1);
        operation.resize(maxSize + 1);
        size = 0;
        position.resize(1);
    }
    void siftUp(int index);
    void insert(int value, int operation);
    void siftDown(int index);
    void extractMin();
    void decreaseKey(int hisOperation, int delta);
    void insertPosition();
    long long getMin();
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
void Binary::insertPosition() {
    position.push_back(0);
}
long long Binary::getMin() {
    return value[1];
}
int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    Binary heap(MaxHeapSize);
    int commandAmount;
    std::cin >> commandAmount;
    for (int i = 1; i <= commandAmount; ++i) {
        std::string command;
        std::cin >> command;
        heap.insertPosition();
        if (command == "insert") {
            int value;
            std::cin >> value;
            heap.insert(value, i);
        }
        if (command == "getMin") {
            std::cout << heap.getMin() << '\n';
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
