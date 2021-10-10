    #include <iostream>
    #include <string>
     
    struct tip {
        char znach;
        tip* pred;
    };
    int kol;
     
    tip* CTEK=nullptr;
     
    void push(int x, tip* &CTEK){
        tip* n=new tip();
        n->pred=CTEK;
        n->znach=x;
        CTEK=n;
        kol++;
    };
     
    void pop(tip* &CTEK){
        tip* n=CTEK->pred;
        delete CTEK;
        CTEK=n;
        kol--;
    };
     
    char back(tip* &CTEK){
        return (CTEK->znach);
    };
     
    bool empty(tip* &CTEK){
        if (CTEK==nullptr) {return true;}
        else {return false;}
    };
     
    int size(tip* &CTEK){
        return kol;
    };
     
    void clear(tip* &CTEK){
        if (!empty(CTEK)) {
            pop(CTEK);
            clear(CTEK);
        }
    };
     
    int main(){
        std::string PCP;
        std::cin >> PCP;
        for (char cat : PCP) {
            if (cat=='(' || cat=='[' || cat=='{') {
                push(cat, CTEK);
            }
            if (cat==')') {
                if (!empty(CTEK) && back(CTEK)=='(') {pop(CTEK);}
                else {
                    std::cout << "no"; return 0;
                }
            }
            if (cat==']') {
                if (!empty(CTEK) && back(CTEK)=='[') {pop(CTEK);}
                else {
                    std::cout << "no"; return 0;
                }
            }
            if (cat=='}') {
                if (!empty(CTEK) && back(CTEK)=='{') {pop(CTEK);}
                else {
                    std::cout << "no"; return 0;
                }
            }
        }
        if (empty(CTEK)) {std::cout << "yes";}
        else {std::cout << "no";}
        return 0;
    }
