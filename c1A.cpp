#include <iostream>
#include <string>

struct Node {
    int value;
    Node* last;
};
struct Stack {
    Node* tale = nullptr;
    int size;
};

void push(int newValue, Stack& myStack){
    Node* newTale = new Node();
    newTale->last = myStack.tale;
    newTale->value = newValue;
    myStack.tale = newTale;
    ++myStack.size;
};

void pop(Stack& myStack){
    Node* newTale = myStack.tale->last;
    delete myStack.tale;
    myStack.tale = newTale;
    --myStack.size;
};

void back(Stack& myStack){
    if (myStack.tale == nullptr) {
        std::cout << "error" << std::endl;
    } else {
        std::cout << (myStack.tale->value) << std::endl;
    }
};

bool isStackEmpty(Stack& myStack){
    if (myStack.tale == nullptr) {
        return true;
    }
    return false;
};

void size(Stack& myStack){
    std::cout << myStack.size << std::endl;
};

void clear(Stack& myStack){
    while (!isStackEmpty(myStack)){
        pop(myStack);
    }
};
void pleaseSolveThisTask(std::string uselessString){
    Stack myStack;
    myStack.size = 0;
    while (true) {
        std::string command;
        std::cin >> command;
        if (command == "exit") {
            std::cout << "bye";
            return;
        }
        if (command == "clear") {
            clear(myStack);
            std::cout << "ok" << std::endl;
        }
        if (command == "size") {
            size(myStack);
        }
        if (command == "back") {
            back(myStack);
        }
        if (command == "pop") {
            back(myStack);
            if (!isStackEmpty(myStack)) {
                pop(myStack);
            }
        }
        if (command == "push") {
            std::cout << "ok" << std::endl;
            int newValue;
            std::cin >> newValue;
            push(newValue, myStack);
        }
    }
}
int main(){
    pleaseSolveThisTask("A task");
    return 0;
}
