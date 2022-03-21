#include <iostream>
#include <vector>
#include <map>

const long long kInfinity = -1e17;

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
            array[i].resize(size2, kInfinity);
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
                answer[i][j] = kInfinity;
            }
        }
        for (int i = 0; i < size1; ++i) {
            for (int j = 0; j < other.size2; ++j) {
                for (int k = 0; k < size2; ++k) {
                    if (j == other.size2 - 1) {
                        answer[i][j] = kInfinity;
                    } else {
                    answer[i][j] = std::max(answer[i][j], array[i][k] + other[k][j]);
                    }
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

void inDegree(Matrix& matrix, long long degree, Matrix& copy) {
    if (degree == 1) {
        return;
    }
    if (degree % 2 == 1) {
        inDegree(matrix, degree - 1, copy);
        matrix  = (matrix * copy);
    } else {
        inDegree(matrix, degree / 2, copy);
        matrix = (matrix * matrix);
    }
}

void findAnswer(long long& answer, std::map<std::string, std::pair<std::string*, int>>& wordPrices,
                 long long length, int allSize) {
    ++allSize;
    int amount = 0;
    std::vector<std::pair<std::string*, int>> words;
    std::vector<std::pair<std::string*, int>> fullWords;
    words.reserve(wordPrices.size());
    for (const auto& pair : wordPrices) {
        if (pair.second.second > 0) {
            fullWords.emplace_back(pair.second);
            ++amount;
        }
        words.emplace_back(pair.second);
    }
    auto ptr = new std::string("");
    words.emplace_back(ptr, 0);
    Matrix matrix(allSize, allSize);
    for (int i = 0; i < allSize - 1; ++i) { //from j word go to i word and find u word in middle
        for (int j = 0; j < allSize; ++j) {
            std::string newString = *words[j].first + (*words[i].first)[(*words[i].first).size() - 1];
            int size = static_cast<int>(newString.size());
            bool ok = true;
            for (int t = size - 1, count = 0; count < (*words[i].first).size(); --t, ++count) {
                if (newString[t] != (*words[i].first)[(*words[i].first).size() - 1 - count]) {
                    ok = false;
                    break;
                }
            }
            if (!ok) {
                continue;
            }
            if (matrix[j][i] == kInfinity) {
                matrix[j][i] = 0;
            }
            for (int u = 0; u < amount; ++u) {
                if ((*fullWords[u].first).size() > newString.size()) {
                    continue;
                }
                ok = true;
                for (int t = size - 1, count = 0; count < (*fullWords[u].first).size(); --t, ++count) {
                    if (newString[t] != (*fullWords[u].first)[(*fullWords[u].first).size() - 1 - count]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    matrix[j][i] += fullWords[u].second;
                }
            }
        }
    }
    wordPrices.clear();
    fullWords.clear();
    Matrix copy(matrix);
    inDegree(matrix, length, copy);
    long long ans1 = 0;
    for (int i = 0; i < allSize; ++i) {
        ans1 = std::max(ans1, matrix[allSize - 1][i]);
    }
    answer = ans1;
    for (auto & word : words) {
        delete word.first;
    }
}

int main() {
    int amount;
    long long length;
    int allSize = 0;
    std::cin >> amount >> length;
    std::vector<int> prices(amount);
    std::map<std::string, std::pair<std::string*, int>> wordPrices;
    for (int i = 0; i < amount; ++i) {
        std::cin >> prices[i];
    }
    for (int i = 0; i < amount; ++i) {
        std::string str;
        std::cin >> str;
        if (!wordPrices.count(str)) {
            auto ptr = new std::string(str);
            wordPrices.insert(std::make_pair(str, std::make_pair(ptr, prices[i])));
            ++allSize;
        } else {
            wordPrices[str].second += prices[i];
        }
        std::string prefix;
        for (char j : str) {
            prefix += j;
            if (!wordPrices.count(prefix)) {
                auto ptr2 = new std::string(prefix);
                wordPrices.insert(std::make_pair(prefix, std::make_pair(ptr2, 0)));
                ++allSize;
            }
        }
    }
    prices.clear();
    long long answer = 0;
    findAnswer(answer, wordPrices, length, allSize);
    std::cout << answer;
}
