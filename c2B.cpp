    #include <iostream>
    #include <vector>
     
    struct border1{
        int val[10];
        bool type;
    };
    struct border2{
        int val;
        bool type;
    };
    namespace N {
        border1 x;
        border1 ans[20000];
        border1 numb[20000];
    }
    void stabsort(int n, int step) {
      if (step != 10) {
        using namespace N;
        if (step>0) {
          for (int i = 0; i < n; ++i) {
            numb[i] = ans[i];
          }
        }
        int cnt[10];
        for (int i = 0; i < 10; ++i) {
            cnt[i] = 0;
        }
        for (int i = 0; i < n; ++i) {
          ++cnt[numb[i].val[step]];
        }
        for (int i = 1; i <= 9; ++i) {
          cnt[i] += cnt[i-1];
        }
        for (int i = n - 1; i >= 0; --i) {
          ans[--cnt[numb[i].val[step]]]=numb[i];
        }
        stabsort(n, step + 1);
      }
    }
    int main()
    {
      using namespace N;
      for (int i = 0; i < 10; ++i) {
        x.val[i] = 0;
      }
      x.type = 0;
      for (int i = 0; i <= 10; ++i) {
        numb[i] = x ;
        ans[i] = x;
      }
      int n;
      std::cin >> n;
      for (int i = 0; i < n; ++i) {
        int l;
        std::cin >> l;
        int p=0;
        while (l>0) {
          numb[i].val[p] = l%10;
          l /= 10;
          ++p;
        }
        numb[i].type = 1;
        int r;
        std::cin >> r;
        p = 0;
        while (r>0) {
          numb[i+n].val[p] = r%10;
          r /= 10;
          ++p;
        }
        numb[i+n].type = 0;
      }
      stabsort(2*n, 0);
      border2 bord[20000];
      for (int i = 0; i < 2*n; ++i) {
        bord[i].val=0;
      }
      for (int i = 0; i < 2*n; ++i) {
        for (int j = 9; j >= 0; --j) {
            bord[i].val = bord[i].val * 10 + ans[i].val[j];
        }
        bord[i].type = ans[i].type;
      }
      int lay = 1;
      int start = bord[0].val;
      int sum = 0;
      for (int i = 1; i < 2*n; ++i) {
        if (lay == 1) {
          lay += (bord[i].type ? 1 : -1);
          sum += (bord[i].val - start);
          continue;
        }
        lay += (bord[i].type ? 1 : -1);
        if (lay == 1) {
            start = bord[i].val;
        }
      }
      std::cout << sum;
      return 0;
    }
