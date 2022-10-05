#include <iostream>
#include <vector>

const long long kModule = 1000000007;

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

void findAnswer(long long& answer, std::vector<int>& sizes, std::vector<long long>& lengths, int segmentAmount) {
    Matrix paths(1, sizes[0]);
    paths[0][0] = 1;
    for (int i = 0; i < segmentAmount; ++i) {
        if (lengths[i] == 0) {
            break;
        }
        if (i > 0) {
            Matrix newPaths(1, sizes[i]);
            for (int j = 0; j < std::min(paths.size2, sizes[i]); ++j) {
                newPaths[0][j] = paths[0][j];
            }
            paths = newPaths;
        }
        Matrix roads(sizes[i]);
        for (int j = 0; j < sizes[i]; ++j) {
            if (j > 0) {
                roads[j][j - 1] = 1;
            }
            if (j < sizes[i] - 1) {
                roads[j][j + 1] = 1;
            }
        }
        inDegree(roads, lengths[i]);
        paths = (paths * roads);
    }
    answer = paths[0][0];
}

int main() {
    int segmentAmount;
    long long destination;
    std::cin >> segmentAmount >> destination;
    std::vector<int> sizes(segmentAmount);
    std::vector<long long> lengths(segmentAmount);
    for (int i = 0; i < segmentAmount; ++i) {
        long long left, right;
        std::cin >> left >> right >> sizes[i];
        lengths[i] = std::max(std::min(destination, right) - left, static_cast<long long>(0));
        ++sizes[i];
    }
    long long answer = 0;
    findAnswer(answer, sizes, lengths, segmentAmount);
    std::cout << answer;
}
