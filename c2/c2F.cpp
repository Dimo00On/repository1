#include <iostream>
#include <vector>
#include <algorithm>

const int MaxHeapSize = 500000;
struct Binary{
private:
    std::vector<int> value;
    std::vector<long long> operation;
    int size;
    std::vector<int> position;
public:
    void siftUp(int index);
    void insert(int value, long long operation);
    void siftDown(int index);
    void decreaseKey(long long hisOperation, int delta);
    int getMin();
    Binary() = default;
    explicit Binary(int newSize) {
        size = 0;
        position.resize(1);
        value.resize(newSize + 1);
        operation.resize(newSize + 1);
    }
};
struct cell{
    long long partNumber;
    long long partPosition;
    cell() = default;
    cell(long long newPartNumber, long long newPartPosition) {
        partNumber = newPartNumber;
        partPosition = newPartPosition;
    }
};
bool comparator(cell cage1, cell cage2) {
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
    position.push_back(0);
    ++size;
    value[size] = newValue;
    operation[size] = newOperation;
    position[newOperation] = size;
    siftUp(size);
}
int Binary::getMin() {
    return value[1];
}
void changeBlackNumber(Binary& minHeap, Binary& maxHeap, long long operation, int delta) {
    minHeap.decreaseKey(operation, delta);
    maxHeap.decreaseKey(operation, -delta);
}
void allLogic(std::vector<cell>& cage, std::vector<int>& blackNumber, Binary& minHeap, Binary& maxHeap, long long& cut,
              long long partAmount, long long length, long long cageAmount, int& minimal, long long& cutAmount) {
    sort(cage.begin(), cage.end(), comparator);
    for (int i = 0; i < partAmount; ++i) {
        minHeap.insert(blackNumber[i], i);
        maxHeap.insert(-blackNumber[i], i);
    }
    minimal = -maxHeap.getMin();
    minimal -= minHeap.getMin();
    cut = 0; // между клетками length и 1
    cutAmount = cage[1].partPosition;
    cell fictitiousCage(-1, length / partAmount);
    cage.push_back(fictitiousCage);
    for (int i = 1; i < cageAmount + 1; ++i) {
        //делаем разрез после cage[i].partPosition клетки
        changeBlackNumber(minHeap, maxHeap, cage[i].partNumber, 1);
        if (cage[i].partNumber == 0) {
            changeBlackNumber(minHeap, maxHeap, partAmount - 1, -1);
        } else {
            changeBlackNumber(minHeap, maxHeap, cage[i].partNumber - 1, -1);
        }
        if (cage[i + 1].partPosition != cage[i].partPosition) {
            int newMinimal = -maxHeap.getMin();
            newMinimal -= minHeap.getMin();
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
}
void fillArray(std::vector<cell>& cage, int step, long long number, long long length, long long partAmount,
               std::vector<int>& blackNumber) {
    cage[step].partPosition = number % (length / partAmount);
    if (cage[step].partPosition == 0) {
        cage[step].partPosition = length / partAmount;
    }
    cage[step].partNumber = (number - 1) / (length / partAmount);
    ++blackNumber[cage[step].partNumber];
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    Binary minHeap(MaxHeapSize);
    Binary maxHeap(MaxHeapSize);
    long long length;
    long long cageAmount;
    long long partAmount;
    std::cin >> length >> cageAmount >> partAmount;
    std::vector<cell> cage(cageAmount + 1);
    cell zeroCell(-1, 0);
    cage[0] = zeroCell;
    std::vector<int> blackNumber(partAmount, 0);
    for (int i = 1; i < cageAmount + 1; ++i) {
        long long number;
        std::cin >> number;
        fillArray(cage, i, number, length, partAmount, blackNumber);
    }
    int minimal;
    long long cut;
    long long cutAmount;
    allLogic(cage, blackNumber, minHeap, maxHeap, cut, partAmount, length, cageAmount, minimal, cutAmount);
    std::cout << minimal << " " << cutAmount * partAmount << '\n' << cut;
}
