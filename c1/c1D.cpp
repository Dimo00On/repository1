#include <iostream>
#include <stack>
#include <queue>


void newVip(int& beginPartSize, int& endPartSize, int& vipSize, int number, int sum,
            std::queue<int>& beginPart, std::queue<int>& endPart, std::stack<int>& vip) {
    if (sum % 2) {
        vip.push(number);
        ++vipSize;
        return;
    }
    beginPart.push(number);
    ++beginPartSize;
}
void newGoblin(int& beginPartSize, int& endPartSize, int& vipSize, int number, int sum,
               std::queue<int>& beginPart, std::queue<int>& endPart, std::stack<int>& vip) {
    endPart.push(number);
    if (!(sum % 2)) {
        if (vipSize) {
            beginPart.push(vip.top());
            vip.pop();
            --vipSize;
        }
        else {
            beginPart.push(endPart.front());
            endPart.pop();
            --endPartSize;
        }
        ++beginPartSize;
    }
    ++endPartSize;
}
void firstLeaves(int& beginPartSize, int& endPartSize, int& vipSize, int sum,
                 std::queue<int>& beginPart, std::queue<int>& endPart, std::stack<int>& vip) {
    beginPart.pop();
    --beginPartSize;
    --sum;
    if (sum % 2) {
        if (vipSize) {
            beginPart.push(vip.top());
            vip.pop();
            --vipSize;
        } else {
            beginPart.push(endPart.front());
            endPart.pop();
            --endPartSize;
        }
        ++beginPartSize;
    }
}
int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::stack<int> vip;
    std::queue<int> beginPart;
    std::queue<int> endPart;
    int commandAmount;
    int beginPartSize = 0;
    int endPartSize = 0;
    int vipSize = 0;
    std::cin >> commandAmount;
    for (int i = 0; i < commandAmount; ++i) {
        char command;
        std::cin >> command;
        int sum = beginPartSize + endPartSize + vipSize;
        if (command == '*') {
            int number;
            std::cin >> number;
            newVip(beginPartSize, endPartSize, vipSize, number, sum, beginPart, endPart, vip);
        }
        if (command == '+') {
            int number;
            std::cin >> number;
            newGoblin(beginPartSize, endPartSize, vipSize, number, sum, beginPart, endPart, vip);
        }
        if (command == '-') {
            std::cout << beginPart.front() << '\n';
            firstLeaves(beginPartSize, endPartSize, vipSize, sum, beginPart, endPart, vip);
        }
    }
}
