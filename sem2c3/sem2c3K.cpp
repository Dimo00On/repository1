#include <iostream>
#include <vector>
#include <queue>
#include <map>

struct Edge{
    int from;
    int to;
    std::vector<int> number;
    int part = -1;
    bool used = false;
    bool prove = false;
    bool woody = false;
    Edge* conjugate;

    Edge(int start, int end, int index) : from(start), to(end) {
        number.push_back(index);
    }
};

void simpleDfs(int now, int prev, std::vector<int>& tin, std::vector<int>& ret, int& time,
               std::vector<std::vector<Edge*>>& edges, std::vector<int>& color) {
    ++time;
    tin[now] = time;
    ret[now] = time;
    color[now] = 1;
    for (int i = 0; i < edges[now].size(); ++i) {
        int to = edges[now][i]->to;
        if (to == prev || to == now) {
            continue;
        }
        if (color[to] == 1) {
            ret[now] = std::min(ret[now], tin[to]);
        } else {
            edges[now][i]->woody = true;
            simpleDfs(to, now, tin, ret, time, edges, color);
            ret[now] = std::min(ret[now], ret[to]);
            if (ret[to] >= tin[now] && prev != -1) {
                edges[now][i]->prove = true;
                edges[now][i]->conjugate->prove = true;
            }
        }
    }
}

void dfs(int now, int prev,  int& count, std::vector<std::vector<Edge*>>& edges,
         std::vector<int>& color, std::queue<std::pair<Edge*, int>>& queue, int nowPart,
         std::vector<int>& tin) {
    color[now] = 1;
    for (int i = 0; i < edges[now].size(); ++i) {
        int to = edges[now][i]->to;
        if (to == prev || to == now) {
            continue;
        }
        if (edges[now][i]->prove) {
            queue.push(std::make_pair(edges[now][i], count));
            ++count;
        } else {
            if (edges[now][i]->part == -1 && (tin[now] > tin[edges[now][i]->to] || edges[now][i]->woody)) {
                edges[now][i]->part = nowPart;
                edges[now][i]->conjugate->part = nowPart;
            }
            if (color[to] == 0 && edges[now][i]->woody) {
                dfs(to, now, count, edges, color, queue, nowPart, tin);
            }
        }
    }
}

void findAnswer(std::vector<std::vector<int>>& answer, int vertexAmount, int edgesAmount,
                std::vector<Edge>& allEdges, std::vector<std::vector<Edge*>>& edges) {
    std::vector<int> color(vertexAmount);
    std::vector<int> tin(vertexAmount);
    std::vector<int> ret(vertexAmount);
    std::queue<std::pair<Edge*, int>> queue;
    int time = 0;
    int count = 0;
    for (int i = 0; i < vertexAmount; ++i) {
        if (color[i] == 0) {
            simpleDfs(i, -1, tin, ret, time, edges, color);
        }
    }
    for (int i = 0; i < vertexAmount; ++i) {
        color[i] = 0;
    }
    for (int i = 0; i < vertexAmount; ++i) {
        if (color[i] == 0) {
            for (auto e : edges[i]) {
                if (e->woody) {
                    queue.push(std::make_pair(e, count));
                    ++count;
                }
            }
            while (!queue.empty()) {
                Edge* edge = queue.front().first;
                int nowPart = queue.front().second;
                queue.pop();
                edge->part = nowPart;
                edge->conjugate->part = nowPart;
                dfs(edge->to, edge->from, count, edges, color, queue, nowPart, tin);
            }
        }
    }
    answer.resize(count);
    for (auto& e : allEdges) {
        if (!e.used) {
            for (int j = 0; j < e.number.size(); ++j) {
                answer[e.part].push_back(e.number[j]);
            }
            e.used = true;
            e.conjugate->used = true;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int vertexAmount, edgesAmount;
    std::cin >> vertexAmount >> edgesAmount;
    std::vector<std::vector<Edge*>> edges(vertexAmount);
    std::map<std::pair<int, int>, Edge*> unique;
    std::vector<Edge> allEdges;
    allEdges.reserve(2 * edgesAmount);
    for (int i = 0; i < edgesAmount; ++i) {
        int from, to;
        std::cin >> from >> to;
        if (unique.count(std::make_pair(from, to))) {
            auto ptr = unique[std::make_pair(from, to)];
            ptr->number.push_back(i);
            ptr->conjugate->number.push_back(i);
        } else {
            allEdges.emplace_back(from, to, i);
            edges[from].push_back(&allEdges[allEdges.size() - 1]);
            allEdges.emplace_back(to, from, i);
            edges[to].push_back(&allEdges[allEdges.size() - 1]);
            allEdges[allEdges.size() - 1].conjugate = &allEdges[allEdges.size() - 2];
            allEdges[allEdges.size() - 2].conjugate = &allEdges[allEdges.size() - 1];
            unique.insert(std::make_pair(std::make_pair(from, to), &allEdges[allEdges.size() - 2]));
            unique.insert(std::make_pair(std::make_pair(to, from), &allEdges[allEdges.size() - 1]));
        }
    }
    std::vector<std::vector<int>> answer;
    findAnswer(answer, vertexAmount, edgesAmount, allEdges, edges);
    std::cout << answer.size() << '\n';
    for (auto & ans : answer) {
        std::cout << ans.size() << " ";
        for (int j : ans) {
            std::cout << j << " ";
        }
        std::cout << '\n';
    }
}
