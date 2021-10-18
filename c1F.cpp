    #include <iostream>
    #include <vector>
    #include <fstream>
     
    void merge(int left_1, int right_1, int left_2, int right_2, std::vector <int> &a, long long &answer) {
      int sizeB = right_1 - left_1 + 1;
      int sizeC = right_2 - left_2 + 1;
      std::vector <int> b(sizeB);
      std::vector <int> c(sizeC);
      for (int i = left_1; i <= right_1; ++i) {
        b[i - left_1] = a[i];
      }
      for (int i = left_2; i <= right_2; ++i) {
        c[i - left_2] = a[i];
      }
      int i = 0;
      int j = 0;
      int p = left_1;
      while (i < sizeB && j < sizeC) {
        if (b[i] <= c[j]) {
          a[p++] = b[i];
          ++i;
        } else {
          a[p++] = c[j];
          ++j;
          answer += (sizeB - i);
        }
      }
      while (i < sizeB) {
        a[p++] = b[i];
        ++i;
      }
      while (j < sizeC) {
        a[p++] = c[j];
        ++j;
      }
      b.clear();
      c.clear();
    }
    void mergesort(int left, int right, std::vector <int> &a, long long &answer){
      if (right != left) {
        int mid = (left + right) / 2;
        mergesort(left, mid, a, answer);
        mergesort(mid+1, right, a, answer);
        merge(left, mid, mid+1, right, a, answer);
      }
    }
    int main(){
      std::ifstream fin("inverse.in");
      std::ios_base::sync_with_stdio(false);
      int n;
      fin >> n;
      std::vector <int> a(n);
      for (int i = 0; i < n; ++i) {
        fin >> a[i];
      }
      long long answer = 0;
      mergesort(0, n-1, a, answer);
      std::ofstream fout("inverse.out");
      fout << answer;
      return 0;
    }
