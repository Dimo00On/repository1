#include <iostream>
#include <vector>

const int kAlphabetBegin = 35; // 35 == '#'
const int kAlphabetSize = 88; // 88 = 26 + (97 - 35); 97 == 'a'

class SuffArrayFinder {
    int step = 0;
    int size = 0;
    std::string str;
    std::vector<int> positions;
    std::vector<int> classes;

    void preFindPositions() {
        std::vector<int> cnt(kAlphabetSize, 0);
        for (char letter : str) {
            ++cnt[letter - kAlphabetBegin];
        }
        for (int i = 1; i < kAlphabetSize; ++i) {
            cnt[i] += cnt[i - 1];
        }
        for (int i = size - 1; i >= 0; --i) {
            positions[--cnt[str[i] - kAlphabetBegin]] = i;
        }
    }
    void preFindClasses() {
        for (size_t i = 1; i < str.length(); ++i) {
            classes[positions[i]] = classes[positions[i - 1]];
            if (str[positions[i]] != str[positions[i - 1]]) {
                ++classes[positions[i]];
            }
        }
    }
    void reFindPositions() {
        std::vector<int> newPositions(size);
        std::vector<int> cnt(size, 0);
        for (int i = 0; i < size; ++i) {
            newPositions[i] = (positions[i] + size - (1 << step)) % size;
        }
        for (int i = 0; i < size; ++i) {
            ++cnt[classes[i]];
        }
        for (int i = 1; i < size; ++i) {
            cnt[i] += cnt[i - 1];
        }
        for (int i = size - 1; i >= 0; --i) {
            int clas = classes[newPositions[i]];
            positions[--cnt[clas]] = newPositions[i];
        }
    }
    void reFindClasses() {
        std::vector<int> newClasses(size, 0);
        for (int i = 1; i < size; ++i) {
            newClasses[positions[i]] = newClasses[positions[i - 1]];
            if (classes[positions[i]] != classes[positions[i - 1]] ||
                classes[positions[i] + (1 << step)] != classes[positions[i - 1] + (1 << step)]) {
                ++newClasses[positions[i]];
            }
        }
        std::swap(classes, newClasses);
    }

public:
    std::vector<int> getSuffArray(const std::string& newStr) {
        str = newStr + '#';
        size = static_cast<int>(str.length());
        positions.resize(size);
        classes.resize(size, 0);
        preFindPositions();
        preFindClasses();
        for (step = 0; (1 << step) < size; ++step) {
            reFindPositions();
            reFindClasses();
        }
        return positions;
    }
};

int main() {
    std::string str;
    std::cin >> str;
    SuffArrayFinder finder;
    auto answer = finder.getSuffArray(str);
    for (size_t i = 1; i < answer.size(); ++i) {
        std::cout << answer[i] + 1 << " ";
    }
    return 0;
}
