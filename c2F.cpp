#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

const int MaxHeapSize = 500000;
struct Binary{
    int* value = new int[MaxHeapSize + 1];
    long long* operation = new long long[MaxHeapSize + 1];
    int size = 0;
    std::vector<int> position;
    void siftUp(int index);
    void insert(int value, long long operation);
    void siftDown(int index);
    void extractMin();
    void decreaseKey(long long hisOperation, int delta);
};
struct cell{
    long long partNumber;
    long long partPosition;
};
bool isCage1LessCage2(cell cage1, cell cage2) {
    if (cage1.partPosition == cage2.partPosition) {
        return cage1.partNumber < cage2.partNumber;
    }
    return cage1.partPosition < cage2.partPosition;
}
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
void Binary::decreaseKey(long long hisOperation, int delta) {
    value[position[hisOperation]] -= delta;
    if (delta > 0) {
        siftUp(position[hisOperation]);
    } else {
        siftDown(position[hisOperation]);
    }
}
void Binary::insert(int newValue, long long newOperation) {
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
int heapDo(const std::string& command, Binary& minHeap, Binary& maxHeap, long long operationNumber, int value, int delta, long long operation){
    if (command == "insert") {
        minHeap.position.push_back(0);
        maxHeap.position.push_back(0);
        minHeap.insert(value, operationNumber);
        maxHeap.insert(-value, operationNumber);
    }
    if (command == "getMin") {
        return minHeap.value[1];
    }
    if (command == "getMax") {
        return -maxHeap.value[1];
    }
    if (command == "extractMin") {
        minHeap.extractMin();
    }
    if (command == "extractMax") {
        maxHeap.extractMin();
    }
    if (command == "decreaseKeyMin") {
        minHeap.decreaseKey(operation, delta);
    }
    if (command == "decreaseKeyMax") {
        maxHeap.decreaseKey(operation, delta);
    }
    return 0;
}
void changeBlackNumber(Binary& minHeap, Binary& maxHeap, long long operation, int delta) {
    heapDo("decreaseKeyMin", minHeap, maxHeap, 0, 0, delta, operation);
    heapDo("decreaseKeyMax", minHeap, maxHeap, 0, 0, -delta, operation);
}
void pleaseSolveThisTask() {
    Binary minHeap;
    Binary maxHeap;
    minHeap.position.push_back(0);
    maxHeap.position.push_back(0);
    long long length; // из условия задачи однобуквенные переменные
    long long cageAmount;
    long long partAmount;
    std::cin >> length >> cageAmount >> partAmount;
    std::vector<cell> cage(cageAmount + 1);
    cage[0].partPosition = 0;
    cage[0].partNumber = -1;
    std::vector<int> blackNumber(partAmount, 0);
    for (int i = 1; i < cageAmount + 1; ++i) {
        long long number;
        std::cin >> number;
        cage[i].partPosition = number % (length / partAmount);
        if (cage[i].partPosition == 0) {
            cage[i].partPosition = length / partAmount;
        }
        cage[i].partNumber = (number - 1) / (length / partAmount);
        ++blackNumber[cage[i].partNumber];
    }
    sort(cage.begin(), cage.end(), isCage1LessCage2);
    for (int i = 0; i < partAmount; ++i) {
        heapDo("insert", minHeap, maxHeap, i, blackNumber[i], 0, 0);
    }
    int minimal = heapDo("getMax", minHeap, maxHeap, 0, 0, 0, 0);
    minimal -= heapDo("getMin", minHeap, maxHeap, 0, 0, 0, 0);
    long long cut = 0; // между клетками length и 1
    long long cutAmount = cage[1].partPosition;
    cell fictitiousCage;
    cage.push_back(fictitiousCage);
    cage[cageAmount + 1].partPosition = length / partAmount;
    cage[cageAmount + 1].partNumber = -1;
    for (int i = 1; i < cageAmount + 1; ++i) {
        //делаем разрез после cage[i].partPosition клетки
        changeBlackNumber(minHeap, maxHeap, cage[i].partNumber, 1);
        if (cage[i].partNumber == 0) {
            changeBlackNumber(minHeap, maxHeap, partAmount - 1, -1);
        } else {
            changeBlackNumber(minHeap, maxHeap, cage[i].partNumber - 1, -1);
        }
        if (cage[i + 1].partPosition != cage[i].partPosition) {
            int newMinimal = heapDo("getMax", minHeap, maxHeap, 0, 0, 0, 0);
            newMinimal -= heapDo("getMin", minHeap, maxHeap, 0, 0, 0, 0);
            if (minimal == newMinimal) {
                cutAmount += (cage[i + 1].partPosition - cage[i].partPosition);
            }
            if (newMinimal < minimal) {
                minimal = newMinimal;
                cut = cage[i].partPosition;
                cutAmount = cage[i + 1].partPosition - cut;
            }
        }
    }
    if (cut == 0) {
        cut = length;
    }
    std::cout << minimal << " " << cutAmount * partAmount << '\n' << cut;
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    pleaseSolveThisTask();
}

