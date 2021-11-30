#include <iostream>

const int Infinity = 1e9 + 42;
//const int AlphabetStart = 48;
struct Node{
    Node* left;
    Node* right;
    Node* parent;
    int delta;
    int value;
    int high;
    Node() {
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        delta = 0;
        high = -1;
        value = Infinity;
    }
    Node (int newValue, Node* newParent) {
        parent = newParent;
        left = nullptr;
        right = nullptr;
        delta = 0;
        high = 0;
        value = newValue;
    }
    void insert(int newValue);
    void remove(int newValue);
    bool isExist(int newValue);
    void next(int newValue, int& answer);
    void prev(int newValue, int& answer);
    void fix();
    Node* leftFix();
    Node* rightFix();
};
Node* Node::leftFix() {
    Node* temp;
    int leftRightHigh = (left->right == nullptr ? -1 : left->right->high);
    int leftLeftHigh = (left->left == nullptr ? -1 : left->left->high);
    left->delta = leftRightHigh - leftRightHigh;
    left->high = std::max(leftRightHigh, leftLeftHigh) + 1;
    if (left->delta >= 0) {
        temp = left;
    } else {
        temp = left->right;
        left->right = temp->left;
        if (temp->left != nullptr) {
            temp->left->parent = left;
        }
        temp->left = left;
        left->parent = temp;
    }
    left = temp->right;
    if (temp->right != nullptr) {
        temp->right->parent = this;
    }
    temp->right = this;
    return temp;
}
Node* Node::rightFix() {
    Node* temp;
    int rightRightHigh = (right->right == nullptr ? -1 : right->right->high);
    int rightLeftHigh = (right->left == nullptr ? -1 : right->left->high);
    right->delta = rightLeftHigh - rightRightHigh;
    right->high = std::max(rightRightHigh, rightLeftHigh) + 1;
    if (right->delta <= 0) {
        temp = right;
    } else {
        temp = right->left;
        right->left = temp->right;
        if (temp->right != nullptr) {
            temp->right->parent = right;
        }
        temp->right = right;
        right->parent = temp;
    }
    right = temp->left;
    if (temp->left != nullptr) {
        temp->left->parent = this;
    }
    temp->left = this;
    return temp;
}
void Node::fix() {
    if (value == Infinity) {
        return;
    }
    int leftHigh = (left == nullptr ? -1 : left->high);
    int rightHigh = (right == nullptr ? -1 : right->high);
    delta = leftHigh - rightHigh;
    Node* temp;
    if (delta == 2 || delta == -2) {
        temp = (delta > 0 ? this->leftFix() : this->rightFix());
        temp->parent = parent;
        if (parent->left == this) {
            parent->left = temp;
        } else {
            parent->right = temp;
        }
        parent = temp;
    }
    leftHigh = (left == nullptr ? -1 : left->high);
    rightHigh = (right == nullptr ? -1 : right->high);
    delta = leftHigh - rightHigh;
    high = std::max(rightHigh, leftHigh) + 1;
    if (right != nullptr) {
        int rightRightHigh = (right->right == nullptr ? -1 : right->right->high);
        int rightLeftHigh = (right->left == nullptr ? -1 : right->left->high);
        right->delta = rightLeftHigh - rightRightHigh;
        right->high = std::max(rightRightHigh, rightLeftHigh) + 1;
    }
    if (left != nullptr) {
        int leftRightHigh = (left->right == nullptr ? -1 : left->right->high);
        int leftLeftHigh = (left->left == nullptr ? -1 : left->left->high);
        left->delta = leftRightHigh - leftRightHigh;
        left->high = std::max(leftRightHigh, leftLeftHigh) + 1;
    }
    parent->fix();
}
void Node::insert(int newValue) {
    if (value == newValue) {
        return;
    }
    if (value < newValue) {
        if (right == nullptr) {
            Node* newElement = new Node(newValue, this);
            right = newElement;
            this->fix();
        }
        else {
            right->insert(newValue);
        }
    } else {
        if (left == nullptr) {
            Node* newElement = new Node(newValue, this);
            left = newElement;
            this->fix();
        } else {
            left->insert(newValue);
        }
    }
}
void Node::remove(int newValue) {
    if (value == newValue) {
        if (right == nullptr) {
            if (left != nullptr) {
                left->parent = parent;
            }
            if (parent->left == this) {
                parent->left = left;
            } else {
                parent->right = left;
            }
            this->parent->fix();
            delete this;
            return;
        }
        Node *replacement = right;
        if (replacement->left == nullptr) {
            replacement->parent->right = replacement->right;
            if (replacement->right != nullptr) {
                replacement->right->parent = replacement->parent;
            }
            value = replacement->value;
            delete replacement;
            return;
        }
        while (replacement->left != nullptr) {
            replacement = replacement->left;
        }
        value = replacement->value;
        replacement->parent->left = replacement->right;
        if (replacement->right != nullptr) {
            replacement->right->parent = replacement->parent;
        }
        replacement->parent->fix();
        delete replacement;
        return;
    }
    if (value < newValue) {
        if (right != nullptr) {
            right->remove(newValue);
        }
    } else {
        if (left != nullptr) {
            left->remove(newValue);
        }
    }
}
bool Node::isExist(int newValue) {
    if (value == newValue) {
        return true;
    }
    if (value < newValue) {
        if (right == nullptr) {
            return false;
        }
        return right->isExist(newValue);
    }
    if (left == nullptr) {
        return false;
    }
    return left->isExist(newValue);
}
void Node::next(int newValue, int& answer) {
    if (value <= newValue) {
        if (right == nullptr) {
            return;
        }
        right->next(newValue, answer);
    }
    if (value > newValue) {
        answer = value;
        if (left == nullptr) {
            return;
        }
        left->next(newValue, answer);
    }
}
void Node::prev(int newValue, int& answer) {
    if (value >= newValue) {
        if (left == nullptr) {
            return;
        }
        return left->prev(newValue, answer);
    }
    if (value < newValue) {
        answer = value;
        if (right == nullptr) {
            return;
        }
        right->prev(newValue, answer);
    }
}
/*int decipherThisTerribleInput(std::string& command) {
    int value = 0;
    int i =0;
    while (command[i] != ' ') {
        ++i;
    }
    ++i;
    while (i < command.length()) {
        value *= 10;
        value += (command[i] - AlphabetStart);
        ++i;
    }
    return value;
}*/
int main() {
    //std::ios_base::sync_with_stdio(false);
    //std::cin.tie(NULL);
    Node tree;
    /*for (int i = 0; i < 10000; ++i) {
        tree.insert(i);
    }
    int x=0;
    for (int i = 0; i < 10000; ++i) {
        if (!(i % 1000)) {
            ++x;
        }
        tree.remove((i * 137) % 10000);
    }*/
    std::string command;
    for (int i = 0; i < 100000; ++i) {
        if (std::cin >> command) {
            int value;
            std::cin >> value;
            if (command[0] == 'i') {
                tree.insert(value);
            }
            if (command[0] == 'd') {
                tree.remove(value);
            }
            if (command[0] == 'e') {
                if (tree.isExist(value)) {
                    std::cout << "true" << '\n';
                } else {
                    std::cout << "false" << '\n';
                }
            }
            if (command[0] == 'n') {
                int answer = Infinity;
                tree.next(value, answer);
                if (answer == Infinity) {
                    std::cout << "none" << '\n';
                } else {
                    std::cout << answer << '\n';
                }
            }
            if (command[0] == 'p') {
                int answer = Infinity;
                tree.prev(value, answer);
                if (answer == Infinity) {
                    std::cout << "none" << '\n';
                } else {
                    std::cout << answer << '\n';
                }
            }
        }
    }
}
