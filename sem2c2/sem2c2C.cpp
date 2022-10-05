#include <iostream>
#include <vector>

const int kModule = 1000000007;

struct Matrix{
    std::vector<std::vector<long long>> array;
    int size1;
    int size2;

    const std::vector<long long>& operator[](int index) const{
        return array[index];
    }
    std::vector<long long>& operator[](int index) {
        return array[index];
    }
    Matrix(int newSize1, int newSize2) {
        size1 = newSize1;
        size2 = newSize2;
        array.resize(size1);
        for (int i = 0; i < size1; ++i) {
            array[i].resize(size2, 0);
        }
    }
    explicit Matrix(int newSize) {
        size1 = newSize;
        size2 = newSize;
        array.resize(size1);
        for (int i = 0; i < size1; ++i) {
            array[i].resize(size1, 0);
            array[i][i] = 1;
        }
    }
    Matrix(const Matrix& other) {
        size1 = other.size1;
        size2 = other.size2;
        array.resize(size1);
        for (int i = 0; i < size1; ++i) {
            array[i].resize(size2);
            for (int j = 0; j < size2; ++j) {
                array[i][j] = other[i][j];
            }
        }
    }
    Matrix operator*(Matrix& other) {
        Matrix answer(size1, other.size2);
        for (int i = 0; i < size1; ++i) {
            for (int j = 0; j < other.size2; ++j) {
                for (int k = 0; k < size2; ++k) {
                    answer[i][j] += array[i][k] * other[k][j];
                    answer[i][j] %= kModule;
                }
            }
        }
        return answer;
    }
    Matrix& operator=(const Matrix& other) {
        if (this == &other) {
            return *this;
        }
        size1 = other.size1;
        size2 = other.size2;
        array.resize(size1);
        for (int i = 0; i < size1; ++i) {
            array[i].resize(size2);
            for (int j = 0; j < size2; ++j) {
                array[i][j] = other[i][j];
            }
        }
        return *this;
    }
};

void inDegree(Matrix& matrix, long long degree) {
    if (degree == 1) {
        return;
    }
    if (degree % 2 == 1) {
        Matrix copy(matrix);
        inDegree(matrix, degree - 1);
        matrix  = (matrix * copy);
    } else {
        inDegree(matrix, degree / 2);
        matrix = (matrix * matrix);
    }
}

void findAnswer(int caveAmount, int length, std::vector<int>& startPath, std::vector<int>& middlePath,
                std::vector<int>& endPrice, long long& answer, int cost) {
    Matrix matrix(cost, cost);
    for (int i = 0; i < cost; ++i) {
        for (int j = 0; j < cost; ++j) {
            matrix[i][j] += middlePath[(j - i + cost) % cost];
        }
    }
    inDegree(matrix, length - 2);
    Matrix start(1, cost);
    for (int i = 0; i < cost; ++i) {
        start[0][i] = startPath[i];
    }
    start = (start * matrix);
    for (int i = 0; i < cost; ++i) {
        for (int j = 0; j < caveAmount; ++j) {
            if ((i + endPrice[j]) % cost == 0) {
                answer += start[0][i];
                answer %= kModule;
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int caveAmount, length, cost;
    std::cin >> caveAmount >> length >> cost;
    std::vector<int> startPath(cost, 0);
    std::vector<int> middlePath(cost, 0);
    std::vector<int> endPrice(caveAmount);
    for (int i = 0; i < caveAmount; ++i) {
        int value;
        std::cin >> value;
        ++startPath[value % cost];
    }
    for (int i = 0; i < caveAmount; ++i) {
        int value;
        std::cin >> value;
        endPrice[i] = value;
        ++middlePath[value % cost];
    }
    for (int i = 0; i < caveAmount; ++i) {
        int value;
        std::cin >> value;
        endPrice[i] += (value % cost);
    }
    long long answer = 0;
    if (length == 2) {
        for (int i = 0; i < cost; ++i) {
            for (int j = 0; j < caveAmount; ++j) {
                if ((i + endPrice[j]) % cost == 0) {
                    answer += startPath[i];
                }
            }
        }
        std::cout << answer << '\n';
        return 0;
    }
    findAnswer(caveAmount, length, startPath, middlePath, endPrice, answer, cost);
    std::cout << answer << '\n';
}
