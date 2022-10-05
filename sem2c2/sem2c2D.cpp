#include <iostream>
#include <vector>

const long long kModule = 1000000007;

int unitAmount(long long value) {
    int count = 0;
    while (value > 0) {
        count += static_cast<int>(value % 2);
        value /= 2;
    }
    return count;
}

void multiply(std::vector<std::vector<long long>>& matrix, std::vector<std::vector<long long>>& other, int amount) {
    std::vector<std::vector<long long>> answer(amount);
    for (int i = 0; i < amount; ++i) {
        answer[i].resize(amount, 0);
        for (int j = 0; j < amount; ++j) {
            for (int k = 0; k < amount; ++k) {
                answer[i][j] += matrix[i][k] * other[k][j];
                answer[i][j] %= kModule;
            }
        }
    }
    for (int i = 0; i < amount; ++i) {
        for (int j = 0; j < amount; ++j) {
            matrix[i][j] = answer[i][j];
        }
    }
}

void inDegree(std::vector<std::vector<long long>>& matrix, long long degree,
              std::vector<std::vector<long long>>& copy, int amount) {
    if (degree == 1) {
        return;
    }
    if (degree % 2 == 1) {
        inDegree(matrix, degree - 1, copy, amount);
        multiply(matrix, copy, amount);
    } else {
        inDegree(matrix, degree / 2, copy, amount);
        multiply(matrix, matrix, amount);
    }
}

void findAnswer(int amount, long long length, long long& answer, std::vector<long long>& numbers) {
    std::vector<std::vector<long long>> matrix(amount);
    std::vector<std::vector<long long>> copy(amount);
    for (int i = 0; i < amount; ++i) {
        matrix[i].resize(amount, 0);
        copy[i].resize(amount, 0);
        for (int j = 0; j < amount; ++j) {
            if (unitAmount(numbers[i] ^ numbers[j]) % 3  == 0) {
                matrix[i][j] = 1;
                copy[i][j] = 1;
            }
        }
    }
    inDegree(matrix, length - 1, copy, amount);
    answer = 0;
    for (int i = 0; i < amount; ++i) {
        for (int j = 0; j < amount; ++j) {
            answer += matrix[i][j];
            answer %= kModule;
        }
    }
}

int main() {
    int amount;
    long long length;
    std::cin >> amount >> length;
    std::vector<long long> numbers(amount);
    for (int i = 0; i < amount; ++i) {
        std::cin >> numbers[i];
    }
    if (length == 1) {
        std::cout << amount << '\n';
        return 0;
    }
    long long answer;
    findAnswer(amount, length, answer, numbers);
    std::cout << answer << '\n';
}
