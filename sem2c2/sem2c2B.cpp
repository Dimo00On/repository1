#include <iostream>
#include <vector>

const long long kModule = 1000000007;
const long long kReverseTwo = 500000004;

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

void findAnswerUp(long long& answerUp, std::vector<long long>& numbers, int amount) {
    Matrix matrix(2);
    Matrix reverseMatrix(2);
    matrix[0][0] = 1;
    matrix[0][1] = 1;
    matrix[1][0] = 2;
    matrix[1][1] = 0;
    reverseMatrix[0][0] = 0;
    reverseMatrix[0][1] = kReverseTwo;
    reverseMatrix[1][0] = 1;
    reverseMatrix[1][1] = kModule - kReverseTwo;
    for (int i = 0; i < amount; ++i) {
        inDegree(matrix, numbers[i]);
    }
    matrix = (matrix * reverseMatrix);
    answerUp = matrix[0][1];
}

void smallInDegree(long long& value, long long degree, long long start) {
    if (degree == 1) {
        return;
    }
    smallInDegree(value, degree / 2, start);
    value *= value;
    value %= kModule;
    if (degree % 2 == 1) {
        value *= start;
        value %= kModule;
    }
}

void findAnswerDown(long long& answerDown, std::vector<long long>& numbers, int amount) {
    for (int i = 0; i < amount; ++i) {
        smallInDegree(answerDown, numbers[i], answerDown);
    }
    answerDown *= kReverseTwo;
    answerDown %= kModule;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int amount;
    std::cin >> amount;
    std::vector<long long> numbers;
    bool isOne = true;
    long long value = 1;
    for (int i = 0; i < amount; ++i) {
        long long newValue;
        std::cin >> newValue;
        if (newValue > 1) {
            isOne = false;
        }
        value %= (kModule - 1);
        newValue %= (kModule - 1);
        value *= newValue;
    }
    if (value == 0) {
        value = kModule - 1;
    }
    if (value != 1) {
        isOne = false;
        numbers.push_back(value);
    }
    amount = static_cast<int>(numbers.size());
    if (isOne) {
        std::cout << "0/1" << '\n';
        return 0;
    }
    long long answerUp = 0;
    long long answerDown = 2;
    findAnswerUp(answerUp, numbers, amount);
    findAnswerDown(answerDown, numbers, amount);
    std::cout << answerUp << "/" << answerDown << '\n';
}
