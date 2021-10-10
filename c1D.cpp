    #include <iostream>
    #include <stack>
    #include <queue>
     
     
    int main(){
        std::stack <int> s;
        std::queue <int> q1,q2;
        std::vector <int> otvet;
        int n,k1,k2,k;
        k1=0; k2=0; k=0;
        std::cin >> n;
        for (int i=0;i<n;++i) {
            char op;
            int x;
            std::cin >> op;
            if (op=='*') {
                std::cin >> x;
                if ((k1+k2+k)%2==1) {s.push(x);k++;}
                else {q1.push(x);k1++;}
            }
            if (op=='+') {
                std::cin >> x;
                q2.push(x);
                if ((k1+k2+k)%2==0) {
                    if (k!=0) {q1.push(s.top());s.pop();k--;k1++;}
                    else {q1.push(q2.front());q2.pop();k2--;k1++;}
                }
                k2++;
            }
            if (op=='-') {
                otvet.push_back(q1.front());
                q1.pop();
                k1--;
                if ((k1+k2+k)%2==1) {
                    if (k!=0) {q1.push(s.top());s.pop();k--;k1++;}
                    else {q1.push(q2.front());q2.pop();k2--;k1++;}
                }
            }
        }
        for (int i=0;i<otvet.size();++i) {
            std::cout << otvet[i] << std::endl;
        }
    }
