    #include <iostream>
    #include <vector>
    #include <string>
     
    struct Keyheap{
        long long val;
        int ind;
    };
    namespace Heap{
      Keyheap myHeap[100001];
      int n = 0;
      std::vector<int> ind;
    }
    void siftUp(int v) {
      using namespace Heap;
      while (v!=1) {
        if (myHeap[v].val<myHeap[v/2].val) {
          ind[myHeap[v].ind]=v/2;
          ind[myHeap[v/2].ind]=v;
          std::swap(myHeap[v], myHeap[v/2]);
          v /= 2;
        } else {
          break;
        }
      }
    }
    void siftDown(int v) {
      using namespace Heap;
      while (2*v <= n) {
        int newv = 2*v;
        if (2*v+1 <= n && myHeap[2*v+1].val < myHeap[2*v].val) {
          newv = 2*v+1;
        }
        if (myHeap[newv].val < myHeap[v].val) {
          ind[myHeap[v].ind] = newv;
          ind[myHeap[newv].ind] = v;
          std::swap(myHeap[newv], myHeap[v]);
          v = newv;
        } else {
          break;
        }
      }
    }
    long long getMin(){
      return Heap::myHeap[1].val;
    }
    void decreaseKey(int iX, int delta) {
      using namespace Heap;
      myHeap[ind[iX]].val -= delta;
      siftUp(ind[iX]);
    }
    void insert(int x, int iX) {
      using namespace Heap;
      myHeap[++n].val=x;
      myHeap[n].ind=iX;
      ind[iX]=n;
      siftUp(n);
    }
    void extractMin(){
      using namespace Heap;
      myHeap[1]=myHeap[n--];
      ind[myHeap[1].ind]=1;
      siftDown(1);
    }
    int main(){
      using namespace Heap;
      std::ios_base::sync_with_stdio(false);
      std::cin.tie(NULL);
      int q;
      std::cin >> q;
      ind.push_back(0);
      //myHeap[0].val=-1;
      for (int i = 1; i <= q; ++i) {
        std::string com;
        std::cin >> com;
        ind.push_back(0);
        if (com == "insert") {
          int val;
          std::cin >> val;
          insert(val, i);
        }
        if (com == "getMin") {
          std::cout << getMin() << '\n';
        }
        if (com == "extractMin") {
          extractMin();
        }
        if (com == "decreaseKey") {
          int iX;
          int delta;
          std::cin >> iX >> delta;
          decreaseKey(iX, delta);
        }
        /*for (int j=1; j<=n; ++j) {
          std::cout << myHeap[j].val << "," << myHeap[j].ind << ";" << ind[myHeap[j].ind] << " ";
        }
        std::cout << '\n';*/
     
      }
    }
