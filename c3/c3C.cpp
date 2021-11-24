#include <iostream>
#include <vector>

const int Infinity = 200001;
struct Segment{
    int min = Infinity;
    int max = -Infinity;
    int left;
    int right;
};
void foundIndex(int left, int right, int index, std::vector<Segment>& tree, int value, int& answer, int perfectSize) {
    if (tree[index].left == tree[index].right) {
        if (answer == -2 && tree[index].min >= value) {
            answer = index - perfectSize;
        }
        return;
    }
    int middle = (tree[index].left + tree[index].right) / 2;
    if (middle >= left) {
        if (tree[2 * index].max >= value) {
            foundIndex(left, std::min(middle, right), index * 2, tree, value, answer, perfectSize);
        }
    }
    if (middle < right) {
        if (answer == -2 && tree[2 * index + 1].max >= value) {
            foundIndex(std::max(left, middle + 1), right, index * 2 + 1, tree, value, answer, perfectSize);
        }
    }
}
void update(int index, std::vector<Segment>& tree) {
    if (!index) {
        return;
    }
    tree[index].min = std::min(tree[2 * index].min, tree[2 * index + 1].min);
    tree[index].max = std::max(tree[2 * index].max, tree[2 * index + 1].max);
    update(index / 2, tree);
}
void build(int index, std::vector<Segment>& tree, int perfectSize) {
    if (index >= perfectSize) {
        return;
    }
    build(index * 2, tree, perfectSize);
    build(index * 2 + 1, tree, perfectSize);
    tree[index].min = std::min(tree[2 * index].min, tree[2 * index + 1].min);
    tree[index].max = std::max(tree[2 * index].max, tree[2 * index + 1].max);
    tree[index].left = tree[2 * index].left;
    tree[index].right = tree[2 * index + 1].right;
}
int findPerfectSize(int numberAmount) {
    int copy = numberAmount - 1;
    int perfectSize = 1;
    while (copy > 0) {
        copy /= 2;
        perfectSize *= 2;
    }
    return perfectSize;
}
void fillTree(std::vector<int>& array, std::vector<Segment>& tree, int numberAmount, int perfectSize) {
    for (int i = 0; i < numberAmount; ++i) {
        tree[i + perfectSize].min = tree[i + perfectSize].max = array[i];
        tree[i + perfectSize].left = tree[i + perfectSize].right = i;
    }
    for (int i = numberAmount; i < perfectSize; ++i) {
        tree[i + perfectSize].left = tree[i + perfectSize].right = i;
    }
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int numberAmount, commandAmount;
    std::cin >> numberAmount >> commandAmount;
    int perfectSize = findPerfectSize(numberAmount);
    std::vector<Segment> tree(2 * perfectSize);
    std::vector<int> array(numberAmount);
    for (int i = 0; i < numberAmount; ++i) {
        std::cin >> array[i];
    }
    fillTree(array, tree, numberAmount, perfectSize);
    build(1, tree, perfectSize);
    for (int i = 0; i < commandAmount; ++i) {
        bool command;
        int index, value;
        std::cin >> command >> index >> value;
        --index;
        if (command) {
            int answer = -2;
            foundIndex(index, numberAmount - 1, 1, tree, value, answer, perfectSize);
            std::cout << answer + 1 << '\n';
        } else {
            tree[index + perfectSize].min = tree[index + perfectSize].max = value;
            array[index] = value;
            update((index + perfectSize) / 2, tree);
        }
    }
}
