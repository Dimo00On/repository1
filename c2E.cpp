    #include <iostream>
    #include <vector>
     
    const int mod230 = 1 << 30;
    const int MaxNeedSize = 1000;
     
    namespace Heap{
        int myHeap[MaxNeedSize + 1];
        int size = 0;
    }
    void siftUp(int index) {
        using namespace Heap;
        while (index != 1) {
            if (myHeap[index] < myHeap[index / 2]) {
                std::swap(myHeap[index], myHeap[index / 2]);
                index /= 2;
            } else {
                break;
            }
      }
    }
    void siftDown(int index) {
        using namespace Heap;
        while (2 * index <= size) {
            int newIndex = 2 * index;
            if (newIndex + 1 <= size && myHeap[newIndex + 1] < myHeap[newIndex]) {
                ++newIndex;
            }
            if (myHeap[newIndex] < myHeap[index]) {
                std::swap(myHeap[newIndex], myHeap[index]);
                index = newIndex;
            } else {
                break;
            }
        }
    }
    int getMin(){
        return Heap::myHeap[1];
    }
    void insert(int x) {
        using namespace Heap;
        myHeap[++size] = x;
        siftUp(size);
    }
    void extractMin(){
        using namespace Heap;
        myHeap[1] = myHeap[size--];
        siftDown(1);
    }
    int main(){
        using namespace Heap;
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(NULL);
        int amount;
        int needSize;
        long long a;
        long long x;
        long long y;
        std::cin >> amount >> needSize >> a >> x >> y;
        for (int i = 0; i < amount; ++i) {
            a = ((x * a) % mod230 + y) % mod230;
            insert(-a);
            if (size > needSize) {
                extractMin();
            }
        }
        std::vector<int> answer(needSize);
        for (int i = 1; i <= needSize; ++i) {
            answer[i - 1] = -getMin();
            extractMin();
        }
        for (int i = needSize - 1; i >= 0; --i) {
            std::cout << answer[i] << " ";
        }
    }
