    #include <iostream>
    #include <string>
     
    struct type {
      int val;
      type* last;
    };
     
    namespace stack {
      type* myStack = nullptr;
      int size;
    };
     
    void push(int x, type* myStack){
      type* n = new type();
      n->last = myStack;
      n->val = x;
      stack::myStack = n;
      stack::size++;
    };
     
    void pop(type* myStack){
      type* n = stack::myStack->last;
      delete stack::myStack;
      stack::myStack = n;
      stack::size--;
    };
     
    void back(type* myStack){
      if (stack::myStack == nullptr) {
        std::cout << "error" << std::endl;
      } else {
        std::cout << (stack::myStack->val) << std::endl;
      }
    };
     
    bool empty(type* myStack){
      if (stack::myStack == nullptr) {
        return true;
      }
      return false;
    };
     
    void size(type* myStack){
      std::cout << stack::size << std::endl;
    };
     
    void clear(type* myStack){
      if (!empty(stack::myStack)) {
        pop(stack::myStack);
        clear(stack::myStack);
      }
    };
     
    int main(){
      while (true) {
        std::string com;
        std::cin >> com;
        if (com == "exit") {
          std::cout << "bye";
          return 0;
        }
        if (com == "clear") {
          clear(stack::myStack);
          std::cout << "ok" << std::endl;
        }
        if (com == "size") {
          size(stack::myStack);
        }
        if (com == "back") {
          back(stack::myStack);
        }
        if (com == "pop") {
          back(stack::myStack);
          if (!empty(stack::myStack)) {
            pop(stack::myStack);
          }
        }
        if (com == "push") {
          std::cout << "ok" << std::endl;
          int newVal;
          std::cin >> newVal;
          push(newVal, stack::myStack);
        }
      }
    }
