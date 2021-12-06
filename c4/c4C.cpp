#include <iostream>
#include <string>

const long long Infinity = 1e18 + 42;

struct Node {
private:
    Node* parent;
    Node* left;
    Node* right;
    long long value;
    int amount;
public:
    friend Node* merge(Node* first, Node* second, Node& Nul);
    friend std::pair<Node*, Node*> split(long long newValue, Node& Nul, Node* tree);
    Node();
    Node(long long newValue, Node* nul);
    Node* insert(long long newValue, Node& Nul);
    Node* erase(long long newValue, Node& Nul);
    Node* get(int index, long long& answer, Node& Nul);
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
    value = Infinity;
    amount = 0;
}

Node::Node(long long newValue, Node* nul) {
    parent = nul;
    left = nul;
    right = nul;
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
        right->right->amount = right->right->right->amount + right->right->left->amount + 1;
        right->amount = right->right->amount + right->left->amount + 1;
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
        left->left->amount = left->left->left->amount + left->left->right->amount + 1;
        left->amount = left->left->amount + left->right->amount + 1;
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
    right->amount = right->right->amount + right->left->amount + 1;
    left->amount = left->right->amount + left->left->amount + 1;
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
        amount = right->amount;
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
        amount = left->amount;
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
    temp->amount -= temp->right->amount;
    answer.first = temp;
    answer.second = temp->right;
    if (temp->right != &Nul) {
        temp->right->parent = &Nul;
    }
    temp->right = &Nul;
    return answer;
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
    //понадобилось написал
    std::pair<Node*, Node*> rightCut = split(newValue, Nul, this);
    std::pair<Node*, Node*> leftCut = split(newValue - 1, Nul, rightCut.first);
    if (leftCut.second != &Nul) {
        delete leftCut.second;
    }
    return merge(leftCut.first, rightCut.second, Nul);
}

Node* Node::get(int index, long long& answer, Node& Nul) {
    if (index == right->amount + 1) {
        answer = value;
        this->splay(Nul);
        return this;
    }
    if (index > right->amount + 1) {
        if (left != &Nul) {
            left->get(index - right->amount - 1, answer, Nul);
        }
    } else {
        if (right != &Nul) {
            right->get(index, answer, Nul);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    Node Nul;
    Node* tree = &Nul;
    int commandAmount;
    std::cin >> commandAmount;
    for (int i = 0; i < commandAmount; ++i) {
        std::string command;
        std::cin >> command;
        if (command[0] == '+' || command[0] == '1') {
            long long value;
            std::cin >> value;
            tree = tree->insert(value, Nul);
        }
        if (command[0] == '0') {
            int index;
            std::cin >> index;
            long long answer;
            tree = tree->get(index, answer, Nul);
            std::cout << answer << '\n';
        }
        if (command[0] == '-') {
            long long value;
            std::cin >> value;
            tree = tree->erase(value, Nul);
        }
    }
}
