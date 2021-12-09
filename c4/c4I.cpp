#include <iostream>
#include <string>
#include <vector>

static const long long Infinity = 1e9 + 42;
static const int MagicConst = 4159296; // 4159296 = log(N) * (M + perfectSize);
struct Node{
    Node* left;
    Node* right;
    Node* parent;
    int delta;
    int value;
    int high;
    int sum;
    int copies;
    Node() = default;
    explicit Node(char c); //отличать конструктор нуля от дефолтного
    explicit Node(Node* Nul);
    void insert(int newValue);
    void remove(int newValue);
    void fix();
    Node* leftFix();
    Node* rightFix();
    void getAmount(int leftBorder, int rightBorder, int& answer);
};

struct Segment{
    std::vector<std::pair<int, int>> push;
    int left;
    int right;
    Node setTree;
};

std::vector<Segment> tree;
std::vector<int> numbers;
Node Nuul('0');
Node* Nul = &Nuul;
Node start(Nul);
Node* data;
int cnt = 0;
//очень нужны глобальные переменные, чтобы не передавать каждый раз в функцию, ради времени

Node::Node(Node* Nul) {
    parent = Nul;
    left = Nul;
    right = Nul;
    delta = 0;
    high = -1;
    value = Infinity;
    sum = 0;
    copies = 0;
}

Node::Node(char c) {
    delta = 0;
    high = -1;
    value = -Infinity;
    sum = 0;
    copies = 0;
}

Node* Node::leftFix() {
    Node* temp;
    left->delta = left->left->high - left->right->high;
    left->high = std::max(left->left->high, left->right->high) + 1;
    if (left->delta >= 0) {
        temp = left;
    } else {
        temp = left->right;
        left->right = temp->left;
        if (temp->left != Nul) {
            temp->left->parent = left;
        }
        temp->left = left;
        left->parent = temp;
    }
    left = temp->right;
    if (temp->right != Nul) {
        temp->right->parent = this;
    }
    temp->right = this;
    return temp;
}

Node* Node::rightFix() {
    Node* temp;
    right->delta = right->left->high - right->right->high;
    right->high = std::max(right->left->high, right->right->high) + 1;
    if (right->delta <= 0) {
        temp = right;
    } else {
        temp = right->left;
        right->left = temp->right;
        if (temp->right != Nul) {
            temp->right->parent = right;
        }
        temp->right = right;
        right->parent = temp;
    }
    right = temp->left;
    if (temp->left != Nul) {
        temp->left->parent = this;
    }
    temp->left = this;
    return temp;
}

void Node::fix() {
    if (value == Infinity || value == -Infinity) {
        return;
    }
    delta = left->high;
    delta -= right->high;
    Node* temp;
    if (delta == 2 || delta == -2) {
        temp = (delta > 0 ? leftFix() : rightFix());
        temp->parent = parent;
        if (parent->left == this) {
            parent->left = temp;
        } else {
            parent->right = temp;
        }
        parent = temp;
    }
    sum = copies;
    sum += right->sum;
    sum += left->sum;
    temp = parent->left;
    if (temp != Nul) {
        temp->sum = temp->copies;
        temp->sum += temp->left->sum;
        temp->sum += temp->right->sum;
    }
    temp = parent->right;
    if (temp != Nul) {
        temp->sum = temp->copies;
        temp->sum += temp->left->sum;
        temp->sum += temp->right->sum;
    }
    delta = left->high;
    delta -= right->high;
    high = std::max(left->high, right->high) + 1;
    if (right != Nul) {
        right->delta = right->left->high;
        right->delta -= right->right->high;
        right->high = std::max(right->left->high, right->right->high) + 1;
    }
    if (left != Nul) {
        left->delta = left->left->high;
        left->delta -= left->right->high;
        left->high = std::max(left->left->high, left->right->high) + 1;
    }
    parent->fix();
}

void Node::insert(int newValue) {
    if (this == Nul) {
        return;
    }
    if (value == newValue) {
        ++copies;
        fix();
        return;
    }
    if (value < newValue) {
        if (right == Nul) {
            data[cnt].value = newValue;
            data[cnt].sum = 1;
            data[cnt].copies = 1;
            data[cnt].high = 0;
            data[cnt].parent = this;
            data[cnt].left = Nul;
            data[cnt].right = Nul;
            data[cnt].delta = 0;
            right = &data[cnt];
            ++cnt;
            fix();
            return;
        }
        right->insert(newValue);
        return;
    }
    if (left == Nul) {
        data[cnt].value = newValue;
        data[cnt].sum = 1;
        data[cnt].copies = 1;
        data[cnt].high = 0;
        data[cnt].parent = this;
        data[cnt].left = Nul;
        data[cnt].right = Nul;
        data[cnt].delta = 0;
        left = &data[cnt];
        ++cnt;
        fix();
        return;
    }
    left->insert(newValue);
}

void Node::remove(int newValue) {
    if (this == Nul) {
        return;
    }
    if (value == newValue) {
        if (copies == 1) {
            if (right == Nul) {
                if (left != Nul) {
                    left->parent = parent;
                }
                if (parent->left == this) {
                    parent->left = left;
                } else {
                    parent->right = left;
                }
                parent->fix();
                return;
            }
            Node *replacement = right;
            if (replacement->left == Nul) {
                right = replacement->right;
                if (replacement->right != Nul) {
                    replacement->right->parent = this;
                }
                value = replacement->value;
                copies = replacement->copies;
                this->fix();
                return;
            }
            while (replacement->left != Nul) {
                replacement = replacement->left;
            }
            value = replacement->value;
            copies = replacement->copies;
            replacement->parent->left = replacement->right;
            if (replacement->right != Nul) {
                replacement->right->parent = replacement->parent;
            }
            replacement->parent->fix();
            return;
        }
        --copies;
        fix();
        return;
    }
    if (value < newValue) {
        right->remove(newValue);
        return;
    }
    left->remove(newValue);
}

void Node::getAmount(int leftBorder, int rightBorder, int& answer) {
    if (this == Nul) {
        return;
    }
    if (value == Infinity) {
        left->getAmount(leftBorder, rightBorder, answer);
        return;
    }
    if (value <= rightBorder && value >= leftBorder) {
        answer += sum;
        Node* temp = this;
        while (temp->right != Nul && temp->right->value <= rightBorder) {
            temp = temp->right;
        }
        if (temp->right != Nul) {
            temp = temp->right;
            answer -= temp->sum;
            temp->left->getAmount(leftBorder, rightBorder, answer);
        }
        temp = this;
        while (temp->left != Nul && temp->left->value >= leftBorder) {
            temp = temp->left;
        }
        if (temp->left != Nul) {
            temp = temp->left;
            answer -= temp->sum;
            temp->right->getAmount(leftBorder, rightBorder, answer);
        }
        return;
    }
    if (value < leftBorder) {
        right->getAmount(leftBorder, rightBorder, answer);
        return;
    }
    left->getAmount(leftBorder, rightBorder, answer);
}

int findPerfectSize(int amount) {
    --amount;
    int perfectSize = 1;
    while (amount > 0) {
        amount /= 2;
        perfectSize *= 2;
    }
    return perfectSize;
}

void copy(Node*& first, Node* second, Node* parent) {
    if (second == Nul) {
        first = Nul;
        return;
    }
    first->copies = second->copies;
    first->sum = second->sum;
    first->high = second->high;
    first->value = second->value;
    first->delta = second->delta;
    Node* left;
    Node* right;
    if (second->left != Nul) {
        left = &data[cnt];
        ++cnt;
        copy(left, second->left, first);
    } else {
        left = Nul;
    }
    if (second->right != Nul) {
        right = &data[cnt];
        ++cnt;
        copy(right, second->right, first);
    } else {
        right = Nul;
    }
    first->left = left;
    first->right = right;
    first->parent = parent;
}

void build(int index, int perfectSize, int amount) {
    if (index >= perfectSize) {
        return;
    }
    build(2 * index, perfectSize, amount);
    build(2 * index + 1, perfectSize, amount);
    tree[index].left = tree[2 * index].left;
    tree[index].right = tree[2 * index + 1].right;
    tree[index].setTree = start;
    Node* temp = &data[cnt];
    ++cnt;
    copy(temp, tree[2 * index].setTree.left, &tree[index].setTree);
    tree[index].setTree.left = temp;
    for (int i = tree[2 * index + 1].left; i <= std::min(tree[index].right, amount); ++i) {
        tree[index].setTree.insert(numbers[i]);
    }
}

void doPush(int index) {
    for (size_t i = 0; i < tree[index].push.size(); ++i) {
        tree[index].setTree.remove(tree[index].push[i].first);
        tree[index].setTree.insert(tree[index].push[i].second);
    }
    tree[index].push.clear();
}

void change(int nowIndex, int newValue, int lastValue) {
    if (nowIndex == 0) {
        return;
    }
    std::pair<int, int> temp;
    temp.first = lastValue;
    temp.second = newValue;
    tree[nowIndex].push.push_back(temp);
    change(nowIndex / 2, newValue, lastValue);
}

void getAmount(int nowIndex, int left, int right, int leftBorder, int rightBorder, int& answer) {
    if (tree[nowIndex].push.size() > 0) {
        doPush(nowIndex);
    }
    if (tree[nowIndex].left == left && tree[nowIndex].right == right) {
        int addSum = 0;
        tree[nowIndex].setTree.getAmount(leftBorder, rightBorder, addSum);
        answer += addSum;
        return;
    }
    int middle = (tree[nowIndex].left+ tree[nowIndex].right) / 2;
    if (left <= middle) {
        getAmount(2 * nowIndex, left, std::min(middle, right), leftBorder, rightBorder, answer);
    }
    if (right > middle) {
        getAmount(2 * nowIndex + 1, std::max(left, middle + 1), right, leftBorder, rightBorder, answer);
    }
}

void prepare (int perfectSize, int amount) {
    for (int i = 0; i < perfectSize; ++i) {
        tree[i + perfectSize].left = tree[i + perfectSize].right = i;
        tree[i + perfectSize].setTree = start;
    }
    for (int i = 0; i < amount; ++i) {
        tree[i + perfectSize].setTree.insert(numbers[i]);
    }
    build(1, perfectSize, amount);
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    int amount, commandAmount;
    std::cin >> amount >> commandAmount;
    data = new Node[MagicConst];
    int perfectSize = findPerfectSize(amount);
    tree.resize(2 * perfectSize);
    for (int i = 0; i < amount; ++i) {
        int value;
        std::cin >> value;
        numbers.push_back(value);
    }
    prepare(perfectSize, amount);
    for (int i = 0; i < commandAmount; ++i) {
        std::string command;
        std::cin >> command;
        if (command[0] == 'S') {
            int index, value;
            std::cin >> index >> value;
            --index;
            change(index + perfectSize, value, numbers[index]);
            numbers[index] = value;
        }
        if (command[0] == 'G') {
            int indexLeft, indexRight, leftBorder, rightBorder;
            std::cin >> indexLeft >> indexRight >> leftBorder >> rightBorder;
            --indexLeft;
            --indexRight;
            int answer = 0;
            if (indexRight == amount - 1) {
                indexRight = perfectSize - 1;
            }
            getAmount(1, indexLeft, indexRight, leftBorder, rightBorder, answer);
            std::cout<< answer << '\n';
        }
    }
    delete[] data;
}