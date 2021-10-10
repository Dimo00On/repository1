    #include <iostream>
    #include <vector>
     
    namespace N{
        std::vector <int> a;
        int less;
        int equal;
    }
    void partition(int l, int r, int x){
      using namespace N;
      std::vector <int> b;
      less = 0;
      equal = 0;
      for (int i = l; i < r; ++i) {
        if (a[i] < x) {
          b.push_back(a[i]);
          ++less;
        }
      }
      for (int i = l; i < r; ++i) {
        if (a[i] == x) {
          b.push_back(a[i]);
          ++equal;
        }
      }
      for (int i = l; i < r; ++i) {
        if (a[i] > x) {
          b.push_back(a[i]);
        }
      }
      for (int i = l, j = 0; i < r; ++i, ++j) {
        a[i]=b[j];
      }
      b.clear();
    }
     
    int QuickSelect(int l, int r, int k) {
      using namespace N;
      if (r == l + 1) {
        return a[l];
      }
      int ipiv=(rand()*32768+rand())%(r-l);
      int piv = a[ipiv+l];
      partition(l, r, piv);
      if (k < less) {
        return QuickSelect(l, l+less, k);
      }
      if (k < less + equal) {
        return piv;
      }
      return QuickSelect(l+less+equal, r, k-less-equal);
    }
    int main(){
      using namespace N;
      int n, k;
      std::ios_base::sync_with_stdio(false);
      std::cin.tie(NULL);
      std::cin >> n >> k;
      for (int i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        a.push_back(x);
      }
      std::cout << QuickSelect(0, n, k);
    }
