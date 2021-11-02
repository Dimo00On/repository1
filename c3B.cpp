#include <iostream>
#include <vector>

const int MaxSize = 1 << 17;//more 100000
struct segmentNode{
    int sum = 0;
    int left;
    int right;
};
segmentNode& build(int index, std::vector<segmentNode>& tree) {
    if (index >= MaxSize){
        return tree[index];
    }
    segmentNode& subtree1 = build(2 * index, tree);
    segmentNode& subtree2 = build(2 * index + 1, tree);
    tree[index].left = subtree1.left;
    tree[index].right = subtree2.right;
    if (index >= MaxSize / 2) {
        tree[index].sum = subtree1.sum - subtree2.sum;
        return tree[index];
    }
    tree[index].sum = subtree1.sum + subtree2.sum;
    return tree[index];
}
void foundSum(int left, int right, std::vector<segmentNode>& tree, int index, int& sum, int& sign) {
    if (tree[index].left == tree[index].right) {
        sum += sign * tree[index].sum;
        sign = -sign;
        return;
    }
    if (tree[index].left == left && tree[index].right == right) {
        sum += sign * tree[index].sum;
        return;
    }
    int middle = (tree[index].left + tree[index].right) / 2;
    if (middle >= left) {
        foundSum(left, std::min(middle, right), tree, 2 * index, sum, sign);
    }
    if (middle + 1 <= right) {
        foundSum(std::max(left, middle + 1), right, tree, 2 * index + 1, sum, sign);
    }
}
void update(std::vector<segmentNode>& tree, int index) {
    tree[index].sum = tree[2 * index].sum + tree[2 * index + 1].sum;
    if (index != 1) {
        update(tree, index/2);
    }
}
void pleaseSolveThisTask(const std::string& uselessString) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::vector<segmentNode> tree(2 * MaxSize);
    int size;
    std::cin >> size;
    for (int i = 0; i < MaxSize; ++i) {
        if (i < size) {
            int value;
            std::cin >> value;
            tree[i + MaxSize].sum = value;
        }
        tree[i + MaxSize].left = i + 1;
        tree[i + MaxSize].right = i + 1;
    }
    build(1, tree);
    int commandAmount;
    std::cin >> commandAmount;
    for (int i = 0; i < commandAmount; ++i) {
        bool command;
        std::cin >> command;
        if (command) {
            int left, right;
            std::cin >> left >> right;
            int sum = 0;
            int sign = 1;
            foundSum(left, right, tree, 1, sum, sign);
            std::cout << sum << '\n';
        } else {
            int index, value;
            std::cin >> index >> value;
            tree[index + MaxSize - 1].sum = value;
            int newIndex = (index + MaxSize - 1) / 2;
            tree[newIndex].sum = tree[newIndex * 2].sum - tree[newIndex * 2 + 1].sum;
            update(tree, newIndex / 2);
        }
    }
}
int main() {
    pleaseSolveThisTask("B task");
}
