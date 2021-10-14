    #include <iostream>
    #include <vector>
    #include <string>
     
    const int MaxSize = 100000;
    const int DigitStart = 48;
    const int FirstDigitIndex = 7;
     
    namespace MyHeap{
        int size = 0;
        std::vector<int> Heap(MaxSize + 1);
    }
     
    using MyHeap::Heap;
    using MyHeap::size;
     
    void SiftDown(int index) {
        while (2 * index <= size) {
            int newIndex = 2 * index;
            if (newIndex + 1 <= size && Heap[newIndex + 1] < Heap[newIndex]) {
                ++newIndex;
            }
            if (Heap[newIndex] < Heap[index]) {
                std::swap(Heap[newIndex], Heap[index]);
                index = newIndex;
            } else {
                break;
            }
        }
    }
    int SiftUp(int index) {
        while (index != 1) {
            if (Heap[index] < Heap[index/2]) {
                std::swap(Heap[index], Heap[index/2]);
                index /= 2;
            } else {
                break;
            }
        }
        return index;
    }
    void Insert(int value){
        ++size;
        Heap[size] = value;
        SiftUp(size);
    }
    int GetMin(){
        int min = Heap[1];
        Heap[1] = Heap[size];
        --size;
        SiftDown(1);
        return min;
    }
    int GetMax(){
        int max = 0;
        int index;
        for (int i = size; i >= size/2; --i) {
            if (Heap[i] > max) {
                max = Heap[i];
                index = i;
            }
        }
        if (Heap[index] > Heap[size]) {
            Heap[index] = Heap[size];
            SiftUp(index);
        } else {
            Heap[index] = Heap[size];
            SiftDown(index);
        }
        --size;
        return max;
    }
    int main(){
        Heap[0] = 0;
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(NULL);
        int commandAmount;
        std::cin >> commandAmount;
        for (int i = 0; i < commandAmount; ++i) {
            std::string command;
            std::cin >> command;
            if (command == "GetMin") {
                std::cout << GetMin() << '\n';
            }
            if (command == "GetMax") {
                std::cout << GetMax() << '\n';
            }
            if (command[0] == 'I') {
                int value = 0;
                int point = FirstDigitIndex;
                while (command[point] != ')') {
                    value = value * 10 + command[point] - DigitStart;
                    point++;
                }
                Insert(value);
            }
        }
    }
