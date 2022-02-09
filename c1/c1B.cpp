#include <iostream>
#include <string>

const int BracketsTypesAmount = 3;
const char Brackets[BracketsTypesAmount] = {')', ']', '}'};
const char ReverseBrackets[BracketsTypesAmount] = {'(', '[', '{'};
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
void antiPush(char nextBracket, bool& error, Stack& bracketSequence, int index) {
    if (!isStackEmpty(bracketSequence) && back(bracketSequence) == ReverseBrackets[index]) {
        pop(bracketSequence);
    } else {
        error = true;
    }
}
void check(char nextBracket, bool& error, Stack& bracketSequence) {
    if (nextBracket == '(' || nextBracket == '[' || nextBracket == '{') {
        push(nextBracket, bracketSequence);
    }
    if (nextBracket == ')' || nextBracket == ']' || nextBracket == '}') {
        int index = 0;
        for (int i = 0; i < BracketsTypesAmount; ++i) {
            if (Brackets[i] == nextBracket) {
                index = i;
            }
        }
        antiPush(nextBracket, error, bracketSequence, index);
    }
}
int main(){
    std::string newBracketSequence;
    std::cin >> newBracketSequence;
    Stack bracketSequence;
    for (char nextBracket : newBracketSequence) {
        bool error = false;
        check(nextBracket, error, bracketSequence);
        if (error) {
            std::cout << "no";
            return 0;
        }
    }
    std::cout << (isStackEmpty(bracketSequence) ? "yes" : "no");
}
