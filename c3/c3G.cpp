#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

struct Point{
    int x;
    int y;
    int value;
    Point() = default;
    Point(int newX, int newY, int newValue) {
        x = newX;
        y = newY;
        value = newValue;
    }
};
struct InsideFenwick{
    std::vector<long long> fenwick;
    std::vector<Point> points;
    int size;
};
int left(int index) {
    return (index & (index + 1));
}
int right(int index){
    return (index | (index + 1));
}
bool comparatorByXCoordinate(Point first, Point second) {
    if (first.x == second.x) {
        return first.y < second.y;
    }
    return first.x < second.x;
}
bool comparatorByYCoordinate(Point first, Point second) {
    if (first.y == second.y) {
        return first.x < second.x;
    }
    return first.y < second.y;
}
void prepare(int pointAmount, std::vector<Point>& points, std::vector<InsideFenwick>& fenwick) {
    for (int i = 0; i < pointAmount; ++i) {
        fenwick[i].size = i + 1 - left(i);
        fenwick[i].fenwick.resize(fenwick[i].size);
        for (int j = left(i); j <= i; ++j) {
            fenwick[i].points.push_back(points[j]);
        }
        std::sort(fenwick[i].points.begin(), fenwick[i].points.end(), comparatorByYCoordinate);
        std::vector<long long> prefix(fenwick[i].size + 1);
        prefix[0] = 0;
        for (int j = 1; j <= fenwick[i].size; ++j) {
            prefix[j] = prefix[j - 1] + fenwick[i].points[j - 1].value;
        }
        for (int j = 0; j < fenwick[i].size; ++j) {
            fenwick[i].fenwick[j] = prefix[j + 1] - prefix[left(j)];
        }
    }
}
int findIndex(int border, std::vector<Point>& points, bool type, int leftBorder, int RightBorder) {//type=false ->x
    int left = leftBorder;                                                                          //type=true ->y
    int right = RightBorder; //не достигается
    while (right - left > 1) {
        int middle = (left + right) / 2;
        if (border < (type ? points[middle].y : points[middle].x)) {
            right = middle;
        } else {
            left = middle;
        }
    }
    if (border < (type ? points[left].y : points[left].x)) {
        return -1;
    }
    return left;
}
void addSum(int indexBorderY, long long& sum, std::vector<Point>& points, std::vector<long long>& fenwick) {
    for (int i = indexBorderY; i >= 0; i = left(i) - 1) {
        sum += fenwick[i];
    }
}
void getSum(int indexBorderX, int borderY, long long& sum, std::vector<InsideFenwick>& fenwick) {
    for (int i = indexBorderX; i >= 0; i = left(i) - 1) {
        int end = static_cast<int>(fenwick[i].points.size());
        int indexBorderY = findIndex(borderY, fenwick[i].points, true, 0, end);
        addSum(indexBorderY, sum, fenwick[i].points, fenwick[i].fenwick);
    }

}
void smallUpdate(int number, int value, std::vector<Point>& points, std::vector<long long>& fenwick, int size,
                 std::vector<Point>& notSortedPoints) {
    int end = static_cast<int>(points.size());
    int rightBorder = 1 + findIndex(notSortedPoints[number].y, points, true, 0, end);
    int leftBorder = 1 + findIndex(notSortedPoints[number].y - 1, points, true, 0, end);
    int start = findIndex(notSortedPoints[number].x, points, false, leftBorder, rightBorder);
    for (int i = start; i < size; i = right(i)) {
        fenwick[i] += (value - notSortedPoints[number].value);
    }
}
void bigUpdate(int number, int value, int pointAmount, std::vector<InsideFenwick>& fenwick,
               std::vector<Point>& notSortedPoints, std::vector<Point>& sortedPoints) {
    --number;
    int end = static_cast<int>(sortedPoints.size());
    int rightBorder = 1 + findIndex(notSortedPoints[number].x, sortedPoints, false, 0, end);
    int leftBorder = 1 + findIndex(notSortedPoints[number].x - 1, sortedPoints, false, 0, end);
    int index = findIndex(notSortedPoints[number].y, sortedPoints, true, leftBorder, rightBorder);
    for (int i = index; i < pointAmount; i = right(i)) {
        smallUpdate(number, value, fenwick[i].points, fenwick[i].fenwick, fenwick[i].size, notSortedPoints);
    }
    notSortedPoints[number].value = value;
}
int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int pointAmount;
    std::cin >> pointAmount;
    std::vector<Point> points;
    std::vector<Point> notSortedPoints;
    for (int i = 0; i < pointAmount; ++i) {
        int x, y, value;
        std::cin >> x >> y >> value;
        Point newPoint(x, y, value);
        points.push_back(newPoint);
        notSortedPoints.push_back(newPoint);
    }
    std::sort(points.begin(), points.end(), comparatorByXCoordinate);
    std::vector<InsideFenwick> fenwick(pointAmount);
    prepare(pointAmount, points, fenwick);
    int commandAmount;
    std::cin >> commandAmount;
    for (int i = 0; i < commandAmount; ++i) {
        std::string command;
        std::cin >> command;
        if (command == "get") {
            int borderX, borderY;
            std::cin >> borderX >> borderY;
            int indexBorderX = findIndex(borderX, points, false, 0, static_cast<int>(points.size()));
            long long sum = 0;
            getSum(indexBorderX, borderY, sum, fenwick);
            std::cout << sum << '\n';
        } else {
            int number, value;
            std::cin >> number >> value;
            bigUpdate(number, value, pointAmount, fenwick, notSortedPoints, points);
        }
    }
}
