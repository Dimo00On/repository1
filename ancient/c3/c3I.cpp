#include <iostream>
#include <vector>

const int Infinity = 10042;
struct Segment{
    int left;
    int right;
    int sum;
    int push;
    Segment(){}
    Segment(const int& newLeft, const int& newRight, const int& newSum, const int& newPush) {
        left = newLeft;
        right = newRight;
        sum = newSum;
        push = newPush;
    }
};
void pushing(int index, std::vector<Segment>& tree, int perfectSize) {
    if (tree[index].push == -1) {
        return;
    }
    if (index >= perfectSize) {
        tree[index].push = -1;
        return;
    }
    tree[2 * index].sum = tree[2 * index + 1].sum = tree[index].push;
    tree[2 * index].push = tree[2 * index + 1].push = tree[index].push;
    tree[index].push = -1;
}
void fixSum(int index, std::vector<Segment>& tree) {
    if (index == 0) {
        return;
    }
    tree[index].sum = (tree[2 * index].sum < tree[2 * index + 1].sum ? tree[2 * index].sum : tree[2 * index + 1].sum);
    fixSum(index / 2, tree);
}
void update(int left, int right, int index, int sum, std::vector<Segment>& tree, int perfectSize) {
    pushing(index, tree, perfectSize);
    if (tree[index].left == left && tree[index].right == right) {
        tree[index].sum = sum;
        tree[index].push = sum;
        fixSum(index / 2, tree);
        return;
    }
    int middle = (tree[index].left + tree[index].right) / 2;
    if (middle >= left) {
        update(left, std::min(middle, right), 2 * index, sum, tree, perfectSize);
    }
    if (middle < right) {
        update(std::max(left, middle + 1), right, 2 * index + 1, sum, tree, perfectSize);
    }
}
int photo(int left, int right, int index, std::vector<Segment>& tree, int perfectSize) {
    pushing(index, tree, perfectSize);
    if (tree[index].left == left && tree[index].right == right) {
        return tree[index].sum;
    }
    int middle = (tree[index].left + tree[index].right) / 2;
    int sum1 = Infinity;
    int sum2 = Infinity;
    if (middle >= left) {
        sum1 = photo(left, std::min(middle, right), 2 * index, tree, perfectSize);
    }
    if (middle < right) {
        sum2 = photo(std::max(left, middle + 1), right, 2 * index + 1, tree, perfectSize);
    }
    return std::min(sum1, sum2);
}
void buildTree(int index, std::vector<Segment>& tree, int perfectSize) {
    if (index >= perfectSize) {
        return;
    }
    buildTree(2 * index, tree, perfectSize);
    buildTree(2 * index + 1, tree, perfectSize);
    tree[index].left = tree[2 * index].left;
    tree[index].right = tree[2 * index + 1].right;
    tree[index].push = -1;
    tree[index].sum = (tree[2 * index].sum < tree[2 * index + 1].sum ? tree[2 * index].sum : tree[2 * index + 1].sum);
}
void addInTree(int newRed, int newGreen, int newBlue, int i, std::vector<Segment>& tree, int perfectSize) {
    int sum = newRed + newGreen + newBlue;
    Segment newPoint(i, i,  sum, -1);
    tree[i + perfectSize] = newPoint;
}
void fillTree(int pointAmount, int perfectSize, std::vector<Segment>& tree) {
    for (int i = pointAmount; i < perfectSize; ++i) {
        Segment newPoint(i, i,  Infinity, -1);
        tree[i + perfectSize] = newPoint;
    }
    buildTree(1, tree, perfectSize);
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
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int pointAmount;
    std::cin >> pointAmount;
    int perfectSize = findPerfectSize(pointAmount);
    std::vector<Segment> tree(2 * perfectSize);
    for (int i = 0; i < pointAmount; ++i) {
        int newRed, newGreen, newBlue;
        std::cin >> newRed >> newGreen >> newBlue;
        addInTree(newRed, newGreen, newBlue, i, tree, perfectSize);
    }
    fillTree(pointAmount, perfectSize, tree);
    int commandAmount;
    std::cin >> commandAmount;
    std::vector<int> answer(commandAmount);
    for (int i = 0; i < commandAmount; ++i) {
        int left, right;
        int red, green, blue;
        std::cin >> left >> right;
        std::cin >> red >> green >> blue;
        update(left, right, 1, red + green + blue, tree, perfectSize);
        std::cin >> left >> right;
        answer[i] = photo(left, right, 1, tree, perfectSize);
    }
    for (int i = 0; i < commandAmount; ++i) {
        std::cout << answer[i] << " ";
    }
}

