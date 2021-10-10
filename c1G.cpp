    #include <iostream>
    #include <vector>
     
     
    int main()
    {
        long long n,p,q,maxa;
        maxa=0;
        std::cin >> n >> p >> q;
        std::vector <long long> a(n);
        for (int i=0;i<n;++i) {
            std::cin >> a[i];
            if (a[i]>maxa) {maxa=a[i];}
        }
        if (p==q) {std::cout << (maxa+p-1)/q; return 0;}
        long long l=0;
        long long r=1e9;
        while (r-l>1) {
            long long m=(r+l)/2;
            long long kolyd=0;
            for (int i=0;i<n;++i) {
                kolyd=kolyd+(std::max((a[i]-m*q),(long long)0)+(p-q)-1)/(p-q);
            }
            if (kolyd>m) {l=m;}
            else {r=m;}
        }
        std::cout << r;
        return 0;
    }
