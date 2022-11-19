#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

const int kAlphabetBegin = 36;
const int kAlphabetSize = 87;

class SuffArrayFinder {
 public:
  std::vector<int> GetSuffArray(const std::string& new_str) {
    str_ = new_str;
    size_ = static_cast<int>(str_.length());
    // positions_.clear(); достаточно resize
    positions_.resize(size_);
    classes_.clear();
    classes_.resize(size_, 0);
    PreFindPositions();
    PreFindClasses();
    for (step_ = 0; (1 << step_) < size_; ++step_) {
      ReFindPositions();
      ReFindClasses();
    }
    return positions_;
  }

 private:
  int step_ = 0;
  int size_ = 0;
  std::string str_;
  std::vector<int> positions_;
  std::vector<int> classes_;

  void PreFindPositions() {
    std::vector<int> cnt(kAlphabetSize, 0);
    for (const char& letter : str_) {
      ++cnt[letter - kAlphabetBegin];
    }
    for (int i = 1; i < kAlphabetSize; ++i) {
      cnt[i] += cnt[i - 1];
    }
    for (int i = size_ - 1; i >= 0; --i) {
      positions_[--cnt[str_[i] - kAlphabetBegin]] = i;
    }
  }
  void PreFindClasses() {
    for (size_t i = 1; i < str_.length(); ++i) {
      classes_[positions_[i]] = classes_[positions_[i - 1]];
      if (str_[positions_[i]] != str_[positions_[i - 1]]) {
        ++classes_[positions_[i]];
      }
    }
  }
  void ReFindPositions() {
    std::vector<int> new_positions(size_);
    std::vector<int> cnt(size_, 0);
    for (int i = 0; i < size_; ++i) {
      new_positions[i] = (positions_[i] + size_ - (1 << step_)) % size_;
    }
    for (int i = 0; i < size_; ++i) {
      ++cnt[classes_[i]];
    }
    for (int i = 1; i < size_; ++i) {
      cnt[i] += cnt[i - 1];
    }
    for (int i = size_ - 1; i >= 0; --i) {
      int current_class = classes_[new_positions[i]];
      positions_[--cnt[current_class]] = new_positions[i];
    }
  }
  void ReFindClasses() {
    std::vector<int> new_classes(size_, 0);
    for (int i = 1; i < size_; ++i) {
      new_classes[positions_[i]] = new_classes[positions_[i - 1]];
      if (classes_[positions_[i]] != classes_[positions_[i - 1]] ||
          classes_[positions_[i] + (1 << step_)] !=
              classes_[positions_[i - 1] + (1 << step_)]) {
        ++new_classes[positions_[i]];
      }
    }
    std::swap(classes_, new_classes);
  }
};

const int kNoPrev = -1;
const int kNoErased = -1;
const int kRoot = 0;

struct Edge {
  int left_index;
  int right_index;
  int length;

  Edge() = default;
  Edge(int left_index, int right_index)
      : left_index(left_index),
        right_index(right_index),
        length(right_index - left_index){};
  void Fix() { length = right_index - left_index; }
};

const int kNoNumber = -1;

struct Node {
  int number = kRoot;
  int new_number = kNoNumber;
  int prev = kNoPrev;
  Edge edge = Edge();
  int depth = 0;
  std::unordered_map<char, int> to;

  Node() = default;
  Node(int number, Node& prev, int edge_left_index, int edge_right_index)
      : number(number),
        new_number(kNoNumber),
        prev(prev.number),
        edge(edge_left_index, edge_right_index),
        depth(prev.depth + edge.length) {}
};

class SuffTree {
 public:
  explicit SuffTree(const std::string& str) : str_(str) {
    vertices_.emplace_back();
    last_path_.push(kRoot);
    ++size_;
  }
  void AddSuffSimpleCase(int start) {
    vertices_.emplace_back(size_, vertices_[kRoot], start, str_.length());
    last_path_.push(size_);
    vertices_[kRoot].to[str_[start]] = size_;
    ++size_;
  }
  void AddSuffNotSimpleCase(int start, int lcp) {
    int last_erased = kNoErased;
    while (vertices_[last_path_.top()].depth > lcp) {
      last_erased = last_path_.top();
      last_path_.pop();
    }
    if (vertices_[last_path_.top()].depth != lcp) {
      int start_index = vertices_[last_erased].edge.left_index;
      int middle_index =
          start_index + lcp - vertices_[vertices_[last_erased].prev].depth;
      vertices_.emplace_back(size_, vertices_[last_path_.top()], start_index,
                             middle_index);
      vertices_[last_path_.top()].to[str_[start_index]] = size_;
      last_path_.push(size_);
      ++size_;
      vertices_[last_erased].edge.left_index = middle_index;
      vertices_[last_erased].edge.Fix();
      vertices_[last_erased].prev = last_path_.top();
      vertices_[last_path_.top()].to[str_[middle_index]] = last_erased;
    }
    vertices_.emplace_back(size_, vertices_[last_path_.top()], start + lcp,
                           str_.length());
    vertices_[last_path_.top()].to[str_[start + lcp]] = size_;
    last_path_.push(size_);
    ++size_;
  }
  void AddSuff(int start, int lcp) {
    if (last_path_.top() == kRoot) {
      AddSuffSimpleCase(start);
    } else {
      AddSuffNotSimpleCase(start, lcp);
    }
  }
  void BuildSuffTree(std::vector<int>& lcp, std::vector<int>& suffix_array) {
    AddSuff(suffix_array[0], 0);
    for (int i = 1; i < static_cast<int>(str_.length()); ++i) {
      AddSuff(suffix_array[i], lcp[i - 1]);
    }
  }
  void PrintTree(int vertex, int prev, int& counter) {
    vertices_[vertex].new_number = counter;
    ++counter;
    if (prev != kNoPrev) {
      std::cout << vertices_[prev].new_number << " "
                << vertices_[vertex].edge.left_index;
      std::cout << " " << vertices_[vertex].edge.right_index << std::endl;
    }
    for (int i = kAlphabetBegin; i < kAlphabetBegin + kAlphabetSize; ++i) {
      auto it = vertices_[vertex].to.find(static_cast<char>(i));
      if (it != vertices_[vertex].to.end()) {
        PrintTree(it->second, vertex, counter);
      }
    }
  }
  int GetSize() const { return size_; }

 private:
  const std::string& str_;
  std::vector<Node> vertices_;
  std::stack<int> last_path_;
  int size_ = 0;
};

void FindLCP(std::vector<int>& lcp, std::vector<int>& suffix_array,
             const std::string& str) {
  int length = static_cast<int>(str.length());
  std::vector<int> back(length);
  for (int i = 0; i < length; ++i) {
    back[suffix_array[i]] = i;
  }
  int cur_lcp = 0;
  for (int meaningful_index = 0; meaningful_index < length;
       ++meaningful_index) {
    cur_lcp = std::max(cur_lcp - 1, 0);
    if (back[meaningful_index] == length - 1) {
      continue;
    }
    int meaningful_jndex = suffix_array[back[meaningful_index] + 1];
    while (meaningful_index + cur_lcp < length &&
           meaningful_jndex + cur_lcp < length &&
           str[meaningful_index + cur_lcp] == str[meaningful_jndex + cur_lcp]) {
      ++cur_lcp;
    }
    lcp[back[meaningful_index]] = cur_lcp;
  }
}

int main() {
  std::string str;
  std::cin >> str;
  SuffArrayFinder finder;
  auto suffix_array = finder.GetSuffArray(str);
  std::vector<int> lcp(str.length() - 1);
  FindLCP(lcp, suffix_array, str);
  SuffTree suff_tree(str);
  suff_tree.BuildSuffTree(lcp, suffix_array);
  std::cout << suff_tree.GetSize() << std::endl;
  int counter = 0;
  suff_tree.PrintTree(kRoot, kNoPrev, counter);
  return 0;
}