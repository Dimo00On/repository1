#include <iostream>
#include <vector>

const long long mod230 = 1 << 30;
const int MaxNeedSize = 1000;
struct Heap{
private:
    std::vector<int> value;
    int size;
public:
    void siftUp(int index);
    void siftDown(int index);
    int getMin();
    void insert(int newValue);
    void extractMin();
    int length();
    Heap() = default;
    explicit Heap(int newSize) {
        value.resize(newSize + 2);
        size = 0;
    }

};
void Heap::siftUp(int index) {
    while (index != 1) {
        if (value[index] < value[index / 2]) {
            std::swap(value[index], value[index / 2]);
            index /= 2;
        } else {
            break;
        }
    }
}
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
int Heap::getMin(){
    return value[1];
}
void Heap::insert(int newValue) {
    value[++size] = newValue;
    siftUp(size);
}
void Heap::extractMin(){
    value[1] = value[size--];
    siftDown(1);
}
int Heap::length() {
    return size;
}
void allLogic(Heap& heap, int amount, int needSize, long long sequenceElement, long long x, long long y, std::vector<int>& answer) {
    for (int i = 0; i < amount; ++i) {
        sequenceElement = ((x * sequenceElement) % mod230 + y) % mod230;
        heap.insert(static_cast<int>(-sequenceElement));
        if (heap.length() > needSize) {
            heap.extractMin();
        }
    }
    for (int i = 0; i < needSize; ++i) {
        answer[i] = -heap.getMin();
        heap.extractMin();
    }
}
int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int amount, needSize;
    long long sequenceElement;
    long long x, y; //from task description
    std::cin >> amount >> needSize >> sequenceElement >> x >> y;
    Heap heap(MaxNeedSize);
    std::vector<int> answer(needSize);
    allLogic(heap, amount, needSize, sequenceElement, x, y, answer);
    for (int i = needSize - 1; i >= 0; --i) {
        std::cout << answer[i] << " ";
    }
}
