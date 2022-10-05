#include <iostream>
#include <vector>
#include <algorithm>

const int kAlphabetStart = 48;
const int kNumberSystem = 10;

bool isZero(std::vector<int>& length) {
    for (int i = 0; i < length.size(); ++i) {
        if (length[i] != 0) {
            return false;
        }
    }
    return true;
}

void stringToInt(std::string& lengthString, std::vector<int>& length) {
    for (auto i : lengthString) {
        length.push_back(i - kAlphabetStart);
    }
    std::reverse(length.begin(), length.end());
}

void divide(std::vector<int>& length) {
    for (int i = length.size() - 1; i >= 0; --i) {
        if (length[i] % 2 == 1) {
            --length[i];
            length[i - 1] += kNumberSystem;
        }
        length[i] /= 2;
    }
}

void multiply(std::vector<std::vector<int>>& matrix, std::vector<std::vector<int>> other, int module) {
    std::vector<std::vector<int>> copy(matrix.size());
    for (int i = 0; i < matrix.size(); ++i) {
        copy[i].resize(matrix.size());
        for (int j = 0; j < matrix.size(); ++j) {
            copy[i][j] = matrix[i][j];
        }
    }
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix.size(); ++j) {
            matrix[i][j] = 0;
            for (int k = 0; k < matrix.size(); ++k) {
                matrix[i][j] += ((copy[i][k] * other[k][j]) % module);
                matrix[i][j] %= module;
            }
        }
    }
}

void inDegree(std::vector<std::vector<int>>& matrix, std::vector<std::vector<int>>& copy,
              std::vector<int>& length, int module) {
    if (isZero(length)) {
        return;
    }
    if (length[0] % 2 == 1) {
        --length[0];
        inDegree(matrix, copy, length, module);
        multiply(matrix, copy, module);
    } else {
        divide(length);
        inDegree(matrix, copy, length, module);
        multiply(matrix, matrix, module);
    }
}

void minusOne(std::vector<int>& length, int step) {
    if (length[step] > 0) {
        --length[step];
    } else {
        minusOne(length, step + 1);
        length[step] = kNumberSystem - 1;
    }
}

void findAnswer(std::vector<std::vector<int>>& matrix, int size, std::vector<int>& length, int& sum,
                std::vector<std::vector<bool>>& masks, int module) {
    std::vector<std::vector<int>> copy((1 << size));
    for (int i = 0; i < (1 << size); ++i) {
        copy[i].resize((1 << size));
        for (int j = 0; j < (1 << size); ++j) {
            int prevColor = 1;
            matrix[i][j] = static_cast<int>(i == j);
            copy[i][j] = 1;
            for (int u = 0; u < size; ++u) {
                int nowColor = 0;
                if (masks[i][u] || masks[j][u]) {
                    ++nowColor;
                }
                if (masks[i][u] && masks[j][u]) {
                    ++nowColor;
                }
                if ((prevColor == 0 && nowColor == 0)
                    || (prevColor == 2 && nowColor == 2)) {
                    copy[i][j] = 0;
                    break;
                }
                prevColor =  nowColor;
            }
        }
    }
    minusOne(length, 0);
    inDegree(matrix, copy, length, module);
    for (int i = 0; i < (1 << size); ++i) {
        for (int j = 0; j < (1 << size); ++j) {
            sum += matrix[i][j];
            sum %= module;
        }
    }
}

void smallInDegree(std::vector<int>& length, int& answer, int module) {
    if (isZero(length)) {
        return;
    }
    if (length[0] % 2 == 1) {
        --length[0];
        smallInDegree(length, answer, module);
        answer *= 2;
    } else {
        divide(length);
        smallInDegree(length, answer, module);
        answer *= answer;
    }
    answer %= module;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int size, module;
    std::string lengthString;
    std::vector<int> length;
    std::cin >> lengthString;
    stringToInt(lengthString, length);
    std::cin >> size;
    std::cin >> module;
    if (size == 1) {
        int answer = 1;
        smallInDegree(length, answer, module);
        std::cout << answer;
        return 0;
    }
    if (length[0] == 1 && length.size() == 1) {
        std::cout << (1 << size);
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
    int sum = 0;
    findAnswer(matrix, size, length, sum, masks, module);
    std::cout << sum << '\n';
}
