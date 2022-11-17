#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

const int kAlphabetSize = 26;
const int kAlphabetStart = 97;
const int kRoot = 0;
const int kNoVertex = -1;

struct Node {
  std::vector<int> to;
  std::vector<int> word_number;

  Node() : to(kAlphabetSize, kNoVertex) {}
  ~Node() = default;
};

class Graph {
 public:
  Graph() { vertices_.emplace_back(); }
  ~Graph() = default;
  void Add(const std::string& str, int word_number) {
    int pos = 0;
    for (auto letter : str) {
      letter -= kAlphabetStart;
      if (vertices_[pos].to[letter] == kNoVertex) {
        vertices_[pos].to[letter] = static_cast<int>(vertices_.size());
        vertices_.emplace_back();
      }
      pos = vertices_[pos].to[letter];
    }
    vertices_[pos].word_number.push_back(word_number);
  }
  Node& operator[](int index) { return vertices_[index]; }
  int Size() { return static_cast<int>(vertices_.size()); }
  bool IsTerminal(int pos) { return !vertices_[pos].word_number.empty(); }

 private:
  std::vector<Node> vertices_;
};

class AhoCorasickAutomaton {
 public:
  explicit AhoCorasickAutomaton(std::vector<std::string>& dict) : dict_(dict) {
    AhoCorasick();
  }
  void FindOccurrences(const std::string& kText,
                       std::vector<std::vector<int>>& ans) {
    int pos = 0;
    for (int i = 0; i < static_cast<int>(kText.length()); ++i) {
      pos = go_[pos][kText[i] - kAlphabetStart];
      int temp = pos;
      while (temp != kNoVertex) {
        for (auto index : trie_[temp].word_number) {
          ans[index].push_back(i - static_cast<int>(dict_[index].length()) + 1);
        }
        temp = compressed_link_[temp];
      }
    }
  }

 private:
  std::vector<std::string>& dict_;
  Graph trie_;
  std::vector<int> link_;
  std::vector<std::vector<int>> go_;
  std::vector<int> compressed_link_;

  void AhoCorasick() {
    for (int i = 0; i < static_cast<int>(dict_.size()); ++i) {
      trie_.Add(dict_[i], i);
    }
    MakeLinks();
    MakeCompressedLinks();
  }
  void MakeLinks() {
    link_.resize(trie_.Size(), kRoot);
    go_.resize(trie_.Size(), std::vector<int>(kAlphabetSize, kNoVertex));
    for (int i = 0; i < kAlphabetSize; ++i) {
      go_[kRoot][i] =
          (trie_[kRoot].to[i] == kNoVertex ? kRoot : trie_[kRoot].to[i]);
    }
    std::queue<int> vertices;
    vertices.push(kRoot);
    while (!vertices.empty()) {
      int pos = vertices.front();
      vertices.pop();
      for (int i = 0; i < kAlphabetSize; ++i) {
        int to = trie_[pos].to[i];
        if (to == kNoVertex) {
          continue;
        }
        link_[to] = (pos == kRoot ? kRoot : go_[link_[pos]][i]);
        for (int j = 0; j < kAlphabetSize; ++j) {
          go_[to][j] = (trie_[to].to[j] == kNoVertex ? go_[link_[to]][j]
                                                     : trie_[to].to[j]);
        }
        vertices.push(to);
      }
    }
  }
  void MakeCompressedLinks() {
    compressed_link_.resize(trie_.Size(), kNoVertex);
    std::queue<int> vertices;
    vertices.push(kRoot);
    while (!vertices.empty()) {
      int pos = vertices.front();
      vertices.pop();
      compressed_link_[pos] =
          (trie_.IsTerminal(link_[pos]) ? link_[pos]
                                        : compressed_link_[link_[pos]]);
      for (int i = 0; i < kAlphabetSize; ++i) {
        if (trie_[pos].to[i] != kNoVertex) {
          vertices.push(trie_[pos].to[i]);
        }
      }
    }
  }
};

void PrintAnswer(std::vector<std::vector<int>>& ans, int dict_size) {
  for (int i = 0; i < dict_size; ++i) {
    std::cout << ans[i].size() << " ";
    std::sort(ans[i].begin(), ans[i].end());
    for (int j : ans[i]) {
      std::cout << j + 1 << " ";
    }
    std::cout << '\n';
  }
}

int main() {
  std::string text;
  std::cin >> text;
  int dict_size;
  std::cin >> dict_size;
  std::vector<std::string> dict(dict_size);
  for (int i = 0; i < dict_size; ++i) {
    std::cin >> dict[i];
  }
  std::vector<std::vector<int>> ans(dict_size);
  AhoCorasickAutomaton automaton(dict);
  automaton.FindOccurrences(text, ans);
  PrintAnswer(ans, dict_size);
  return 0;
}
