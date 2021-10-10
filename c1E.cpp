    #include <iostream>
    #include <vector>
     
     
     
    int main(){
        int n,mm,x,y,q,ll,t;
        std::vector <int> a[900];
        std::vector <int> b[900];
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(NULL);
        std::cin >> n >> mm >> ll;
        for (int i=0;i<n;++i) {
            for (int j=0;j<ll;++j) {
                std::cin >> t; a[i].push_back(t);
            }
        }
        for (int i=0;i<mm;++i) {
            for (int j=0;j<ll;++j) {
                std::cin >> t; b[i].push_back(t);
            }
        }
        std::cin >> q;
        for (int g=0;g<q;++g) {
            std::cin >> x >> y;
            x--; y--;
            int l=0; int r=ll-1; int m;
            while (r-l>1) {
                m=(r+l)/2;
                if (a[x][m]>b[y][m]) {r=m;}
                else {l=m;}
            }
            if (std::max(a[x][l],b[y][l])<std::max(a[x][r],b[y][r])) {m=l;}
            else {m=r;}
            std::cout << m+1 << '\n';
        }
        return 0;
    }
