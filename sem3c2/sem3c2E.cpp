#include <algorithm>
#include <iostream>
#include <vector>

class IntegerFFT {
 public:
  IntegerFFT() {
    reversed_degree_.resize(kMaxDegreePower, 1);
    for (int i = 1; i < kMaxDegreePower; ++i) {
      reversed_degree_[i] =
          (reversed_degree_[i - 1] * kReversedDegreeBase) % kMod;
    }
  }
  std::vector<long long> Multiply(const std::vector<long long>& first,
                                  const std::vector<long long>& second) {
    Prepare(static_cast<int>(first.size() + second.size()) - 1);
    long long root_degree = (kMod - 1) / degree_;
    long long root = Pow(kBaseRoot, root_degree);
    CreateRootTable(root);
    std::vector<long long> first_calculated = Transform(first);
    std::vector<long long> second_calculated = Transform(second);
    std::vector<long long> ans(degree_);
    for (int i = 0; i < degree_; ++i) {
      ans[i] = (first_calculated[i] * second_calculated[i]) % kMod;
    }
    std::reverse(root_in_power_.begin(), root_in_power_.end());
    ans = Transform(ans);
    for (int i = 0; i < degree_; ++i) {
      ans[i] *= reversed_degree_[degree_power_];
      ans[i] %= kMod;
      if (ans[i] > kMod / 2) {
        ans[i] -= kMod;
      }
    }
    return ans;
  }

 private:
  const int kMaxDegreePower = 24;
  const long long kMod = 998244353;  // = 119 * 2^23 + 1;
  const long long kBaseRoot = 31;
  const long long kReversedDegreeBase = 499122177;
  long long degree_ = 1;
  int degree_power_ = 0;
  std::vector<long long> reversed_degree_;
  std::vector<long long> root_in_power_;

  void Prepare(int value) {
    degree_power_ = 0;
    degree_ = (value == 0 ? 0 : 1);
    while (degree_ < value) {
      degree_ *= 2;
      ++degree_power_;
    }
  }
  long long Pow(long long value, long long power) {
    value %= kMod;
    long long temp = 1;
    if (power / 2 != 0) {
      temp = Pow(value * value, power / 2);
    }
    if (power % 2 == 0) {
      return temp;
    }
    return (temp * value) % kMod;
  }
  void CreateRootTable(long long root) {
    root_in_power_.clear();
    root_in_power_.resize(degree_ + 1, 1);
    for (int i = 1; i < degree_; ++i) {
      root_in_power_[i] = (root_in_power_[i - 1] * root) % kMod;
    }
  }
  int ReversedBits(int value) {
    int ans = 0;
    for (int i = 0; i < degree_power_; ++i) {
      ans += ((value >> i) % 2) << (degree_power_ - i - 1);
    }
    return ans;
  }
  std::vector<long long> Transform(const std::vector<long long>& polynomial) {
    int size = static_cast<int>(polynomial.size());
    std::vector<long long> calculated(degree_, 0);
    for (int i = 0; i < degree_; ++i) {
      int index = ReversedBits(i);
      if (index < size) {
        calculated[i] = polynomial[index];
        if (calculated[i] < 0) {
          calculated[i] += kMod;
        }
      }
    }
    size = 1;
    for (int cur_degree_power = 0; cur_degree_power < degree_power_;
         ++cur_degree_power) {
      for (int cur_degree = 0; cur_degree < degree_; ++cur_degree) {
        if ((cur_degree / size) % 2 == 1) {
          continue;
        }
        long long first = calculated[cur_degree];
        long long second = calculated[cur_degree + size];
        long long shift = degree_ / 2 / size;
        calculated[cur_degree] = first;
        calculated[cur_degree] +=
            root_in_power_[(cur_degree * shift) % degree_] * second;
        calculated[cur_degree] %= kMod;
        calculated[cur_degree + size] = first;
        calculated[cur_degree + size] +=
            root_in_power_[((cur_degree + size) * shift) % degree_] * second;
        calculated[cur_degree + size] %= kMod;
      }
      size *= 2;
    }
    return calculated;
  }
};

const int kStartSize = 2;

void ReadPolynomial(std::vector<long long>& polynomial, int& degree) {
  std::cin >> degree;
  polynomial.resize(degree + 1);
  for (int i = 0; i <= degree; ++i) {
    std::cin >> polynomial[i];
  }
  std::reverse(polynomial.begin(), polynomial.end());
}

void WritePolynomial(const std::vector<long long>& polynomial) {
  bool start_zero = true;
  int ans_degree = static_cast<int>(polynomial.size() - 1);
  for (long long value : polynomial) {
    if (start_zero) {
      if (value == 0) {
        --ans_degree;
        continue;
      }
      std::cout << ans_degree << " ";
      start_zero = false;
    }
    std::cout << value << " ";
  }
}

int main() {
  IntegerFFT fft;
  int degree[kStartSize];
  std::vector<long long> polynomial[kStartSize];
  ReadPolynomial(polynomial[0], degree[0]);
  ReadPolynomial(polynomial[1], degree[1]);
  std::cin >> degree[1];
  std::vector<long long> ans = fft.Multiply(polynomial[0], polynomial[1]);
  std::reverse(ans.begin(), ans.end());
  WritePolynomial(ans);
  return 0;
}
