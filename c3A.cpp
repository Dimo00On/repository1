#include <iostream>
#include <vector>

const int MaxSize = 1 << 17;//more 100000
const int Infinity = MaxSize;
const long long magicConst1 = 12345;
const long long magicConst2 = 23456;
struct segmentNode{
    int max;
    int min;
    int left;
    int right;
};
segmentNode& build(int index, std::vector<segmentNode>& tree) {
    if (index >= MaxSize){
        return tree[index];
    }
    segmentNode& subtree1 = build(2 * index, tree);
    segmentNode& subtree2 = build(2 * index + 1, tree);
    tree[index].max = std::max(subtree1.max, subtree2.max);
    tree[index].min = std::min(subtree1.min, subtree2.min);
    tree[index].left = subtree1.left;
    tree[index].right = subtree2.right;
    return tree[index];
}
int foundMax(int left, int right, std::vector<segmentNode>& tree, int index) {
    if (tree[index].left == tree[index].right) {
        return tree[index].max;
    }
    if (tree[index].left == left && tree[index].right == right) {
        return tree[index].max;
    }
    int middle = (tree[index].left + tree[index].right) / 2;
    int max1 = -Infinity;
    int max2 = -Infinity;
    if (middle >= left) {
        max1 = foundMax(left, std::min(middle, right), tree, 2 * index);
    }
    if (middle + 1 <= right) {
        max2 = foundMax(std::max(left, middle + 1), right, tree, 2 * index + 1);
    }
    return std::max(max1, max2);
}
int foundMin(int left, int right, std::vector<segmentNode>& tree, int index) {
    if (tree[index].left == tree[index].right) {
        return tree[index].min;
    }
    if (tree[index].left == left && tree[index].right == right) {
        return tree[index].min;
    }
    int middle = (tree[index].left + tree[index].right) / 2;
    int min1 = Infinity;
    int min2 = Infinity;
    if (middle >= left) {
        min1 = foundMin(left, std::min(middle, right), tree, 2 * index);
    }
    if (middle + 1 <= right) {
        min2 = foundMin(std::max(left, middle + 1), right, tree, 2 * index + 1);
    }
    return std::min(min1, min2);
}
void update(std::vector<segmentNode>& tree, int index) {
    tree[index].max = std::max(tree[2 * index].max, tree[2 * index + 1].max);
    tree[index].min = std::min(tree[2 * index].min, tree[2 * index + 1].min);
    if (index != 1) {
        update(tree, index/2);
    }
}
void pleaseSolveThisTask(const std::string& uselessString) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::vector<segmentNode> tree(2 * MaxSize);
    for (long long i = 1; i < MaxSize + 1; ++i) {
        tree[i + MaxSize - 1].max = tree[i + MaxSize - 1].min = static_cast<int>((i * i) % magicConst1 + (i * i * i) % magicConst2);
        tree[i + MaxSize - 1].left = tree[i + MaxSize - 1].right = static_cast<int>(i);
    }
    build(1, tree);
    int commandAmount;
    std::cin >> commandAmount;
    for (int i = 0; i < commandAmount; ++i) {
        int xi, yi; //from task description
        std::cin >> xi >> yi;
        if (xi > 0) {
            std::cout << foundMax(xi, yi, tree, 1)  -  foundMin(xi, yi, tree, 1)<< '\n';
        } else {
            tree[MaxSize - xi - 1].max = tree[MaxSize - xi - 1].min = yi;
            update(tree, (MaxSize - xi - 1) / 2);
        }
    }
}
int main() {
    pleaseSolveThisTask("A task");
}

