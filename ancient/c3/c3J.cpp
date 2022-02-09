#include <iostream>
#include <vector>
#include <algorithm>

const int Infinity = 1e9 + 42;
struct Point{
    int y;
    int index;
    Point() = default;
    explicit Point(int newY) {
        y = newY;
        index = 0;
    }
};
struct VerticalEdge{
    int x;
    Point* up;
    Point* down;
    bool type;
    VerticalEdge() = default;
    VerticalEdge(Point* newUp, Point* newDown, bool newType, int newX) {
        up = newUp;
        down = newDown;
        type = newType;
        x = newX;
    }
};
struct Segment{
    int left;
    int right;
    int yStart;
    int indexInAllPoints;
    int answer;
    int min;
    int lengthOfMinimals;
    int push;
};
void fillArrays(int x1, int y1, int x2, int y2, int i, int rectangleAmount,
                std::vector<Point>& allPoints, std::vector<VerticalEdge>& allEdges) {
    Point up(y2);
    Point down(y1);
    allPoints[i] = up;
    allPoints[i + rectangleAmount] = down;
    VerticalEdge left(&allPoints[i], &allPoints[i + rectangleAmount], true, x1);
    VerticalEdge right(&allPoints[i], &allPoints[i + rectangleAmount], false, x2);
    allEdges[i] = left;
    allEdges[i + rectangleAmount] = right;
}
int findPerfectSize(int rectangleAmount) {
    rectangleAmount *= 2;
    --rectangleAmount;
    int perfectSize = 1;
    while (rectangleAmount > 0) {
        rectangleAmount /= 2;
        perfectSize *= 2;
    }
    return perfectSize;
}
bool comparatorByXCoordinate(VerticalEdge first, VerticalEdge second) {
    return first.x < second.x;
}
bool comparatorByYCoordinate(Segment first, Segment second) {
    return first.yStart < second.yStart;
}
void startFillTree(std::vector<Segment>& tree, int rectangleAmount, int perfectSize, std::vector<Point>& allPoints) {
    for (int i = 0; i < 2 * rectangleAmount; ++i) {
        tree[i + perfectSize].yStart = allPoints[i].y;
        tree[i + perfectSize].indexInAllPoints = i;
    }
}
void findIndexesAndOther(std::vector<Segment>& tree, std::vector<Point>& allPoints, int perfectSize, int rectangleAmount) {
    for (int i = perfectSize; i < perfectSize + 2 * rectangleAmount; ++i) {
        allPoints[tree[i].indexInAllPoints].index = i;
        if (i + 1 < perfectSize + 2 * rectangleAmount) {
            tree[i].lengthOfMinimals = tree[i + 1].yStart - tree[i].yStart;
            tree[i].min = 0;
            tree[i].push = 0;
            tree[i].left = tree[i].right = i - perfectSize;
            tree[i].answer = tree[i].lengthOfMinimals;
        }
    }
    for (int i = perfectSize + 2 * rectangleAmount - 1; i < 2 * perfectSize; ++i) {
        tree[i].lengthOfMinimals = 0;
        tree[i].min = Infinity;
        tree[i].push = 0;
        tree[i].left = tree[i].right = i - perfectSize;
        tree[i].answer = tree[i].lengthOfMinimals;
    }
}
void buildTree(int index, std::vector<Segment>& tree, int perfectSize) {
    if (index >= perfectSize) {
        return;
    }
    buildTree(2 * index, tree, perfectSize);
    buildTree(2 * index + 1, tree, perfectSize);
    tree[index].left = tree[2 * index].left;
    tree[index].right = tree[2 * index + 1].right;
    tree[index].push = 0;
    tree[index].lengthOfMinimals = tree[2 * index].lengthOfMinimals + tree[2 * index + 1].lengthOfMinimals;
    tree[index].min = std::min(tree[2 * index].min, tree[2 * index + 1].min);
    tree[index].answer = tree[index].lengthOfMinimals;
}
void fixMin(int destination, int delta, std::vector<Segment>& tree) {
    if (tree[destination].min == 0 && delta > 0) {
        tree[destination].answer = 0;
    }
    if (tree[destination].min == -delta) {
        tree[destination].answer = tree[destination].lengthOfMinimals;
    }
    tree[destination].min += delta;
    tree[destination].push += delta;
}
void pushing(int index, std::vector<Segment>& tree, int perfectSize) {
    if (index >= perfectSize) {
        tree[index].push = 0;
        return;
    }
    fixMin(2 * index, tree[index].push, tree);
    fixMin(2 * index + 1, tree[index].push, tree);
    tree[index].push = 0;
}
void update(int left, int right, int index, int delta, std::vector<Segment>& tree, int perfectSize) {
    if (tree[index].push != 0) {
        pushing(index, tree, perfectSize);
    }
    if (tree[index].left == left && tree[index].right == right) {
        fixMin(index, delta, tree);
        return;
    }
    int middle = (tree[index].left + tree[index].right) / 2;
    if (middle >= left) {
        update(left, std::min(middle, right), 2 * index, delta, tree, perfectSize);
    }
    if (middle < right) {
        update(std::max(left, middle + 1), right, 2 * index + 1, delta, tree, perfectSize);
    }
    if (tree[2 * index].min == tree[2 * index + 1].min) {
        tree[index].min = tree[2 * index].min;
        tree[index].lengthOfMinimals = tree[2 * index].lengthOfMinimals + tree[2 * index + 1].lengthOfMinimals;
    } else {
        if (tree[2 * index].min < tree[2 * index + 1].min) {
            tree[index].min = tree[2 * index].min;
            tree[index].lengthOfMinimals = tree[2 * index].lengthOfMinimals;
        } else {
            tree[index].min = tree[2 * index + 1].min;
            tree[index].lengthOfMinimals = tree[2 * index + 1].lengthOfMinimals;
        }
    }
    tree[index].answer = tree[2 * index].answer + tree[2 * index + 1].answer;
}
long long allLogic(int rectangleAmount, std::vector<Point>& allPoints, std::vector<VerticalEdge>& allEdges) {
    int perfectSize = findPerfectSize(rectangleAmount);
    std::vector<Segment> tree(2 * perfectSize);
    startFillTree(tree, rectangleAmount, perfectSize, allPoints);
    std::sort(tree.begin() + perfectSize, tree.begin() + perfectSize + 2 * rectangleAmount, comparatorByYCoordinate);
    int allLength = tree[perfectSize + 2 * rectangleAmount - 1].yStart - tree[perfectSize].yStart;
    findIndexesAndOther(tree, allPoints, perfectSize, rectangleAmount);
    buildTree(1, tree, perfectSize);
    std::sort(allEdges.begin(), allEdges.end(), comparatorByXCoordinate);
    long long sum = 0;
    for (int i = 0; i < 2 * rectangleAmount - 1; ++i) {
        int delta = (allEdges[i].type ? 1 : -1);
        update(tree[allEdges[i].down->index].left, tree[allEdges[i].up->index - 1].right, 1, delta, tree, perfectSize);
        sum += (static_cast<long long>(allLength - tree[1].answer) * static_cast<long long>(allEdges[i + 1].x - allEdges[i].x));
    }
    return sum;
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int rectangleAmount;
    std::cin >> rectangleAmount;
    std::vector<Point> allPoints(2 * rectangleAmount);
    std::vector<VerticalEdge> allEdges(2 * rectangleAmount);
    for (int i = 0; i < rectangleAmount; ++i) {
        int x1, x2, y1, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        fillArrays(x1, y1, x2, y2, i, rectangleAmount, allPoints, allEdges);
    }
    long long answer  = allLogic(rectangleAmount, allPoints, allEdges);
    std::cout << answer;
}

