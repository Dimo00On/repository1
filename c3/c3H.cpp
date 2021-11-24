#include <iostream>
#include <vector>

const int Infinity = static_cast<int>((static_cast<long long>(1) << 31) - 1) ;
struct mins{
    int min1 = Infinity;//короче чем first на целый символ
    int min2 = Infinity;//короче чем second на целых 2 символа
    mins() = default;
    mins(int first, int second) {
        min1 = first;
        min2 = second;
    }
};
void degreesing(std::vector<int>& degrees) {
    degrees[1] = 0;
    for (int i = 2; i < degrees.size(); ++i) {
        degrees[i] = degrees[i-1];
        if (!(i & (i - 1))) {
            ++degrees[i];
        }
    }
}
mins newMining(mins& first, mins& second, std::vector<int>& values) {
    mins newMin;
    if (values[first.min1] < values[second.min1]) {
        newMin.min1 = first.min1;
        if (first.min1 == second.min1) {
            if (values[first.min2] < values[second.min2]) {
                newMin.min2 = first.min2;
            } else {
                newMin.min2 = second.min2;
            }
        } else {
            if (values[first.min2] < values[second.min1]) {
                newMin.min2 = first.min2;
            } else {
                newMin.min2 = second.min1;
                //j min1 < j min2 >= j+1 min1 < j+1 min2
            }
        }
    } else {
        newMin.min1 = second.min1;
        if (first.min1 == second.min1) {
            if (values[first.min2] < values[second.min2]) {
                newMin.min2 = first.min2;
            } else {
                newMin.min2 = second.min2;
            }
        } else {
            if (values[second.min2] < values[first.min1]) {
                newMin.min2 = second.min2;
            } else {
                newMin.min2 = first.min1;
            }
        }
    }
    return newMin;
}
int answer(const int left, const int right, std::vector<std::vector<mins>>& sparseChair,
           std::vector<int>& degrees, std::vector<int>& values) {
    int length =  right - left + 1;
    int level = degrees[length];
    int shift = 1 << level;
    return newMining(sparseChair[level][left - 1], sparseChair[level][right - shift], values).min2;
}
void buildChair(int numberAmount, std::vector<int>& degrees, std::vector<std::vector<mins>>& sparseChair,
                std::vector<int>& values){
    int levelSize0 = numberAmount;
    int levelSize;
    int shift = 1;
    for (int i = 1; i < degrees[numberAmount] + 1; ++i) {
        shift *= 2;
        levelSize = levelSize0 - shift + 1;
        for (int j = 0; j < levelSize; ++j) {
            if (i == 1) {
                int index1 = sparseChair[i - 1][j].min1;
                int index2 = sparseChair[i - 1][j + 1].min1;
                if (values[index1] > values[index2]) {
                    std::swap(index1, index2);
                }
                mins newMin(index1, index2);
                sparseChair[i].push_back(newMin);
                continue;
            }
            mins newMin = newMining(sparseChair[i - 1][j], sparseChair[i - 1][j + shift/2], values);
            sparseChair[i].push_back(newMin);
        }
    }
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int numberAmount, commandAmount;
    std::cin >> numberAmount >> commandAmount;
    std::vector<int> degrees(numberAmount + 1);
    degreesing(degrees);
    std::vector<std::vector<mins>> sparseChair(degrees[numberAmount] + 1);
    std::vector<int> values(numberAmount);
    for (int i = 0; i < numberAmount; ++i) {
        int value;
        std::cin >> value;
        values[i] = value;
        mins newMin(i, i);
        sparseChair[0].push_back(newMin);
    }
    buildChair(numberAmount, degrees, sparseChair, values);
    for (int i = 0; i < commandAmount; ++i) {
        int left, right;
        std::cin >> left >> right;
        std::cout << values[answer(left, right, sparseChair, degrees, values)] << '\n';
    }
}
