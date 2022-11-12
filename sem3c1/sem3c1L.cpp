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
    positions_.resize(size_);
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
    for (char letter : str_) {
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
      int clas = classes_[new_positions[i]];
      positions_[--cnt[clas]] = new_positions[i];
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
  void fix() { length = right_index - left_index; }
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
  void AddSuff(int start, int lcp) {
    if (last_path_.top() == kRoot) {
      vertices_.emplace_back(size_, vertices_[kRoot], start, str_.length());
      last_path_.push(size_);
      vertices_[kRoot].to[str_[start]] = size_;
      ++size_;
    } else {
      int lastErased = kNoErased;
      while (vertices_[last_path_.top()].depth > lcp) {
        lastErased = last_path_.top();
        last_path_.pop();
      }
      if (vertices_[last_path_.top()].depth != lcp) {
        int startIndex = vertices_[lastErased].edge.left_index;
        int middleIndex =
            startIndex + lcp - vertices_[vertices_[lastErased].prev].depth;
        vertices_.emplace_back(size_, vertices_[last_path_.top()], startIndex,
                               middleIndex);
        vertices_[last_path_.top()].to[str_[startIndex]] = size_;
        last_path_.push(size_);
        ++size_;
        vertices_[lastErased].edge.left_index = middleIndex;
        vertices_[lastErased].edge.fix();
        vertices_[lastErased].prev = last_path_.top();
        vertices_[last_path_.top()].to[str_[middleIndex]] = lastErased;
      }
      vertices_.emplace_back(size_, vertices_[last_path_.top()], start + lcp,
                             str_.length());
      vertices_[last_path_.top()].to[str_[start + lcp]] = size_;
      last_path_.push(size_);
      ++size_;
    }
  }
  void BuildSuffTree(std::vector<int>& lcp, std::vector<int>& suff_ar) {
    AddSuff(suff_ar[0], 0);
    for (int i = 1; i < static_cast<int>(str_.length()); ++i) {
      AddSuff(suff_ar[i], lcp[i - 1]);
    }
  }
  void DfsCout(int vertex, int prev, int& counter) {
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
        DfsCout(it->second, vertex, counter);
      }
    }
  }
  int Size() const { return size_; }

 private:
  const std::string& str_;
  std::vector<Node> vertices_;
  std::stack<int> last_path_;
  int size_ = 0;
};

void FindLCP(std::vector<int>& lcp, std::vector<int>& suff_ar,
             const std::string& str) {
  int length = static_cast<int>(str.length());
  std::vector<int> back(length);
  for (int i = 0; i < length; ++i) {
    back[suff_ar[i]] = i;
  }
  int cur_lcp = 0;
  for (int i = 0; i < length; ++i) {
    cur_lcp = std::max(cur_lcp - 1, 0);
    if (back[i] == length - 1) {
      continue;
    }
    int j = suff_ar[back[i] + 1];
    while (i + cur_lcp < length && j + cur_lcp < length &&
           str[i + cur_lcp] == str[j + cur_lcp]) {
      ++cur_lcp;
    }
    lcp[back[i]] = cur_lcp;
  }
}

int main() {
  std::string str;
  std::cin >> str;
  SuffArrayFinder finder;
  auto suffAr = finder.GetSuffArray(str);
  std::vector<int> lcp(str.length() - 1);
  FindLCP(lcp, suffAr, str);
  SuffTree suff_tree(str);
  suff_tree.BuildSuffTree(lcp, suffAr);
  std::cout << suff_tree.Size() << std::endl;
  int counter = 0;
  suff_tree.DfsCout(kRoot, kNoPrev, counter);
  return 0;
}