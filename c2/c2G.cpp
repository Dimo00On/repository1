#include <iostream>
#include <vector>
#include <string>

const int MaxSize = 100000;
const int DigitStart = 48;
const int FirstDigitIndex = 7;
struct Heap{
private:
    int size;
    std::vector<int> value;
public:
    void siftDown(int index);
    int siftUp(int index);
    void insert(int value);
    int getMin();
    int getMax();
    Heap() = default;
    explicit Heap(int newSize) {
        value.resize(newSize);
        size = 0;
    }
};
void Heap::siftDown(int index) {
    while (2 * index <= size) {
        int newIndex = 2 * index;
        if (newIndex + 1 <= size && value[newIndex + 1] < value[newIndex]) {
            ++newIndex;
        }
        if (value[newIndex] < value[index]) {
            std::swap(value[newIndex], value[index]);
            index = newIndex;
        } else {
            break;
        }
    }
}
int Heap::siftUp(int index) {
    while (index != 1) {
        if (value[index] < value[index/2]) {
            std::swap(value[index], value[index/2]);
            index /= 2;
        } else {
            break;
        }
    }
    return index;
}
void Heap::insert(int newValue){
    ++size;
    value[size] = newValue;
    siftUp(size);
}
int Heap::getMin(){
    int min = value[1];
    value[1] = value[size];
    --size;
    siftDown(1);
    return min;
}
int Heap::getMax(){
    int max = 0;
    int index;
    for (int i = size; i >= size/2; --i) {
        if (value[i] > max) {
            max = value[i];
            index = i;
        }
    }
    if (value[index] > value[size]) {
        value[index] = value[size];
        siftUp(index);
    } else {
        value[index] = value[size];
        siftDown(index);
    }
    --size;
    return max;
}
int decipherThisTerribleInput(std::string& command) {
    int value = 0;
    int point = FirstDigitIndex;
    while (command[point] != ')') {
        value = value * 10 + command[point] - DigitStart;
        point++;
    }
    return value;
}
int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int commandAmount;
    std::cin >> commandAmount;
    Heap heap(MaxSize);
    for (int i = 0; i < commandAmount; ++i) {
        std::string command;
        std::cin >> command;
        if (command == "GetMin") {
            std::cout << heap.getMin() << '\n';
        }
        if (command == "GetMax") {
            std::cout << heap.getMax() << '\n';
        }
        if (command[0] == 'I') {
            int value = decipherThisTerribleInput(command);
            heap.insert(value);
        }
    }
}
