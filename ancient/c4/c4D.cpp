#include <iostream>
#include <string>
#include <fstream>
#include <vector>

const long long Infinity = 1e18 + 42;

struct Node {
private:
    Node* parent;
    Node* left;
    Node* right;
    long long sum;
    long long value;
    int amount;
public:
    friend Node* merge(Node* first, Node* second, Node& Nul);
    friend std::pair<Node*, Node*> split(long long newValue, Node& Nul, Node* tree);
    Node();
    Node(long long newValue, Node* nul);
    Node* insert(long long newValue, Node& Nul);
    Node* erase(long long newValue, Node& Nul);
    Node* getSum(long long leftBorder, long long rightBorder, long long& answer, Node& Nul);
    Node* getIndex(int index, long long& answer, Node& Nul);
    void splay(Node& Nul);
    void zigzig(Node& Nul);
    void zigzag(Node& Nul);
    void zig(Node& Nul);
    Node* findFirstLessOrEqual(long long newValue, Node& Nul);
};

Node::Node() {
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    sum = 0;
    value = Infinity;
    amount = 0;
}

Node::Node(long long newValue, Node* nul) {
    parent = nul;
    left = nul;
    right = nul;
    sum = newValue;
    value = newValue;
    amount = 1;
}

void Node::zigzig(Node& Nul) { //temp максимум splayTree, не выше
    if (parent->left == this) {
        if (right != &Nul) {
            right->parent = parent;
        }
        if (parent->right !=  &Nul) {
            parent->right->parent = parent->parent;
        }
        parent->parent->left = parent->right;
        parent->left = right;
        Node* temp = parent->parent->parent;
        parent->parent->parent = parent;
        parent->right = parent->parent;
        right = parent;
        parent->parent = this;
        parent = temp;
        if (temp != &Nul) {
            if (temp->left == right->right) {
                temp->left = this;
            } else {
                temp->right = this;
            }
        }
        right->right->sum = right->right->right->sum + right->right->left->sum + right->right->value;
        right->right->amount = right->right->right->amount + right->right->left->amount + 1;
        right->sum = right->right->sum + right->left->sum + right->value;
        right->amount = right->right->amount + right->left->amount + 1;
        sum = right->sum + left->sum + value;
        amount = right->amount + left->amount + 1;
    } else {
        if (left != &Nul) {
            left->parent = parent;
        }
        if (parent->left !=  &Nul) {
            parent->left->parent = parent->parent;
        }
        parent->parent->right = parent->left;
        parent->right = left;
        Node* temp = parent->parent->parent;
        parent->parent->parent = parent;
        parent->left = parent->parent;
        left = parent;
        parent->parent = this;
        parent = temp;
        if (temp != &Nul) {
            if (temp->right == left->left) {
                temp->right = this;
            } else {
                temp->left = this;
            }
        }
        left->left->sum = left->left->left->sum + left->left->right->sum + left->left->value;
        left->left->amount = left->left->left->amount + left->left->right->amount + 1;
        left->sum = left->left->sum + left->right->sum + left->value;
        left->amount = left->left->amount + left->right->amount + 1;
        sum = right->sum + left->sum + value;
        amount = right->amount + left->amount + 1;
    }
}

void Node::zigzag(Node& Nul) {
    if (parent->right == this) {
        if (left != &Nul) {
            left->parent = parent;
        }
        if (right != &Nul) {
            right->parent = parent->parent;
        }
        parent->right = left;
        parent->parent->left = right;
        left = parent;
        right = parent->parent;
        if (parent->parent->parent != &Nul) {
            if (parent->parent->parent->left == parent->parent) {
                parent->parent->parent->left = this;
            } else {
                parent->parent->parent->right = this;
            }
        }
        Node* temp = parent->parent->parent;
        parent->parent->parent = this;
        parent->parent = this;
        parent = temp;
    } else {
        if (right != &Nul) {
            right->parent = parent;
        }
        if (left != &Nul) {
            left->parent = parent->parent;
        }
        parent->left = right;
        parent->parent->right = left;
        right = parent;
        left = parent->parent;
        if (parent->parent->parent != &Nul) {
            if (parent->parent->parent->right == parent->parent) {
                parent->parent->parent->right = this;
            } else {
                parent->parent->parent->left = this;
            }
        }
        Node* temp = parent->parent->parent;
        parent->parent->parent = this;
        parent->parent = this;
        parent = temp;
    }
    right->sum = right->right->sum + right->left->sum + right->value;
    right->amount = right->right->amount + right->left->amount + 1;
    left->sum = left->right->sum + left->left->sum + left->value;
    left->amount = left->right->amount + left->left->amount + 1;
    sum = left->sum + right->sum + value;
    amount = left->amount + right->amount + 1;
}

void Node::zig(Node& Nul) {
    if (parent->left == this) {
        parent->left = right;
        if (right != &Nul) {
            right->parent = parent;
        }
        Node* temp = parent->parent;
        parent->parent = this;
        if (temp != &Nul) {
            if (temp->left == parent->parent) {
                temp->left = this;
            } else {
                temp->right = this;
            }
        }
        right = parent;
        parent = temp;
        sum = right->sum;
        amount = right->amount;
        right->sum = right->right->sum + right->left->sum + right->value;
        right->amount = right->right->amount + right->left->amount + 1;
    } else {
        parent->right = left;
        if (left != &Nul) {
            left->parent = parent;
        }
        Node* temp = parent->parent;
        parent->parent = this;
        if (temp != &Nul) {
            if (temp->right == parent->parent) {
                temp->right = this;
            } else {
                temp->left = this;
            }
        }
        left = parent;
        parent = temp;
        sum = left->sum;
        amount = left->amount;
        left->sum = left->right->sum + left->left->sum + left->value;
        left->amount = left->right->amount + left->left->amount + 1;
    }
}

void Node::splay(Node& Nul) {
    while(true) {
        if (value == Infinity || parent->value == Infinity) {
            return;
        }
        if (parent->parent->value == Infinity) {
            zig(Nul);
            return;
        }
        Node *temp = parent;
        if ((temp->parent->left == temp && temp->left == this) || (temp->parent->right == temp && temp->right == this)) {
            zigzig(Nul);
        } else {
            zigzag(Nul);
        }
    }
}

Node* merge(Node* first, Node* second, Node& Nul) {
    if (first == &Nul) {
        return second;
    }
    if (second == &Nul) {
        return first;
    }
    Node* temp = first;
    while (temp->right != &Nul) {
        temp = temp->right;
    }
    temp->splay(Nul);
    temp->right = second;
    second->parent = temp;
    temp->sum += second->sum;
    temp->amount += second->amount;
    return temp;
}

Node* Node::findFirstLessOrEqual(long long newValue, Node& Nul) {
    if (value == Infinity) {
        return &Nul;
    }
    if (value == newValue) {
        return this;
    }
    if (value < newValue) {
        if (right != &Nul) {
            Node* temp = right->findFirstLessOrEqual(newValue, Nul);
            if (temp != &Nul) {
                return temp;
            }
        }
        return this;
    }
    if (left != &Nul) {
        return left->findFirstLessOrEqual(newValue, Nul);
    }
    return &Nul;
}

std::pair<Node*, Node*> split(long long newValue, Node& Nul, Node* tree) {
    std::pair<Node*, Node*> answer;
    if (tree == &Nul) {
        answer.first = &Nul;
        answer.second = &Nul;
        return answer;
    }
    Node* temp = tree->findFirstLessOrEqual(newValue, Nul);
    if (temp == &Nul) {
        temp = tree;
        while (temp->left != &Nul) {
            temp = temp->left;
        }
        temp->splay(Nul);
        answer.first = &Nul;
        answer.second = temp;
        return answer;
    }
    temp->splay(Nul);
    temp->sum -= temp->right->sum;
    temp->amount -= temp->right->amount;
    answer.first = temp;
    answer.second = temp->right;
    if (temp->right != &Nul) {
        temp->right->parent = &Nul;
    }
    temp->right = &Nul;
    return answer;
}

Node* Node::getSum(long long leftBorder, long long rightBorder, long long& answer, Node& Nul) {
    std::pair<Node*, Node*> smallParts = split(leftBorder - 1, Nul, this);
    std::pair<Node*, Node*> bigParts = split(rightBorder, Nul, smallParts.second);
    answer = bigParts.first->sum;
    return merge(smallParts.first, merge(bigParts.first, bigParts.second, Nul), Nul);
}

Node* Node::insert(long long newValue, Node& Nul) {
    std::pair<Node*, Node*> parts = split(newValue, Nul, this);
    if (parts.first->value == newValue) {
        return merge(parts.first, parts.second, Nul);
    }
    Node* newElement = new Node(newValue, &Nul);
    return merge(merge(parts.first, newElement, Nul), parts.second, Nul);
}

Node* Node::erase(long long newValue, Node& Nul) {
    std::pair<Node*, Node*> rightCut = split(newValue, Nul, this);
    std::pair<Node*, Node*> leftCut = split(newValue - 1, Nul, rightCut.first);
    if (leftCut.second != &Nul) {
        delete leftCut.second;
    }
    return merge(leftCut.first, rightCut.second, Nul);
}

Node* Node::getIndex(int index, long long& answer, Node& Nul) {
    if (index == left->amount + 1) {
        answer = value;
        this->splay(Nul);
        return this;
    }
    if (index > left->amount + 1) {
        if (right != &Nul) {
            right->getIndex(index - left->amount - 1, answer, Nul);
        }
    } else {
        if (left != &Nul) {
            left->getIndex(index, answer, Nul);
        }
    }
}

void prepare(long long& status, Node*& tree, Node& Nul, long long value, long long& coordinate) {
    status += (value * value);
    coordinate += value;
    tree = tree->insert(coordinate, Nul);
}
void allLogic(int command, int index, long long& status, int& amount, Node*& tree, Node& Nul) {
    if (command == 1) {
        if (index == 1) {
            long long deletingPosition;
            long long end;
            tree = tree->getIndex(index + 1, deletingPosition, Nul);
            tree = tree->getIndex(index + 2, end, Nul);
            status -= (deletingPosition * deletingPosition);
            status -= ((end - deletingPosition) * (end - deletingPosition));
            status += (end * end);
            tree = tree->erase(deletingPosition, Nul);
        }
        if (index == amount) {
            long long deletingPosition;
            long long end;
            long long start;
            tree = tree->getIndex(index, deletingPosition, Nul);
            tree = tree->getIndex(index + 1, end, Nul);
            tree = tree->getIndex(index - 1, start, Nul);
            status -= ((deletingPosition - start) * (deletingPosition - start));
            status -= ((end - deletingPosition) * (end - deletingPosition));
            status += ((end - start) * (end - start));
            tree = tree->erase(deletingPosition, Nul);
        }
        if (index != amount && index != 1) {
            long long deletingLeft;
            long long deletingRight;
            long long start;
            long long end;
            tree = tree->getIndex(index, deletingLeft, Nul);
            tree = tree->getIndex(index + 1, deletingRight, Nul);
            tree = tree->getIndex(index - 1, start, Nul);
            tree = tree->getIndex(index + 2, end, Nul);
            long long middle = (deletingLeft + deletingRight) / 2;
            status -= ((deletingLeft - start) * (deletingLeft - start));
            status -= ((deletingRight - deletingLeft) * (deletingRight - deletingLeft));
            status -= ((end - deletingRight) * (end - deletingRight));
            status += ((middle - start) * (middle - start));
            status += ((end - middle) * (end - middle));
            tree = tree->erase(deletingLeft, Nul);
            tree = tree->erase(deletingRight, Nul);
            tree = tree->insert(middle, Nul);
        }
        --amount;
    }
    if (command == 2) {
        ++amount;
        long long middle;
        long long start;
        long long end;
        tree = tree->getIndex(index, start, Nul);
        tree = tree->getIndex(index + 1, end, Nul);
        middle = (start + end) / 2;
        status -= ((end - start) * (end - start));
        status += ((end - middle) * (end - middle));
        status += ((middle - start) * (middle - start));
        tree = tree->insert(middle, Nul);
    }
}
int main() {
    Node Nul;
    Node* tree = &Nul;
    std::ifstream cin("river.in");
    std::ofstream cout("river.out");
    int amount, commandAmount;
    cin >> amount >> commandAmount;
    long long status = 0;
    tree = tree->insert(0, Nul);
    long long coordinate = 0;
    for (int i = 0; i < amount; ++i) {
        long long value;
        cin >> value;
        prepare(status, tree, Nul, value, coordinate);
    }
    cin >> commandAmount;
    cout << status << '\n';
    for (int i = 0; i < commandAmount; ++i) {
        int command, index;
        cin >> command >> index;
        allLogic(command, index, status, amount, tree, Nul);
        cout << status << '\n';
    }
    return 0;
}
