#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

const int kSize = 4;
const int kModule = 16;
const char kInfinity = 120;
const int kAccuracy = 150;

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

class Solver { //а чё так можно было что-ли?
private:
    std::vector<uint64_t> degree;
    uint64_t start = 0;
    uint64_t end = 0;
    std::vector<std::vector<char>> forCheck;
    std::vector<std::pair<int, int>> forApprox[2]; // 0 == left, 1 == right
    std::unordered_map<uint64_t, std::pair<uint64_t, char>> steps[2];
    uint64_t foundVertex;
    std::unordered_map<uint64_t, char> realDist[2];
    std::unordered_set<uint64_t> used[2];
    std::unordered_map<uint64_t, int> numbers;
    std::vector<uint64_t> positions;
    Binary heap[2];
    int counter = 1;
    int foundCount = 0;
    char minVal = kInfinity;
    bool timeToStop = false;

public:
    int stepAmount;
    std::vector<char> answer;

public:
    Solver();
    void read(int i, int j, uint64_t value);
    bool checkIsOk();
    bool isStartEqualsEnd() {
        return start == end;
    };
    void createAnswer();
    void prepare();
    void findAnswer();
    char getEmptyPos(uint64_t pos);
    uint64_t changePos(uint64_t pos, char type, char emptyPos);
    char approx(uint64_t pos, int side);
    void addNewPos(uint64_t from, int side, char letter, char emptyPos, char changeType);
    void newStep(int side);
};

char Solver::getEmptyPos(uint64_t pos) {
    for (char i = 0; i < kModule; ++i) {
        if ((pos >> degree[i]) % kModule == 0) {
            return i;
        }
    }
    return -1;
}

uint64_t Solver::changePos(uint64_t pos, char type, char emptyPos) {
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

bool Solver::checkIsOk() {
    int counter = 0;
    std::vector<char> elements;
    for (int i = 0; i < kSize; ++i) {
        for (int j = 0; j < kSize; ++j) {
            if (forCheck[i][j] == 0) {
                counter += (i + 1);
            }
            else {
                elements.push_back(forCheck[i][j]);
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

char Solver::approx(uint64_t pos, int side) {
    std::vector<std::vector<char>> position(kSize);
    for (int i = 0; i < kModule; ++i) {
        position[i / 4].push_back(static_cast<char>((pos >> degree[i]) % kModule));
    }
    char answer = 0;
    for (int i = 0; i < kSize; ++i) {
        for (int j = 0; j < kSize; ++j) {
            if (position[i][j] != 0) {
                answer += (std::abs(i - forApprox[side][position[i][j]].first) + std::abs(j - forApprox[side][position[i][j]].second));
            }
        }
    }
    for (int i = 0; i < kSize; ++i) {
        bool badLine = false;
        bool badColumn = false;
        for (int j = 0; j < kSize; ++j) {
            if (!badLine && position[i][j] != 0) {
                for (int k = j + 1; k < kSize; ++k) {
                    if (forApprox[side][position[i][j]].first == i && forApprox[side][position[i][k]].first == i
                        && forApprox[side][position[i][j]].second > forApprox[side][position[i][k]].second
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
                        && forApprox[side][position[j][i]].second == i && forApprox[side][position[k][i]].second == i
                        && forApprox[side][position[j][i]].first > forApprox[side][position[k][i]].first) {
                        answer += 2;
                        badColumn = true;
                        break;
                    }
                }
            }
        }
    }
    return 2 * answer; //-No, you can't just multiply heuristic! -Hehe, accuracy makes brrrrr
}

void Solver::addNewPos(uint64_t from, int side, char letter, char emptyPos, char changeType) {
    uint64_t to = changePos(from, changeType, emptyPos);
    char fromDist = realDist[side][from];
    char approximation = approx(to, side);
    if (realDist[side].count(to)) {
        char toDist = realDist[side][to];
        if ((toDist > fromDist + 1)) {
            int number = numbers[to];
            realDist[side][to] = fromDist + 1;
            char delta = heap[side].getValue(number) - fromDist - 1 - approximation;
            char approxDist = fromDist + 1 + approximation;
            if (heap[side].contains(number)) {
                heap[side].decreaseKey(number, delta);
            } else {
                heap[side].insert(approxDist, number);
            }
            steps[side][to].first = from;
            steps[side][to].second = letter;
        }
    } else {
        realDist[side].insert(std::make_pair(to, fromDist + 1));
        if (numbers.count(to)) {
            heap[side].insert(fromDist + 1 + approximation, numbers[to]);
        } else {
            ++counter;
            positions.push_back(to);
            numbers.insert(std::make_pair(to, counter));
            heap[side].insert(fromDist + 1 + approximation, counter);
        }
        steps[side].insert(std::make_pair(to, std::make_pair(from, letter)));
        heap[1 - side].position.push_back(-1);
    }
}

void Solver::prepare() {
    positions.push_back(end);
    positions.push_back(start);
    numbers.insert(std::make_pair(start, 1));
    numbers.insert(std::make_pair(end, 0));
    realDist[0].insert(std::make_pair(start, 0));
    realDist[1].insert(std::make_pair(end, 0));
    steps[0].insert(std::make_pair(start, std::make_pair(-1, 'o')));
    steps[1].insert(std::make_pair(end, std::make_pair(-1, 'o')));
    heap[0].value.push_back(-1);
    heap[0].operation.push_back(-1);
    heap[1].value.push_back(-1);
    heap[1].operation.push_back(-1);
    heap[0].position.push_back(-1);
    heap[0].insert(approx(start, 0), 1);
    heap[1].insert(approx(end, 1), 0);
    heap[1].position.push_back(-1);
}

void Solver::findAnswer() {
    prepare();
    while (!timeToStop) {
        newStep(0);
        if (timeToStop) {
            break;
        }
        newStep(1);
    }
    stepAmount = (realDist[0][foundVertex] + realDist[1][foundVertex]);
}

void Solver::newStep(int side) {
    int number = heap[side].getMinOperation();
    char value = heap[side].getMin();
    uint64_t from = positions[number];
    heap[side].extractMin();
    used[side].insert(from);
    if (used[1 - side].count(from) || from == (side ? start : end)) {
        ++foundCount;
        char realVal = realDist[side][from] + realDist[1 - side][from];
        if (value == realVal || foundCount > kAccuracy || from == (side ? start : end)) {
            if (minVal > realVal) {
                foundVertex = from;
            }
            timeToStop = true;
            return;
        }
        if (minVal > realVal) {
            foundVertex = from;
            minVal = realVal;
        }
    }
    char emptyPos = getEmptyPos(from);
    int line = emptyPos / 4;
    int column = emptyPos % 4;
    if (line < 3) {
        addNewPos(from, side, (side ? 'D' : 'U'), emptyPos, 'D');
    }
    if (line > 0) {
        addNewPos(from, side, (side ? 'U' : 'D'), emptyPos,'U');
    }
    if (column < 3) {
        addNewPos(from, side, (side ? 'R' : 'L'), emptyPos,'R');
    }
    if (column > 0) {
        addNewPos(from, side, (side ? 'L' : 'R'), emptyPos, 'L');
    }
}

void Solver::createAnswer() {
    std::pair<uint64_t, char> nextStep = steps[0][foundVertex];
    while (nextStep.first != -1) {
        answer.push_back(nextStep.second);
        nextStep = steps[0][nextStep.first];
    }
    std::reverse(answer.begin(), answer.end());
    nextStep = steps[1][foundVertex];
    while (nextStep.first != -1) {
        answer.push_back(nextStep.second);
        nextStep = steps[1][nextStep.first];
    }
}

void Solver::read(int i, int j, uint64_t value) {
    forApprox[1][value] = std::make_pair(i, j);
    forApprox[0][(4 * i + j + 1) % kModule] = std::make_pair(i, j);
    forCheck[i][j] = static_cast<char>(value);
    start *= kModule;
    start += value;
    end *= kModule;
    end += ((4 * i + j + 1) % kModule);
}

Solver::Solver() : degree(kModule), forCheck(kSize, std::vector<char>(kSize)) {
    forApprox[0].resize(kModule);
    forApprox[1].resize(kModule);
    for (int i = kModule - 1; i >= 0; --i) {
        degree[i] = 4 * (kModule - i - 1);
    }
}

int main() {
    Solver solver;
    for (int i = 0; i < kSize; ++i) {
        for (int j = 0; j < kSize; ++j) {
            uint64_t value;
            std::cin >> value;
            solver.read(i, j, value);
        }
    }
    if (!solver.checkIsOk()) {
        std::cout << -1;
        return 0;
    }
    if (solver.isStartEqualsEnd()) {
        std::cout << 0;
        return 0;
    }
    solver.findAnswer();
    std::cout << solver.stepAmount << '\n';
    if (solver.stepAmount > 0) {
        solver.createAnswer();
        for (int i = 0; i < solver.stepAmount; ++i) {
            std::cout << solver.answer[i];
        }
    }
}