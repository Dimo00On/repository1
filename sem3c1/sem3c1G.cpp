#include <iostream>
#include <vector>

const int kAlphabetBegin = 35;  // 35 == '#'
const int kAlphabetSize = 88;   // 88 = 26 + (97 - 35); 97 == 'a'
const char kDelimiter = '#';

class SuffArrayFinder {
 public:
  std::vector<int> GetSuffArray(const std::string& new_str) {
    str_ = new_str + kDelimiter;
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
    return {positions_.begin() + 1, positions_.end()};
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

int main() {
  std::string str;
  std::cin >> str;
  SuffArrayFinder finder;
  auto answer = finder.GetSuffArray(str);
  for (const auto& ans : answer) {
    std::cout << ans + 1 << " ";
  }
  return 0;
}
