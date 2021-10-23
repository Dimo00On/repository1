#include <iostream>
#include <stack>
#include <queue>
#include <string>

void pleaseSolveThisTask(std::string uselessString) {
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
        int number;
        std::cin >> command;
        int sum = beginPartSize + endPartSize + vipSize;
        if (command == '*') {
            std::cin >> number;
            if (sum % 2) {
                vip.push(number);
                ++vipSize;
            } else {
                beginPart.push(number);
                ++beginPartSize;
            }
        }
        if (command == '+') {
            std::cin >> number;
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
        if (command == '-') {
            std::cout << beginPart.front() << std::endl;
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
    }
}
int main(){
    pleaseSolveThisTask("D task");
}
