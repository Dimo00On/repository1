#include <iostream>
#include <vector>

void findAnswer(std::vector<std::vector<int>>& matrix, int size, int length, long long& sum,
                std::vector<std::vector<bool>>& masks) {
    for (int i = 0; i < (1 << size); ++i) {
        for (int j = 0; j < (1 << size); ++j) {
            int prevColor = 1;
            matrix[i][j] = 1;
            for (int u = 0; u < size; ++u) {
                int nowColor = 0;
                if (masks[i][u] || masks[j][u]) {
                    ++nowColor;
                }
                if (masks[i][u] && masks[j][u]) {
                    ++nowColor;
                }
                if (prevColor == 0 && nowColor == 0) {
                    matrix[i][j] = 0;
                    break;
                }
                if (prevColor == 2 && nowColor == 2) {
                    matrix[i][j] = 0;
                    break;
                }
                prevColor =  nowColor;
            }
        }
    }
    long long* dpLast = new long long[1 << size];
    for (int i = 0; i < (1 << size); ++i) {
        dpLast[i] = 1;
    }
    for (int i = 0; i < (length - 1); ++i) {
        long long* dpNow = new long long[1 << size];
        for (int j = 0; j < (1 << size); ++j) {
            dpNow[j] = 0;
            for (int u = 0; u < (1 << size); ++u) {
                dpNow[j] += (dpLast[u] * matrix[u][j]);
            }
        }
        delete[] dpLast;
        dpLast = dpNow;
    }
    for (int i = 0; i < (1 << size); ++i) {
        sum += dpLast[i];
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int size, length;
    std::cin >> size >> length;
    if (size > length) {
        int temp = size;
        size =  length;
        length = temp;
    }
    if (size == 1) {
        std::cout << (1 << length);
        return 0;
    }
    std::vector<std::vector<bool>> masks(1 << size);
    std::vector<std::vector<int>> matrix(1 << size);
    for (int i = 0; i < (1 << size); ++i) {
        matrix[i].resize(1 << size);
        int mask = i;
        for (int j = 0; j < size; ++j) {
            masks[i].push_back((mask % 2 == 0));
            mask /= 2;
        }
    }
    long long sum = 0;
    findAnswer(matrix, size, length, sum, masks);
    std::cout << sum << '\n';
}
