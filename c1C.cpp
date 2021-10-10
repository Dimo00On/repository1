    #include <iostream>
    #include <stack>
     
     
     
    int main(){
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(NULL);                   
        struct q{
            long long znach;
            long long lev;
            long long on;
        };
        q h;
        long long n,x,potm;
        std::stack <q> a;
        std::cin >> n;
        std::cin >> x;
        h.znach=-100; h.lev=-1; h.on=0; a.push(h);
        h.znach=x; h.lev=0; h.on=1; a.push(h);
        long long m1=x;
        for (long long i=1;i<n;++i) {
            std::cin >> x;
            h=a.top();
            if (x>h.znach) {h.znach=x; h.lev=h.on; h.on=i+1; a.push(h);}
            else {
                while (x<=h.znach) {
                    potm =(i-h.lev)*h.znach;
                    if (m1<potm) {m1=potm;}
                    a.pop();
                    h=a.top();
                }
                h.znach=x; h.lev=h.on; h.on=i+1; a.push(h);
            }
        }
        x=0;
        h=a.top();
        while (x<=h.znach) {
            potm =(n-h.lev)*h.znach;
            if (m1<potm) {m1=potm;}
            a.pop();
            h=a.top();
        }
        std::cout << m1 << '\n';
        return 0;
  }
