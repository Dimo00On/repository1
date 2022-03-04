#include <iostream>
#include <vector>

template<size_t N, typename Field = long long>
class Row {
private:
    std::vector<Field> value;
public:
    friend std::istream& operator>>(std::istream& in, Row<N, Field>& newRow) {
        for (size_t i = 0; i < N; ++i) {
            in >> newRow.value[i];
        }
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Row<N, Field>& newRow) {
        for (size_t i = 0; i < N; ++i) {
            out << newRow.value[i] << " ";
            out <<'\n';
        }
        return out;
    }

    Row();
    explicit Row(const std::vector<Field> &newVector);
    explicit Row(const char &zero);
    Row(const Row<N, Field> &copy);

    Row<N, Field> &operator=(const std::vector<Field> &newVector);
    const Field &operator[](const size_t &index) const;
    Field &operator[](const size_t &index);
    Row<N, Field> operator-() const;
    Row<N, Field> &operator+=(const Row<N, Field> &newRow);
    Row<N, Field> &operator-=(const Row<N, Field> &newRow);
    Row<N, Field> &operator*=(const Field &newValue);
    Row<N, Field> &operator/=(const Field &newValue);
    void resize(const size_t &newSize);
};

template<size_t N, typename Field>
Row<N, Field>::Row() {
    value.resize(N);
}

template<size_t N, typename Field>
Row<N, Field>::Row(const char &zero) {
    value.resize(N);
    for (size_t i = 0; i < N; ++i) {
        value[i] = Field(0);
    }
}

template<size_t N, typename Field>
Row<N, Field>::Row(const std::vector<Field> &newVector) {
    for (size_t i = 0; i < newVector.size(); ++i) {
        value.push_back(newVector[i]);
    }
}

template<size_t N, typename Field>
Row<N, Field> &Row<N, Field>::operator=(const std::vector<Field> &newVector) {
    for (size_t i = 0; i < newVector.size(); ++i) {
        value[i] = newVector[i];
    }
}

template<size_t N, typename Field>
const Field &Row<N, Field>::operator[](const size_t &index) const {
    return value[index];
}

template<size_t N, typename Field>
Field &Row<N, Field>::operator[](const size_t &index) {
    return value[index];
}

template<size_t N, typename Field>
Row<N, Field> Row<N, Field>::operator-() const {
    Row<N, Field> copy;
    for (size_t i = 0; i < N; ++i) {
        copy.value[i] = -value[i];
    }
    return copy;
}

template<size_t N, typename Field>
Row<N, Field> operator*(const Field &newValue, const Row<N, Field> &newRow) {
    return (newRow * newValue);
}

template<size_t N, typename Field>
Row<N, Field> operator*(const Row<N, Field> &newRow, const Field &newValue) {
    Row<N, Field> copy(newRow);
    copy *= newValue;
    return copy;
}

template<size_t N, typename Field>
Row<N, Field> &Row<N, Field>::operator-=(const Row<N, Field> &newRow) {
    *this += -newRow;
    return *this;
}

template<size_t N, typename Field>
Row<N, Field> operator-(const Row<N, Field> &first, const Row<N, Field> &second) {
    Row<N, Field> copy(first);
    copy -= second;
    return copy;
}

template<size_t N, typename Field>
Row<N, Field>::Row(const Row<N, Field> &copy) {
    value.resize(N);
    for (size_t i = 0; i < N; ++i) {
        value[i] = copy.value[i];
    }
}

template<size_t N, typename Field>
Row<N, Field> &operator+(const Row<N, Field> &first, const Row<N, Field> &second) {
    Row<N, Field> copy(first);
    copy += second;
    return copy;
}

template<size_t N, typename Field>
Row<N, Field> &Row<N, Field>::operator/=(const Field &newValue) {
    for (size_t i = 0; i < N; ++i) {
        value[i] /= newValue;
    }
    return *this;
}

template<size_t N, typename Field>
Row<N, Field> operator/(const Row<N, Field> &newRow, const Field &newValue) {
    Row<N, Field> copy(newRow);
    copy /= newValue;
    return copy;
}

template<size_t N, typename Field>
void Row<N, Field>::resize(const size_t &newSize) {
    value.resize(newSize);
}

template<size_t M, size_t N, typename Field = long long>
class Matrix {
private:
    std::vector<Row<N, Field>> rows;
public:

    friend std::istream& operator>>(std::istream& in, Matrix<M, N, Field>& newMatrix) {
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                int value;
                in >> value;
                newMatrix.rows[i][j] = Field(value);
            }
        }
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Matrix<M, N, Field>& newMatrix) {
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                out << newMatrix.rows[i][j] << " ";
            }
            out <<'\n';
        }
        out << '\n';
        return out;
    }

    Matrix();
    explicit Matrix(const std::vector<std::vector<Field>> &newVector);
    Matrix(const std::initializer_list<std::initializer_list<int>>& newMatrix);
    Matrix(const Matrix<M, N, Field> &copy);
    explicit Matrix(const char &zero);

    Matrix<M, N, Field> &operator=(const Matrix<M, N, Field> &copy);
    Matrix<M, N, Field> operator-() const;
    Matrix<M, N, Field> &operator+=(const Matrix<M, N, Field> &newMatrix);
    Matrix<M, N, Field> &operator-=(const Matrix<M, N, Field> &newMatrix);
    Matrix<M, N, Field> &operator*=(const Field &number);
    Row<N, Field> &operator[](const size_t &index);
    const Row<N, Field> &operator[](const size_t &index) const;
    Matrix<M, N, Field> &operator*=(const Matrix<M, N, Field> &newMatrix);

    Matrix<N, M, Field> transposed() const;
    Field det() const;
    Field trace() const;
    size_t rank() const;
    std::vector<Field> getRow(const size_t &index) const;
    std::vector<Field> getColumn(const size_t &index) const;
    Matrix<M, N, Field> inverted() const;
    void invert();
    void swapRows(const size_t& first, const size_t& second);
    void inDegree(const long long& degree);
};

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field>::Matrix() {
    rows.resize(M);
    for (size_t i = 0; i < M; ++i) {
        rows[i].resize(N);
    }
    if (N == M) {
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                rows[i][j] = Field(static_cast<int>(i == j));
            }
        }
    }
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field>::Matrix(const char &zero) {
    if (zero == '0') {
        rows.resize(M);
    }
    for (size_t i = 0; i < M; ++i) {
        rows[i].resize(N);
        for (size_t j = 0; j < N; ++j) {
            rows[i][j] = Field(0);
        }
    }
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field>::Matrix(const std::vector<std::vector<Field>> &newVector) {
    rows.resize(M);
    for (size_t i = 0; i < M; ++i) {
        rows[i] = newVector[i];
    }
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field>::Matrix(const std::initializer_list<std::initializer_list<int>>& newMatrix) {
    rows.resize(M);
    int i = 0;
    for (auto newRow : newMatrix) {
        int j = 0;
        rows[i].resize(N);
        for (auto element : newRow) {
            rows[i][j] = Field(element);
            ++j;
        }
        ++i;
    }
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field>::Matrix(const Matrix<M, N, Field> &copy) {
    rows.resize(M);
    for (size_t i = 0; i < M; ++i) {
        rows[i] = copy[i];
    }
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> &Matrix<M, N, Field>::operator=(const Matrix<M, N, Field> &copy) {
    for (size_t i = 0; i < M; ++i) {
        rows[i] = copy.rows[i];
    }
    return *this;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> Matrix<M, N, Field>::operator-() const {
    Matrix<M, N, Field> copy;
    for (size_t i = 0; i < M; ++i) {
        copy.rows[i] = -rows[i];
    }
    return copy;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> &Matrix<M, N, Field>::operator+=(const Matrix<M, N, Field> &newMatrix) {
    for (size_t i = 0; i < M; ++i) {
        rows[i] += newMatrix.rows[i];
    }
    return *this;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> &Matrix<M, N, Field>::operator-=(const Matrix<M, N, Field> &newMatrix) {
    *this += -newMatrix;
    return *this;
}

template<size_t M, size_t N, typename FirstField, size_t T, size_t U, typename SecondField>
bool operator==(const Matrix<M, N, FirstField> &first, const Matrix<T, U, SecondField> &second) {
    if (M != T || N != U) {
        return false;
    }
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (first[i][j] != second[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> &Matrix<M, N, Field>::operator*=(const Field &number) {
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            rows[i][j] *= number;
        }
    }
    return *this;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> operator*(const Field &number, const Matrix<M, N, Field> &matrix) {
    Matrix<M, N, Field> copy(matrix);
    copy *= number;
    return copy;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> operator*(const Matrix<M, N, Field> &matrix, const Field &number) {
    Matrix<M, N, Field> copy(matrix);
    copy *= number;
    return copy;
}

template<size_t M, size_t N, typename FirstField, size_t T, size_t U, typename SecondField>
bool operator!=(const Matrix<M, N, FirstField> &first, const Matrix<T, U, SecondField> &second) {
    return !(first == second);
}

template<size_t M, size_t N, typename Field>
Row<N, Field> &Matrix<M, N, Field>::operator[](const size_t &index) {
    return rows[index];
}

template<size_t M, size_t N, typename Field>
const Row<N, Field> &Matrix<M, N, Field>::operator[](const size_t &index) const {
    return rows[index];
}

template<size_t N, typename Field = long long>
using SquareMatrix = Matrix<N, N, Field>;

template<size_t M, size_t N, size_t K, typename Field>
Matrix<M, K, Field> operator*(const Matrix<M, N, Field> &first, const Matrix<N, K, Field> &second) {
    Matrix<M, K, Field> copy('0');
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            copy[i] += first[i][j] * second[j];
        }
    }
    return copy;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field> operator+(const Matrix<M, N, Field>& first, const Matrix<M, N, Field>& second) {
    Matrix<M, N, Field> copy(first);
    copy += second;
    return copy;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field> operator-(const Matrix<M, N, Field>& first, const Matrix<M, N, Field>& second) {
    Matrix<M, N, Field> copy(first);
    copy -= second;
    return copy;
}

template<size_t M, size_t N, typename Field>
Matrix<N, M, Field> Matrix<M, N, Field>::transposed() const {
    Matrix<N, M, Field> copy;
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            copy[j][i] = rows[i][j];
        }
    }
    return copy;
}

template<size_t M, size_t N, typename Field>
Field Matrix<M, N, Field>::det() const {
    static_assert(M == N);
    Matrix<M, N, Field> copy(*this);
    Field answer = Field(1);
    int sign = 1;
    size_t step = 0;
    for (size_t u = 0; u < N; ++u) {
        bool isFindNotZero = false;
        for (size_t i = step; i < M; ++i) {
            if (copy[i][u] != Field(0)) {
                ++step;
                Field value = copy[i][u];
                answer *= value;
                copy[i] /= value;
                for (size_t j = 0; j < M; ++j) {
                    if (j != i && copy[j][u] != Field(0)) {
                        value = copy[j][u];
                        copy[j] -= (copy[i] * value);
                    }
                }
                if (i > u) {
                    copy.swapRows(i, u);
                    sign = -sign;
                }
                isFindNotZero = true;
                break;
            }
        }
        if (!isFindNotZero) {
            return Field(0);
        }
    }
    answer *= Field(sign);
    return answer;
}

template<size_t M, size_t N, typename Field>
Field Matrix<M, N, Field>::trace() const {
    static_assert(M == N);
    Field answer = rows[0][0];
    for (size_t i = 1; i < M; ++i) {
        answer += rows[i][i];
    }
    return answer;
}

template<size_t M, size_t N, typename Field>
size_t Matrix<M, N, Field>::rank() const {
    Matrix<M, N, Field> copy(*this);
    size_t answer = 0;
    for (size_t u = 0; u < N; ++u) {
        for (size_t i = answer; i < M; ++i) {
            if (copy[i][u] != Field(0)) {
                ++answer;
                Field value = copy[i][u];
                copy[i] /= value;
                for (size_t j = 0; j < M; ++j) {
                    if (j != i && copy[j][u] != Field(0)) {
                        value = copy[j][u];
                        copy[j] -= (copy[i] * value);
                    }
                }
                if (i > u) {
                    copy.swapRows(i, u);
                }
                break;
            }
        }
    }
    return answer;
}

template<size_t M, size_t N, typename Field>
std::vector<Field> Matrix<M, N, Field>::getRow(const size_t &index) const {
    std::vector<Field> copy(N);
    for (size_t i = 0; i < N; ++i) {
        copy[i] = rows[index][i];
    }
    return copy;
}

template<size_t M, size_t N, typename Field>
std::vector<Field> Matrix<M, N, Field>::getColumn(const size_t &index) const {
    std::vector<Field> copy(M);
    for (size_t i = 0; i < M; ++i) {
        copy[i] = rows[i][index];
    }
    return copy;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> Matrix<M, N, Field>::inverted() const {
    static_assert(M == N);
    Matrix<M, N, Field> copy(*this);
    Matrix<M, N, Field> answer;
    size_t step = 0;
    for (size_t u = 0; u < N; ++u) {
        for (size_t i = step; i < M; ++i) {
            if (copy[i][u] != Field(0)) {
                ++step;
                Field value = copy[i][u];
                copy[i] /= value;
                answer[i] /= value;
                for (size_t j = 0; j < M; ++j) {
                    if (j != i && copy[j][u] != Field(0)) {
                        value = copy[j][u];
                        answer[j] -= (answer[i] * value);
                        copy[j] -= (copy[i] * value);
                    }
                }
                if (i > u) {
                    copy.swapRows(i, u);
                    answer.swapRows(i, u);
                }
                break;
            }
        }
    }
    return answer;
}

template<size_t M, size_t N, typename Field>
void Matrix<M, N, Field>::invert() {
    static_assert(M == N);
    *this = inverted();
}

template<size_t M, size_t N, typename Field>
void Matrix<M, N, Field>::swapRows(const size_t& first, const size_t& second) {
    Row<N, Field> copy(rows[first]);
    rows[first] = rows[second];
    rows[second] = copy;
}
//это был кусок моего matrix.h из введения в С++

const long long kModule = 999999937;
const int kSize = 5;

template<size_t N, typename Field>
Row<N, Field> &Row<N, Field>::operator*=(const Field &newValue) {
    for (size_t i = 0; i < N; ++i) {
        value[i] *= newValue;
        value[i] %= kModule;
    }
    return *this;
}

template<size_t N, typename Field>
Row<N, Field> &Row<N, Field>::operator+=(const Row<N, Field> &newRow) {
    for (size_t i = 0; i < N; ++i) {
        value[i] += newRow.value[i];
        value[i] %= kModule;
    }
    return *this;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> &Matrix<M, N, Field>::operator*=(const Matrix<M, N, Field> &newMatrix) {
    static_assert(M == N);
    Matrix<M, N, Field> copy('0');
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            copy.rows[i] += rows[i][j] * newMatrix[j];
        }
    }
    *this = copy;
    return *this;
}


template<size_t M, size_t N, typename Field>
void Matrix<M, N, Field>::inDegree(const long long& degree) {
    if (degree == 0) {
        *this = Matrix();
    }
    if (degree > 1) {
        Matrix<M, N, Field> copy(*this);
        inDegree(degree / 2);
        *this *= *this;
        if (degree % 2 == 1) {
            *this *= copy;
        }
    }
}

long long findAnswer(long long length, Matrix<kSize, kSize, long long> transferMatrix) {
    transferMatrix.inDegree(length);
    long long answer = 0;
    for (int i = 0; i < kSize; ++i) {
        for (int j = 0; j < kSize; ++j) {
            answer += transferMatrix[i][j];
            answer %= kModule;
        }
    }
    return answer;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    long long newLength = -1;\
    Matrix<kSize, kSize, long long> transferMatrix = {
            {1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1},
            {1, 1, 1, 0, 0},
            {1, 1, 1, 1, 1},
            {1, 1, 1, 0, 0},
    };
    std::cin >> newLength;
    while (newLength != 0) {
        std::cout << findAnswer(newLength - 1, transferMatrix) << '\n';
        std::cin >> newLength;
    }
}
