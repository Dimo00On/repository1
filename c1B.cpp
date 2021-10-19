#include <iostream>
#include <string>

struct Node {
    char value;
    Node* last;
};
struct Stack {
    int size = 0;
    Node* tale = nullptr;
};
void push(char newBracket, Stack& bracketSequence){
    Node* newTale = new Node();
    newTale->last = bracketSequence.tale;
    newTale->value = newBracket;
    bracketSequence.tale=newTale;
    ++bracketSequence.size;
};
void pop(Stack& bracketSequence){
    Node* newTale = bracketSequence.tale->last;
    delete bracketSequence.tale;
    bracketSequence.tale = newTale;
    --bracketSequence.size;
};
char back(const Stack& bracketSequence){
    return (bracketSequence.tale->value);
};
bool isStackEmpty(const Stack& bracketSequence){
    if (bracketSequence.tale == nullptr) {
        return true;
    }
    return false;
};
void pleaseSolveThisTask(std::string uselessString) {
    std::string newBracketSequence;
    std::cin >> newBracketSequence;
    Stack bracketSequence;
    for (char nextBracket : newBracketSequence) {
        if (nextBracket == '(' || nextBracket == '[' || nextBracket == '{') {
            push(nextBracket, bracketSequence);
        }
        char lastBracket = back(bracketSequence);
        if (nextBracket == ')') {
            if (!isStackEmpty(bracketSequence) && lastBracket == '(') {
                pop(bracketSequence);
            } else {
                std::cout << "no";
                return;
            }
        }
        if (nextBracket == ']') {
            if (!isStackEmpty(bracketSequence) && lastBracket == '[') {
                pop(bracketSequence);
            } else {
                std::cout << "no";
                return;
            }
        }
        if (nextBracket == '}') {
            if (!isStackEmpty(bracketSequence) && lastBracket == '{') {
                pop(bracketSequence);
            } else {
                std::cout << "no";
                return;
            }
        }
    }
    if (isStackEmpty(bracketSequence)) {
        std::cout << "yes";
    } else {
        std::cout << "no";
    }
    return;
}
int main(){
    pleaseSolveThisTask("B task");
    return 0;
}
