#include <iostream>
#include <string>

const long long Mod = 1e9;
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
    Node* get(long long leftBorder, long long rightBorder, long long& answer, Node& Nul);
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

Node* Node::get(long long leftBorder, long long rightBorder, long long& answer, Node& Nul) {
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
    //написать как понадобится
    return this;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    Node Nul;
    Node* splayTree = &Nul;
    int commandAmount;
    std::cin >> commandAmount;
    std::string previousCommand = " ";
    long long answer = 0;
    for (int i = 0; i < commandAmount; ++i) {
        std::string command;
        std::cin >> command;
        if (command[0] == '+') {
            long long value;
            std::cin >> value;
            if (previousCommand[0] == '?') {
                value += (answer % Mod);
                value %= Mod;
            }
            splayTree = splayTree->insert(value, Nul);
        }
        if (command[0] == '-') {
            long long value;
            std::cin >> value;
            splayTree = splayTree->erase(value, Nul);
        }
        if (command[0] == '?') {
            long long leftBorder, rightBorder;
            std::cin >> leftBorder >> rightBorder;
            splayTree = splayTree->get(leftBorder, rightBorder, answer, Nul);
            std::cout << answer << '\n';
        }
        previousCommand = command;
    }
}
