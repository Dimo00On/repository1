#include <iostream>
#include <vector>
#include <algorithm>

struct edge{
    int value;
    int type;
    int number;
};
struct segment{
    int left;
    int right;
    int index;
};
struct segmentTree{
    int status = 0;
    int positionLeft = 0;
    int positionRight = 0;
};
namespace N{
    std::vector<segment> info;
}
void foundSum(int index, int leftSide, int rightSide, long long& sum, std::vector<segmentTree>& tree){
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
void update(int index, std::vector<segmentTree>& tree){
    if (index == 0) {
        return;
    }
    tree[index].status = tree[2 * index].status + tree[2 * index + 1].status;
    update(index / 2, tree);
}
void build(int index, std::vector<segmentTree>& tree, int perfectSize){
    if (index < perfectSize) {
        build(2 * index, tree, perfectSize);
        build(2 * index + 1, tree, perfectSize);
        tree[index].positionLeft = tree[2 * index].positionLeft;
        tree[index].positionRight = tree[2 * index + 1].positionRight;
    }
}
bool isEdge1LessEdge2(edge edge1, edge edge2){
    if (edge1.value == edge2.value) {
        if (edge1.type == edge2.type) {
            if (edge1.type == 0) {
                return N::info[edge1.number].left > N::info[edge2.number].left;
            }
            return N::info[edge1.number].right > N::info[edge2.number].right;
        }
        return edge1.type > edge2.type;
    }
    return edge1.value < edge2.value;
}
void pleaseSolveThisTask(const std::string& uselessString){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int segmentAmount;
    std::cin >> segmentAmount;
    int copy = segmentAmount - 1;
    int perfectSize = 1;
    while (copy > 0) {
        copy /= 2;
        perfectSize *= 2;
    }
    std::vector<edge>  border(2 * segmentAmount);
    std::vector<segmentTree> tree(2 * perfectSize);
    segment newSegment;
    for (int i = 0; i < segmentAmount; ++i) {
        int left, right;
        std::cin >> left >> right;
        border[i].value = left;
        border[i].type = 1;
        border[i].number = i;
        border[i + segmentAmount].value = right;
        border[i + segmentAmount].type = 0;
        border[i + segmentAmount].number = i;
        newSegment.left = left;
        newSegment.right = right;
        N::info.push_back(newSegment);
    }
    std::sort(border.begin(), border.end(), isEdge1LessEdge2);
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
    index = 1;
    build(index, tree, perfectSize);
    long long sum = 0;
    long long temp = 0;
    long long multiplier = 1;
    for (int i = 0; i < 2 * segmentAmount; ++i) { // важен порядок закрывающий ri типо 1 3 и 2 3
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
    std::cout << sum;
}
int main(){
    pleaseSolveThisTask("D task");
}
