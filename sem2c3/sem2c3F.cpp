#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

bool comparator(const std::pair<int, int>& first, const std::pair<int, int>& second) {
    return first.second > second.second;
}

void dfs(int now, std::vector<std::vector<int>>& edges, std::vector<int>& color, int& time,
         std::vector<std::pair<int, int>>& tout) {
    color[now] = 1;
    for (int i = 0; i < edges[now].size(); ++i) {
        if (color[edges[now][i]] == 2 || color[edges[now][i]] == 1) {
            continue;
        }
        if (color[edges[now][i]] == 0) {
            dfs(edges[now][i], edges, color, time, tout);
        }
    }
    color[now] = 2;
    ++time;
    tout[now].second = time;
}

void dfsR(int now, std::vector<std::vector<int>>& edges, std::vector<int>& color, std::vector<int>& parts, int& count,
          std::vector<int>& back) {
    color[now] = 1;
    for (int i = 0; i < edges[now].size(); ++i) {
        if (color[edges[now][i]] == 0) {
            dfsR(edges[now][i], edges, color, parts, count, back);
        }
    }
    parts[now] = count;
    back[count] = now;
    color[now] = 2;
}

void condensation(std::vector<std::vector<int>>& edges, std::vector<std::vector<int>>& reversedEdges,
                  int vertexAmount, int& count, std::vector<std::vector<int>>& newEdges,
                  std::vector<std::vector<int>>& reversedNewEdges, std::vector<int>& back) {
    std::vector<std::pair<int, int>> tout(vertexAmount);
    for (int i = 0; i < vertexAmount; ++i) {
        tout[i].first = i;
        tout[i].second = 0;
    }
    std::vector<int> color(vertexAmount, 0);
    int time = 0;
    for (int i = 0; i < vertexAmount; ++i) {
        if (color[i] == 0) {
            dfs(i, edges, color, time, tout);
        }
    }
    std::sort(tout.begin(), tout.end(), comparator);
    for (int i = 0; i < vertexAmount; ++i) {
        color[i] = 0;
    }
    std::vector<int> parts(vertexAmount);
    for (int i = 0; i < vertexAmount; ++i) {
        if (color[tout[i].first] == 0) {
            back.push_back(-1);
            dfsR(tout[i].first, reversedEdges, color, parts, count, back);
            ++count;
        }
    }
    newEdges.resize(count);
    reversedNewEdges.resize(count);
    std::set<std::pair<int, int>> uniqueEdges;
    int newEdgesAmount = 0;
    for (int i = 0; i < vertexAmount; ++i) {
        for (int j = 0; j < edges[i].size(); ++j) {
            std::pair<int, int> newEdge = std::make_pair(parts[i], parts[edges[i][j]]);
            if (newEdge.first == newEdge.second) {
                continue;
            }
            if (!uniqueEdges.count(newEdge)) {
                uniqueEdges.insert(newEdge);
                newEdges[parts[i]].push_back(parts[edges[i][j]]);
                reversedNewEdges[parts[edges[i][j]]].push_back(parts[i]);
                ++newEdgesAmount;
            }
        }
    }
    edges.clear();
    reversedEdges.clear();
}

void dfsK(int from, int now, std::vector<std::vector<int>>& edges, std::vector<std::vector<int>>& reversedEdges,
          std::vector<int>& color, std::vector<std::pair<int, int>>& pairs, std::vector<int>& lonelyStarts,
          std::vector<bool>& isStart, std::vector<bool>& isEnd, bool& isFind) {
    color[now] = 1;
    for (int i : edges[now]) {
        if (color[i] == 1 || color[i] == 2) {
            continue;
        }
        dfsK(from, i, edges, reversedEdges, color, pairs, lonelyStarts, isStart, isEnd, isFind);
        if (isFind) {
            break;
        }
    }
    if (isEnd[now] && !isFind) {
        pairs.emplace_back(from, now);
        isFind = true;
    }
    if (from == now && !isFind) {
        lonelyStarts.push_back(from);
    }
    color[now] = 2;
}

void buildAnswer(int& answer, std::vector<std::pair<int, int>>& answerEdges, std::vector<std::pair<int, int>>& pairs,
                 std::vector<int>& lonelyStarts, std::vector<int>& lonelyEnds, std::vector<int>& allBoths,
                 std::vector<int>& back, int endAmount, int startAmount, int bothAmount,
                 std::vector<std::pair<int, int>>& badPairs) {
    if (endAmount == 0 && startAmount == 0) {
        if (bothAmount == 1) {
            answer = 0;
        } else {
            answer = bothAmount;
            for (int i = 0; i < answer - 1; ++i) {
                answerEdges.emplace_back(back[allBoths[i]], back[allBoths[i + 1]]);
            }
            answerEdges.emplace_back(back[allBoths[answer - 1]], back[allBoths[0]]);
        }
        return;
    }
    answer = bothAmount + std::max(startAmount, endAmount);
    for (int i = 0; i + 1 < pairs.size(); ++i) {
        answerEdges.emplace_back(back[pairs[i].second], back[pairs[i + 1].first]);
    }
    if (bothAmount == 0) {
        answerEdges.emplace_back(back[pairs[pairs.size() - 1].second], back[pairs[0].first]);
    } else {
        answerEdges.emplace_back(back[pairs[pairs.size() - 1].second], back[allBoths[0]]);
        answerEdges.emplace_back(back[allBoths[allBoths.size() - 1]], back[pairs[0].first]);
        for (int i = 0; i + 1 < allBoths.size(); ++i) {
            answerEdges.emplace_back(back[allBoths[i]], back[allBoths[i + 1]]);
        }
    }
    if (!badPairs.empty()) {
        for (int i = 0; i + 1 < badPairs.size(); ++i) {
            answerEdges.emplace_back(back[badPairs[i].second], back[badPairs[i + 1].first]);
        }
        answerEdges.emplace_back(back[badPairs[badPairs.size() - 1].second], back[badPairs[0].first]);
    }
    for (int lonelyStart : lonelyStarts) {
        answerEdges.emplace_back(back[pairs[0].second], back[lonelyStart]);
    }
    for (int lonelyEnd : lonelyEnds) {
        answerEdges.emplace_back(back[lonelyEnd], back[pairs[0].first]);
    }
}

void findAnswer(std::vector<std::vector<int>>& edges, std::vector<std::vector<int>>& reversedEdges,
                int vertexAmount, int& answer, std::vector<std::pair<int, int>>& answerEdges) {
    int count = 0;
    std::vector<std::vector<int>> newEdges;
    std::vector<std::vector<int>> reversedNewEdges;
    std::vector<int> back;
    condensation(edges, reversedEdges, vertexAmount, count, newEdges, reversedNewEdges, back);
    int startAmount = 0;
    int endAmount = 0;
    int bothAmount = 0;
    std::vector<bool> isStart(count, false);
    std::vector<bool> isEnd(count, false);
    std::vector<bool> both(count, false);
    std::vector<int> allBoths;
    for (int i = 0; i < count; ++i) {
        if (newEdges[i].empty()) {
            if (reversedNewEdges[i].empty()) {
                both[i] = true;
                allBoths.push_back(i);
                ++bothAmount;
            } else {
                isEnd[i] = true;
                ++endAmount;
            }
        } else {
            if (reversedNewEdges[i].empty()) {
                isStart[i] = true;
                ++startAmount;
            }
        }
    }
    std::vector<std::pair<int, int>> pairs;
    std::vector<int> lonelyStarts;
    std::vector<int> lonelyEnds;
    std::vector<int> color(count, 0);
    for (int i = 0; i < count; ++i) {
        if (color[i] == 0 && isStart[i]) {
            bool isFind = false;
            dfsK(i, i, newEdges, reversedNewEdges, color, pairs, lonelyStarts, isStart, isEnd, isFind);
        }
    }
    for (int i = 0; i < count; ++i) {
        if (color[i] == 0 && isEnd[i]) {
            lonelyEnds.push_back(i);
        }
    }
    std::vector<std::pair<int, int>> badPairs;
    for (int i = static_cast<int>(lonelyEnds.size()) - 1, j = static_cast<int>(lonelyStarts.size()) - 1;
         std::min(i, j) >= 0; --i, --j) {
        badPairs.emplace_back(lonelyStarts[j], lonelyEnds[i]);
        lonelyEnds.pop_back();
        lonelyStarts.pop_back();
    }
    buildAnswer(answer, answerEdges, pairs, lonelyStarts, lonelyEnds,
                allBoths, back, endAmount, startAmount, bothAmount, badPairs);
}

int main() {
    int vertexAmount, edgesAmount;
    std::cin >> vertexAmount >> edgesAmount;
    std::vector<std::vector<int>> edges(vertexAmount);
    std::vector<std::vector<int>> reversedEdges(vertexAmount);
    std::set<std::pair<int, int>> uniqueEdges;
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to;
        std::cin >> from >> to;
        if (from == to) {
            continue;
        }
        std::pair<int, int> newEdge = std::make_pair(from, to);
        if (!uniqueEdges.count(newEdge)) {
            uniqueEdges.insert(newEdge);
            edges[from - 1].push_back(to - 1);
            reversedEdges[to - 1].push_back(from - 1);
        }
    }
    uniqueEdges.clear();
    int answer = 0;
    std::vector<std::pair<int, int>> answerEdges;
    findAnswer(edges, reversedEdges, vertexAmount, answer, answerEdges);
    std::cout << answer << '\n';
    for (int i = 0; i < answer; ++i) {
        std::cout << answerEdges[i].first + 1 << " " << answerEdges[i].second + 1 << '\n';
    }
}