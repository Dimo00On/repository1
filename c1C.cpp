#include <iostream>
#include <stack>
#include <string>

const int anyNegativeNumber = -100;
struct Column{
    long long height = anyNegativeNumber;
    long long firstLeftColumn = -1;
    long long myPosition = 0;
};
void pleaseSolveThisTask(std::string uselessString) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    Column lastLessColumn;
    long long columnAmount;
    long long columnHeight;
    std::stack <Column> increasingColumns;
    std::cin >> columnAmount;
    std::cin >> columnHeight;
    increasingColumns.push(lastLessColumn);
    lastLessColumn.height = columnHeight;
    lastLessColumn.firstLeftColumn = 0;
    lastLessColumn.myPosition = 1;
    increasingColumns.push(lastLessColumn);
    long long maxArea = columnHeight;
    for (long long i = 1; i < columnAmount; ++i) {
        std::cin >> columnHeight;
        lastLessColumn = increasingColumns.top();
        if (columnHeight > lastLessColumn.height) {
            lastLessColumn.height=columnHeight;
            lastLessColumn.firstLeftColumn = lastLessColumn.myPosition;
            lastLessColumn.myPosition = i + 1;
            increasingColumns.push(lastLessColumn);
        } else {
            long long area;
            while (columnHeight <= lastLessColumn.height) {
                area = (i - lastLessColumn.firstLeftColumn) * lastLessColumn.height;
                maxArea = std::max(maxArea, area);
                increasingColumns.pop();
                lastLessColumn = increasingColumns.top();
            }
            lastLessColumn.height = columnHeight;
            lastLessColumn.firstLeftColumn = lastLessColumn.myPosition;
            lastLessColumn.myPosition = i + 1;
            increasingColumns.push(lastLessColumn);
        }
    }
    columnHeight=0;
    lastLessColumn = increasingColumns.top();
    long long area;
    while (columnHeight <= lastLessColumn.height) {
        area = (columnAmount - lastLessColumn.firstLeftColumn) * lastLessColumn.height;
        maxArea = std::max(maxArea, area);
        increasingColumns.pop();
        lastLessColumn = increasingColumns.top();
    }
    std::cout << maxArea << '\n';
    return;
}
int main(){
    pleaseSolveThisTask("C task");
    return 0;
}
