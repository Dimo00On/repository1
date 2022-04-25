#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

const int kSize = 4;
const int kModule = 16;
const char kInfinity = 120;
const int kAccuracy = 150;

char getEmptyPos(uint64_t pos, std::vector<uint64_t>& degree) {
    for (char i = 0; i < kModule; ++i) {
        if ((pos >> degree[i]) % kModule == 0) {
            return i;
        }
    }
    return -1;
}

uint64_t changePos(uint64_t pos, std::vector<uint64_t>& degree, char type, char emptyPos) {
    char otherPos = emptyPos;
    if (type == 'U') {
        otherPos -= 4;
    }
    if (type == 'D') {
        otherPos += 4;
    }
    if (type == 'L') {
        --otherPos;
    }
    if (type == 'R') {
        ++otherPos;
    }
    uint64_t otherVal = (pos >> degree[otherPos]) % kModule;
    pos -= otherVal * ((static_cast<uint64_t>(1) << degree[otherPos]) - (static_cast<uint64_t>(1) << degree[emptyPos]));
    return pos;
}

struct Binary{
    std::vector<char> value;
    std::vector<int> operation;
    int size = 0;
    std::vector<int> position;
    void siftUp(int index);
    void insert(char value, int operation);
    void siftDown(int index);
    void extractMin();
    void decreaseKey(int hisOperation, char delta);
    int getMinOperation();
    bool contains(int hisOperation);
    char getValue(int hisOperation);
    char getMin();
};

char Binary::getMin() {
    return value[1];
}

bool Binary::contains(int hisOperation) {
    if (position[hisOperation] == -1) {
        return false;
    }
    return true;
}

char Binary::getValue(int hisOperation) {
    return value[position[hisOperation]];
}

void Binary::siftUp(int index) {
    while (index != 1) {
        if (value[index] < value[index / 2]) {
            std::swap(position[operation[index]], position[operation[index / 2]]);
            std::swap(value[index], value[index / 2]);
            std::swap(operation[index], operation[index / 2]);
            index /= 2;
        } else {
            break;
        }
    }
}

void Binary::siftDown(int index) {
    while (2 * index <= size) {
        int newIndex = 2 * index;
        if (newIndex + 1 <= size && value[newIndex + 1] < value[newIndex]) {
            ++newIndex;
        }
        if (value[newIndex] < value[index]) {
            std::swap(position[operation[index]], position[operation[newIndex]]);
            std::swap(value[index], value[newIndex]);
            std::swap(operation[index], operation[newIndex]);
            index = newIndex;
        } else {
            break;
        }
    }
}

void Binary::decreaseKey(int hisOperation, char delta) {
    value[position[hisOperation]] -= delta;
    siftUp(position[hisOperation]);
}

void Binary::insert(char newValue, int newOperation) {
    value.push_back(newValue);
    operation.push_back(newOperation);
    ++size;
    position.push_back(size);
    siftUp(size);
}

int Binary::getMinOperation() {
    return operation[1];
}

void Binary::extractMin(){
    std::swap(value[1], value[size]);
    std::swap(operation[1], operation[size]);
    position[operation[1]] = 1;
    position[operation[size]] = -1;
    --size;
    value.pop_back();
    operation.pop_back();
    siftDown(1);
}

bool checkIsOk(std::vector<std::vector<char>>& start) {
    int counter = 0;
    std::vector<char> elements;
    for (int i = 0; i < kSize; ++i) {
        for (int j = 0; j < kSize; ++j) {
            if (start[i][j] == 0) {
                counter += (i + 1);
            }
            else {
                elements.push_back(start[i][j]);
            }
        }
    }
    for (int i = 0; i < kModule - 1; ++i) {
        for (int j = i + 1; j < kModule - 1; ++j) {
            if (elements[i] > elements[j]) {
                ++counter;
            }
        }
    }
    if (counter % 2 == 1) {
        return false;
    }
    return true;
}

char approx(uint64_t pos, std::vector<std::pair<int, int>>& forApprox, std::vector<uint64_t>& degree) {
    std::vector<std::vector<char>> position(kSize);
    for (int i = 0; i < kModule; ++i) {
        position[i / 4].push_back(static_cast<char>((pos >> degree[i]) % kModule));
    }
    char answer = 0;
    for (int i = 0; i < kSize; ++i) {
        for (int j = 0; j < kSize; ++j) {
            if (position[i][j] != 0) {
                answer += (std::abs(i - forApprox[position[i][j]].first) + std::abs(j - forApprox[position[i][j]].second));
            }
        }
    }
    for (int i = 0; i < kSize; ++i) {
        bool badLine = false;
        bool badColumn = false;
        for (int j = 0; j < kSize; ++j) {
            if (!badLine && position[i][j] != 0) {
                for (int k = j + 1; k < kSize; ++k) {
                    if (forApprox[position[i][j]].first == i && forApprox[position[i][k]].first == i
                        && forApprox[position[i][j]].second > forApprox[position[i][k]].second
                        && position[i][k] != 0) {
                        answer += 2;
                        badLine = true;
                        break;
                    }
                }
            }
            if (!badColumn && position[j][i] != 0) {
                for (int k = j + 1; k < kSize; ++k) {
                    if (position[k][i] != 0
                        && forApprox[position[j][i]].second == i && forApprox[position[k][i]].second == i
                        && forApprox[position[j][i]].first > forApprox[position[k][i]].first) {
                        answer += 2;
                        badColumn = true;
                        break;
                    }
                }
            }
        }
    }
    return answer;
}

void addNewPos(uint64_t from, std::vector<std::pair<int, int>>& forApprox,
               Binary& heap, std::unordered_map<uint64_t, char>& realDist,
               char letter, char emptyPos, std::unordered_map<uint64_t, std::pair<uint64_t, char>>& steps,
               std::vector<uint64_t>& degree, char changeType,
               Binary& otherHeap, std::unordered_map<uint64_t, int>& numbers, int& counter,
               std::vector<uint64_t>& positions) {
    uint64_t to = changePos(from, degree, changeType, emptyPos);
    char fromDist = realDist[from];
    char approximation = approx(to, forApprox, degree);
    if (realDist.count(to)) {
        char toDist = realDist[to];
        if ((toDist > fromDist + 1)) {
            int number = numbers[to];
            realDist[to] = fromDist + 1;
            char delta = heap.getValue(number) - fromDist - 1 - approximation;
            char approxDist = fromDist + 1 + approximation;
            if (heap.contains(number)) {
                heap.decreaseKey(number, delta);
            } else {
                heap.insert(approxDist, number);
            }
            steps[to].first = from;
            steps[to].second = letter;
        }
    } else {
        realDist.insert(std::make_pair(to, fromDist + 1));
        if (numbers.count(to)) {
            heap.insert(fromDist + 1 + approximation, numbers[to]);
        } else {
            ++counter;
            positions.push_back(to);
            numbers.insert(std::make_pair(to, counter));
            heap.insert(fromDist + 1 + approximation, counter);
        }
        steps.insert(std::make_pair(to, std::make_pair(from, letter)));
        otherHeap.position.push_back(-1);
    }
}

int findAnswer(uint64_t start, uint64_t end, std::unordered_map<uint64_t, std::pair<uint64_t, char>>& stepsLeft,
               std::unordered_map<uint64_t, std::pair<uint64_t, char>>& stepsRight, uint64_t& foundVertex,
               std::vector<uint64_t>& degree, std::vector<std::pair<int, int>>& forApproxLeft,
               std::vector<std::pair<int, int>>& forApproxRight) {
    std::unordered_map<uint64_t, char> realDistLeft;
    std::unordered_map<uint64_t, char> realDistRight;
    std::unordered_set<uint64_t> usedLeft;
    std::unordered_set<uint64_t> usedRight;
    std::unordered_map<uint64_t, int> numbers;
    std::vector<uint64_t> positions;
    positions.push_back(end);
    positions.push_back(start);
    numbers.insert(std::make_pair(start, 1));
    numbers.insert(std::make_pair(end, 0));
    realDistLeft.insert(std::make_pair(start, 0));
    realDistRight.insert(std::make_pair(end, 0));
    stepsLeft.insert(std::make_pair(start, std::make_pair(-1, 'o')));
    stepsRight.insert(std::make_pair(end, std::make_pair(-1, 'o')));
    Binary heapLeft;
    Binary heapRight;
    heapLeft.value.push_back(-1);
    heapLeft.operation.push_back(-1);
    heapRight.value.push_back(-1);
    heapRight.operation.push_back(-1);
    heapLeft.position.push_back(-1);
    heapLeft.insert(approx(start, forApproxLeft, degree), 1);
    heapRight.insert(approx(end, forApproxRight, degree), 0);
    heapRight.position.push_back(-1);
    uint64_t from;
    int number;
    int counter = 1;
    char value;
    int foundCount = 0;
    char minVal = kInfinity;
    while (true) {
            number = heapLeft.getMinOperation();
            value = heapLeft.getMin();
            from = positions[number];
            heapLeft.extractMin();
            usedLeft.insert(from);
            if (usedRight.count(from) || from == end) {
                ++foundCount;
                char realVal = realDistLeft[from] + realDistRight[from];
                if (value == realVal || foundCount > kAccuracy || from == end) {
                    if (minVal > realVal) {
                        foundVertex = from;
                    }
                    break;
                }
                if (minVal > realVal) {
                    foundVertex = from;
                    minVal = realVal;
                }
            }
            {
                char emptyPos = getEmptyPos(from, degree);
                int line = emptyPos / 4;
                int column = emptyPos % 4;
                if (line < 3) {
                    addNewPos(from, forApproxLeft, heapLeft,  realDistLeft, 'U', emptyPos,
                              stepsLeft, degree, 'D', heapRight, numbers, counter, positions);
                }
                if (line > 0) {
                    addNewPos(from, forApproxLeft, heapLeft, realDistLeft, 'D', emptyPos,
                              stepsLeft, degree, 'U', heapRight, numbers, counter, positions);
                }
                if (column < 3) {
                    addNewPos(from, forApproxLeft, heapLeft, realDistLeft, 'L', emptyPos,
                              stepsLeft, degree, 'R', heapRight, numbers, counter, positions);
                }
                if (column > 0) {
                    addNewPos(from, forApproxLeft, heapLeft, realDistLeft, 'R', emptyPos,
                              stepsLeft, degree, 'L', heapRight, numbers, counter, positions);
                }
            }
            number = heapRight.getMinOperation();
            value = heapRight.getMin();
            from = positions[number];
            heapRight.extractMin();
            usedRight.insert(from);
            if (usedLeft.count(from) || from == start) {
                ++foundCount;
                char realVal = realDistLeft[from] + realDistRight[from];
                if (value == realVal || foundCount > kAccuracy || from == start) {
                    if (minVal > realVal) {
                        foundVertex = from;
                    }
                    break;
                }
                if (minVal > realVal) {
                    foundVertex = from;
                    minVal = realVal;
                }
            }
            char emptyPos = getEmptyPos(from, degree);
            int line = emptyPos / 4;
            int column = emptyPos % 4;
            if (line < 3) {
                addNewPos(from, forApproxRight, heapRight, realDistRight, 'D', emptyPos,
                          stepsRight, degree, 'D', heapLeft, numbers, counter, positions);
            }
            if (line > 0) {
                addNewPos(from, forApproxRight, heapRight, realDistRight, 'U', emptyPos,
                          stepsRight, degree, 'U', heapLeft, numbers, counter, positions);
            }
            if (column < 3) {
                addNewPos(from, forApproxRight, heapRight, realDistRight, 'R', emptyPos,
                          stepsRight, degree, 'R', heapLeft, numbers, counter, positions);
            }
            if (column > 0) {
                addNewPos(from, forApproxRight, heapRight, realDistRight, 'L', emptyPos,
                          stepsRight, degree, 'L', heapLeft, numbers, counter, positions);
            }
    }
    return (realDistLeft[foundVertex] + realDistRight[foundVertex]);
}

int main() {
    std::vector<uint64_t> degree(kModule);
    for (int i = kModule - 1; i >= 0; --i) {
        degree[i] = 4 * (kModule - i - 1);
    }
    uint64_t start = 0;
    uint64_t end = 0;
    std::vector<std::vector<char>> forCheck(kSize, std::vector<char>(kSize));
    std::vector<std::pair<int, int>> forApproxLeft(kModule);
    std::vector<std::pair<int, int>> forApproxRight(kModule);
    for (int i = 0; i < kSize; ++i) {
        for (int j = 0; j < kSize; ++j) {
            uint64_t value;
            std::cin >> value;
            forApproxRight[value] = std::make_pair(i, j);
            forApproxLeft[(4 * i + j + 1) % kModule] = std::make_pair(i, j);
            forCheck[i][j] = static_cast<char>(value);
            start *= kModule;
            start += value;
            end *= kModule;
            end += ((4 * i + j + 1) % kModule);
        }
    }
    if (!checkIsOk(forCheck)) {
        std::cout << -1;
        return 0;
    }
    if (start == end) {
        std::cout << 0;
        return 0;
    }
    std::unordered_map<uint64_t, std::pair<uint64_t, char>> stepsLeft;
    std::unordered_map<uint64_t, std::pair<uint64_t, char>> stepsRight;
    uint64_t foundVertex;
    int stepAmount = findAnswer(start, end, stepsLeft, stepsRight, foundVertex, degree, forApproxLeft, forApproxRight);
    std::cout << stepAmount << '\n';
    if (stepAmount > 0) {
        std::pair<uint64_t, char> nextStep = stepsLeft[foundVertex];
        std::vector<char> answer;
        while (nextStep.first != -1) {
            answer.push_back(nextStep.second);
            nextStep = stepsLeft[nextStep.first];
        }
        std::reverse(answer.begin(), answer.end());
        nextStep = stepsRight[foundVertex];
        while (nextStep.first != -1) {
            answer.push_back(nextStep.second);
            nextStep = stepsRight[nextStep.first];
        }
        for (int i = 0; i < stepAmount; ++i) {
            std::cout << answer[i];
        }
    }
}