#include <iostream>
#include <stack>

struct Column{
    long long height;
    long long firstLeftColumn;
    long long myPosition;
    Column() = default;
    Column(long long newHeight, long long newLeftColumn, long long newPosition) {
        height = newHeight;
        firstLeftColumn = newLeftColumn;
        myPosition = newPosition;
    }
};
void prepareStack(long long columnHeight, std::stack<Column>& increasingColumns) {
    {
        Column lastLessColumn(-1, -1, 0);
        increasingColumns.push(lastLessColumn);
    }
    Column lastLessColumn(columnHeight, 0, 1);
    increasingColumns.push(lastLessColumn);
}
Column findLastLessColumn(long long columnHeight, std::stack<Column>& increasingColumns, long long& maxArea, long long step) {
    Column lastLessColumn = increasingColumns.top();
    long long area;
    while (columnHeight <= lastLessColumn.height) {
        area = (step - lastLessColumn.firstLeftColumn) * lastLessColumn.height;
        maxArea = std::max(maxArea, area);
        increasingColumns.pop();
        lastLessColumn = increasingColumns.top();
    }
    return lastLessColumn;
}
void compareHeights(long long columnHeight, std::stack<Column>& increasingColumns, long long& maxArea, long long step) {
    Column lastLessColumn = findLastLessColumn(columnHeight, increasingColumns, maxArea, step);
    Column newColumn(columnHeight, lastLessColumn.myPosition, step + 1);
    increasingColumns.push(newColumn);
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    long long columnAmount;
    long long columnHeight;
    std::stack<Column> increasingColumns;
    std::cin >> columnAmount;
    std::cin >> columnHeight;
    prepareStack(columnHeight, increasingColumns);
    long long maxArea = columnHeight;
    for (long long i = 1; i < columnAmount; ++i) {
        std::cin >> columnHeight;
        compareHeights(columnHeight, increasingColumns, maxArea, i);
    }
    findLastLessColumn(0, increasingColumns, maxArea, columnAmount);
    std::cout << maxArea << '\n';
}
