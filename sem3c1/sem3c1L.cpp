#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>

const int kAlphabetBegin = 36;
const int kAlphabetSize = 87;

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
        str = newStr;
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

const int kNoPrev = -1;
const int kNoErased = -1;
const int kRoot = 0;

class Edge {
public:
    int leftIndex;
    int rightIndex;
    int length;

    Edge() = default;
    Edge(int leftIndex, int rightIndex) : leftIndex(leftIndex), rightIndex(rightIndex),
        length(rightIndex - leftIndex) {};
    void fix() {
        length = rightIndex - leftIndex;
    }
};

const int kNoNumber = -1;

class Node {
public:
    int number = kRoot;
    int newNumber = kNoNumber;
    int prev = kNoPrev;
    Edge edge = Edge();
    int depth = 0;
    std::unordered_map<char, int> to;

    Node() = default;
    Node(int number, Node& prev, int edgeLeftIndex, int edgeRightIndex) : number(number), newNumber(kNoNumber),
        prev(prev.number), edge(edgeLeftIndex, edgeRightIndex), depth(prev.depth + edge.length) {}
};

class SuffTree {
public:
    const std::string& str;
    std::vector<Node> vertexes;
    std::stack<int> lastPath;
    int size = 0;

    explicit SuffTree(const std::string& str) : str(str) {
        vertexes.emplace_back();
        lastPath.push(kRoot);
        ++size;
    }
    void addSuff(int start, int lcp) {
        if (lastPath.top() == kRoot) {
            vertexes.emplace_back(size, vertexes[kRoot], start, str.length());
            lastPath.push(size);
            vertexes[kRoot].to[str[start]] = size;
            ++size;
        } else {
            int lastErased = kNoErased;
            while (vertexes[lastPath.top()].depth > lcp) {
                lastErased = lastPath.top();
                lastPath.pop();
            }
            if (vertexes[lastPath.top()].depth != lcp) {
                int startIndex = vertexes[lastErased].edge.leftIndex;
                int middleIndex = startIndex + lcp - vertexes[vertexes[lastErased].prev].depth;
                vertexes.emplace_back(size, vertexes[lastPath.top()], startIndex, middleIndex);
                vertexes[lastPath.top()].to[str[startIndex]] = size;
                lastPath.push(size);
                ++size;
                vertexes[lastErased].edge.leftIndex = middleIndex;
                vertexes[lastErased].edge.fix();
                vertexes[lastErased].prev = lastPath.top();
                vertexes[lastPath.top()].to[str[middleIndex]] = lastErased;
            }
            vertexes.emplace_back(size, vertexes[lastPath.top()], start + lcp, str.length());
            vertexes[lastPath.top()].to[str[start + lcp]] = size;
            lastPath.push(size);
            ++size;
        }
    }
    void buildSuffTree(std::vector<int>& lcp, std::vector<int>& suffAr) {
        addSuff(suffAr[0], 0);
        for (int i = 1; i < str.length(); ++i) {
            addSuff(suffAr[i], lcp[i - 1]);
        }
    }
};

void findLCP(std::vector<int>& lcp, std::vector<int>& suffAr, const std::string& str) {
    int length = static_cast<int>(str.length());
    std::vector<int> back(length);
    for (int i = 0; i < length; ++i) {
        back[suffAr[i]] = i;
    }
    int k = 0;
    for (int i = 0; i < length; ++i) {
        k = std::max(k - 1, 0);
        if (back[i] == length - 1) {
            continue;
        }
        int j = suffAr[back[i] + 1];
        while (i + k < length && j + k < length && str[i + k] == str[j + k]) {
            ++k;
        }
        lcp[back[i]] = k;
    }
}

void dfsCout(SuffTree& suffTree, int vertex, int prev, int& counter) {
    suffTree.vertexes[vertex].newNumber = counter;
    ++counter;
    if (prev != kNoPrev) {
        std::cout << suffTree.vertexes[prev].newNumber << " " << suffTree.vertexes[vertex].edge.leftIndex;
        std::cout << " " << suffTree.vertexes[vertex].edge.rightIndex << std::endl;
    }
    for (int i = kAlphabetBegin; i < kAlphabetBegin + kAlphabetSize; ++i) {
        auto it = suffTree.vertexes[vertex].to.find(static_cast<char>(i));
        if (it != suffTree.vertexes[vertex].to.end()) {
            dfsCout(suffTree, it->second, vertex, counter);
        }
    }
}

int main() {
    std::string str;
    std::cin >> str;
    SuffArrayFinder finder;
    auto suffAr = finder.getSuffArray(str);
    std::vector<int> lcp(str.length() - 1);
    findLCP(lcp, suffAr, str);
    SuffTree suffTree(str);
    suffTree.buildSuffTree(lcp, suffAr);
    std::cout << suffTree.size << std::endl;
    int counter = 0;
    dfsCout(suffTree, kRoot, kNoPrev, counter);
    return 0;
}