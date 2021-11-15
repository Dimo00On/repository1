#include <iostream>
#include <vector>
#include <algorithm>

struct Edge{
    int value;
    bool type;
    int number;
    Edge() = default;
    Edge(int newValue, bool newType, int newNumber) {
        value = newValue;
        type = newType;
        number = newNumber;
    }
};
struct Segment{
    int left;
    int right;
    int index;
    Segment() = default;
    Segment(int newLeft, int newRight) {
        left = newLeft;
        right = newRight;
    }
};
struct SegmentTree{
    int status = 0;
    int positionLeft = 0;
    int positionRight = 0;
};
namespace N{
    std::vector<Segment> info;
}
void foundSum(int index, int leftSide, int rightSide, long long& sum, std::vector<SegmentTree>& tree){
    if (tree[index].positionLeft == leftSide && tree[index].positionRight == rightSide) {
        sum += tree[index].status;
        return;
    }
    int middle = (tree[index].positionLeft + tree[index].positionRight) / 2;
    if (leftSide <= middle) {
        foundSum(index * 2, leftSide, std::min(rightSide, middle), sum, tree);
    }
    if (middle < rightSide) {
        foundSum(index * 2 + 1, std::max(leftSide, middle + 1), rightSide, sum, tree);
    }
}
void update(int index, std::vector<SegmentTree>& tree){
    if (index == 0) {
        return;
    }
    tree[index].status = tree[2 * index].status + tree[2 * index + 1].status;
    update(index / 2, tree);
}
void build(int index, std::vector<SegmentTree>& tree, int perfectSize){
    if (index < perfectSize) {
        build(2 * index, tree, perfectSize);
        build(2 * index + 1, tree, perfectSize);
        tree[index].positionLeft = tree[2 * index].positionLeft;
        tree[index].positionRight = tree[2 * index + 1].positionRight;
    }
}
bool comparator(Edge edge1, Edge edge2){
    if (edge1.value == edge2.value) {
        if (edge1.type == edge2.type) {
            if (!edge1.type) {
                return N::info[edge1.number].left > N::info[edge2.number].left;
            }
            return N::info[edge1.number].right > N::info[edge2.number].right;
        }
        return edge1.type > edge2.type;
    }
    return edge1.value < edge2.value;
}
int findPerfectSize(int segmentAmount) {
    int copy = segmentAmount - 1;
    int perfectSize = 1;
    while (copy > 0) {
        copy /= 2;
        perfectSize *= 2;
    }
    return perfectSize;
}
void fillArrays(int left, int right, int segmentAmount, int step, std::vector<Edge>&  border) {
    {
        Edge newEdge(left, true, step);
        border[step] = newEdge;
    }
    Edge newEdge(right, false, step);
    border[step + segmentAmount] = newEdge;
    Segment newSegment(left, right);
    N::info.push_back(newSegment);
}
void fillTree(std::vector<SegmentTree>& tree, std::vector<Edge>& border, int segmentAmount, int perfectSize) {
    int index = 0;
    for (int i = 0; i < 2 * segmentAmount; ++i) {
        if (border[i].type) {
            tree[index + perfectSize].positionLeft = tree[index + perfectSize].positionRight = index;
            N::info[border[i].number].index = index + perfectSize;
            ++index;
        }
    }
    for (int i = index; i < perfectSize; ++i) {
        tree[i + perfectSize].positionLeft = tree[i + perfectSize].positionRight = i;
    }
}
void allLogic(int segmentAmount, long long& sum, std::vector<Edge>& border, std::vector<SegmentTree>& tree){
    long long temp = 0;
    long long multiplier = 1;
    for (int i = 0; i < 2 * segmentAmount; ++i) {
        if (border[i].type) {
            tree[N::info[border[i].number].index].status = 1;
            update(N::info[border[i].number].index / 2, tree);
        } else {
            tree[N::info[border[i].number].index].status = 0;
            update(N::info[border[i].number].index / 2, tree);
            if (i != 2 * segmentAmount - 1 && border[i].value == border[i + 1].value && !border[i + 1].type
                && N::info[border[i].number].left == N::info[border[i + 1].number].left) {
                ++multiplier;
                continue;
            }
            if (multiplier > 1) {
                foundSum(1, 0, tree[N::info[border[i].number].index].positionRight, temp, tree);
                sum += multiplier * temp;
                multiplier = 1;
                temp = 0;
                continue;
            }
            foundSum(1, 0, tree[N::info[border[i].number].index].positionRight, sum, tree);
        }
    }
}
int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int segmentAmount;
    std::cin >> segmentAmount;
    int perfectSize = findPerfectSize(segmentAmount);
    std::vector<Edge> border(2 * segmentAmount);
    std::vector<SegmentTree> tree(2 * perfectSize);
    for (int i = 0; i < segmentAmount; ++i) {
        int left, right;
        std::cin >> left >> right;
        fillArrays(left, right, segmentAmount, i, border);
    }
    std::sort(border.begin(), border.end(), comparator);
    fillTree(tree, border, segmentAmount, perfectSize);
    build(1, tree, perfectSize);
    long long sum = 0;
    allLogic(segmentAmount, sum, border, tree);
    std::cout << sum;
}
