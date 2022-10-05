#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

const int kAlphabetSize = 26;
const int kAlphabetStart = 97;

struct Node {
    std::vector<int> to;
    std::vector<int> wordNumber;

    Node() : to(kAlphabetSize, -1) {}
    ~Node() = default;
};

class Graph {
    std::vector<Node> vertexes;

public:
    Graph() {
        vertexes.emplace_back();
    }
    ~Graph() = default;
    void add(const std::string& str, int wordNumber) {
        int pos = 0;
        for (auto i : str) {
            i -= kAlphabetStart;
            if (vertexes[pos].to[i] == -1) {
                vertexes[pos].to[i] = static_cast<int>(vertexes.size());
                vertexes.emplace_back();
            }
            pos = vertexes[pos].to[i];
        }
        vertexes[pos].wordNumber.push_back(wordNumber);
    }
    Node& operator[](int index) {
        return vertexes[index];
    }
    int size() {
        return static_cast<int>(vertexes.size());
    }
    bool isTerminal(int pos) {
        return !vertexes[pos].wordNumber.empty();
    }
};

class Solver {
    std::string& text;
    std::vector<std::string>& dict;
    std::vector<std::vector<int>>& ans;
    Graph bor;
    std::vector<int> link;
    std::vector<std::vector<int>> go;
    std::vector<int> compressedLink;

    void makeLinks() {
        link.resize(bor.size());
        go.resize(bor.size(), std::vector<int>(kAlphabetSize, -1));
        //link[0] = 0;
        for (int i = 0; i < kAlphabetSize; ++i) {
            go[0][i] = (bor[0].to[i] == -1 ? 0 : bor[0].to[i]);
        }
        std::queue<int> vertexes;
        vertexes.push(0);
        while (!vertexes.empty()) {
            int pos = vertexes.front();
            vertexes.pop();
            for (int i = 0; i < kAlphabetSize; ++i) {
                int to = bor[pos].to[i];
                if (to == -1) {
                    continue;
                }
                link[to] = (pos == 0 ? 0 : go[link[pos]][i]);
                for (int j = 0; j < kAlphabetSize; ++j) {
                    go[to][j] = (bor[to].to[j] == -1 ? go[link[to]][j] : bor[to].to[j]);
                }
                vertexes.push(to);
            }
        }
    }
    void makeCompressedLinks() {
        compressedLink.resize(bor.size(), -1);
        std::queue<int> vertexes;
        vertexes.push(0);
        while (!vertexes.empty()) {
            int pos = vertexes.front();
            vertexes.pop();
            compressedLink[pos] = (bor.isTerminal(link[pos]) ? link[pos] : compressedLink[link[pos]]);
            for (int i = 0; i < kAlphabetSize; ++i) {
                if (bor[pos].to[i] != -1) {
                    vertexes.push(bor[pos].to[i]);
                }
            }
        }
    }

public:
    Solver(std::string& text, std::vector<std::string>& dict,
           std::vector<std::vector<int>>& ans) :
           text(text), dict(dict), ans(ans) {}
    void pleaseSolveThisTask() {
        for (int i = 0; i < dict.size(); ++i) {
            bor.add(dict[i], i);
        }
        makeLinks();
        makeCompressedLinks();
        int pos = 0;
        for (int i = 0; i < text.length(); ++i) {
            pos = go[pos][text[i] - kAlphabetStart];
            int temp = pos;
            while (temp != -1) {
                for (auto index : bor[temp].wordNumber) {
                    ans[index].push_back(i - static_cast<int>(dict[index].length()) + 1);
                }
                temp = compressedLink[temp];
            }
        }
    }
};

int main() {
    std::string text;
    std::cin >> text;
    int dictSize;
    std::cin >> dictSize;
    std::vector<std::string> dict(dictSize);
    for (int i = 0; i < dictSize; ++i) {
        std::cin >> dict[i];
    }
    std::vector<std::vector<int>> ans(dictSize);
    Solver solver(text, dict, ans);
    solver.pleaseSolveThisTask();
    for (int i = 0; i < dictSize; ++i) {
        std::cout << ans[i].size() << " ";
        std::sort(ans[i].begin(), ans[i].end());
        for (int j : ans[i]) {
            std::cout << j + 1 << " ";
        }
        std::cout << '\n';
    }
    return 0;
}
