#include <algorithm>
#include <iostream>
#include <vector>

class IntegerFFT {
 public:  // 1 пробел ну и мерзость
  IntegerFFT() {
    reversed_degree_.resize(k_max_degree_power_, 1);
    // my reversed degree values collection
    reversed_degree_[0] = 1;
    reversed_degree_[1] = 499122177;
    reversed_degree_[2] = 748683265;
    reversed_degree_[3] = 873463809;
    reversed_degree_[4] = 935854081;
    reversed_degree_[5] = 967049217;
    reversed_degree_[6] = 982646785;
    reversed_degree_[7] = 990445569;
    reversed_degree_[8] = 994344961;
    reversed_degree_[9] = 996294657;
    reversed_degree_[10] = 997269505;
    reversed_degree_[11] = 997756929;
    reversed_degree_[12] = 998000641;
    reversed_degree_[13] = 998122497;
    reversed_degree_[14] = 998183425;
    reversed_degree_[15] = 998213889;
    reversed_degree_[16] = 998229121;
    reversed_degree_[17] = 998236737;
    reversed_degree_[18] = 998240545;
    reversed_degree_[19] = 998242449;
    reversed_degree_[20] = 998243401;
    reversed_degree_[21] = 998243877;
    reversed_degree_[22] = 998244115;
    reversed_degree_[23] = 998244234;
  }
  std::vector<long long> Multiply(const std::vector<long long>& first,
                                  const std::vector<long long>& second) {
    Prepare_(static_cast<int>(first.size() + second.size()) - 1);
    long long root_degree = (k_best_mod_ever_ - 1) / degree_;
    long long root = Pow_(k_best_root_ever_, root_degree);
    CreateRootTable_(root);
    std::vector<long long> first_calculated = Transform_(first);
    std::vector<long long> second_calculated = Transform_(second);
    std::vector<long long> ans(degree_);
    for (int i = 0; i < degree_; ++i) {
      ans[i] = (first_calculated[i] * second_calculated[i]) % k_best_mod_ever_;
    }
    std::reverse(root_in_power_.begin(), root_in_power_.end());
    ans = Transform_(ans);
    for (int i = 0; i < degree_; ++i) {
      ans[i] *= reversed_degree_[degree_power_];
      ans[i] %= k_best_mod_ever_;
      if (ans[i] > k_best_mod_ever_ / 2) {
        ans[i] -= k_best_mod_ever_;
      }
    }
    return ans;
  }

 private:
  const int k_max_degree_power_ = 24;
  const long long k_best_mod_ever_ = 998244353;  // = 119 * 2^23 + 1;
  const long long k_best_root_ever_ = 31;
  long long degree_ = 1;
  int degree_power_ = 0;
  std::vector<long long> reversed_degree_;
  std::vector<long long>
      root_in_power_;  // blank line divides variables and functions

  void Prepare_(int value) {
    degree_power_ = 0;
    degree_ = (value == 0 ? 0 : 1);
    while (degree_ < value) {
      degree_ *= 2;
      ++degree_power_;
    }
  }
  long long Pow_(long long value, long long power) {
    value %= k_best_mod_ever_;
    long long temp = 1;
    if (power / 2 != 0) {
      temp = Pow_(value * value, power / 2);
    }
    if (power % 2 == 0) {
      return temp;
    } else {
      return (temp * value) % k_best_mod_ever_;
    }
  }
  void CreateRootTable_(long long root) {
    root_in_power_.clear();
    root_in_power_.resize(degree_ + 1, 1);
    for (int i = 1; i < degree_; ++i) {
      root_in_power_[i] = (root_in_power_[i - 1] * root) % k_best_mod_ever_;
    }
  }
  int ReversedBits_(int value) {
    int ans = 0;
    for (int i = 0; i < degree_power_; ++i) {
      ans += ((value >> i) % 2) << (degree_power_ - i - 1);
    }
    return ans;
  }
  std::vector<long long> Transform_(const std::vector<long long>& polynomial) {
    int size = static_cast<int>(polynomial.size());
    std::vector<long long> calculated(degree_, 0);
    for (int i = 0; i < degree_; ++i) {
      int index = ReversedBits_(i);
      if (index < size) {
        calculated[i] = polynomial[index];
        if (calculated[i] < 0) {
          calculated[i] += k_best_mod_ever_;
        }
      }
    }
    size = 1;
    for (int i = 0; i < degree_power_; ++i) {
      for (int j = 0; j < degree_; ++j) {
        if ((j / size) % 2 == 1) {
          continue;
        }
        long long first = calculated[j];
        long long second = calculated[j + size];
        calculated[j] = first;
        calculated[j] +=
            root_in_power_[(j * (degree_ / 2 / size)) % degree_] * second;
        calculated[j] %= k_best_mod_ever_;
        calculated[j + size] = first;
        calculated[j + size] +=
            root_in_power_[((j + size) * (degree_ / 2 / size)) % degree_] *
            second;
        calculated[j + size] %= k_best_mod_ever_;
      }
      size *= 2;
    }
    return calculated;
  }
};

int main() {  // нет, 2 пробела вместо 4 отвратительно выглядит
  IntegerFFT fft;
  int degree[2];
  std::vector<long long> polynomial[2];
  std::cin >> degree[0];
  polynomial[0].resize(degree[0] + 1);
  for (int i = 0; i <= degree[0]; ++i) {
    std::cin >> polynomial[0][i];
  }
  std::reverse(polynomial[0].begin(), polynomial[0].end());
  std::cin >> degree[1];
  polynomial[1].resize(degree[1] + 1);
  for (int i = 0; i <= degree[1]; ++i) {
    std::cin >> polynomial[1][i];
  }
  std::reverse(polynomial[1].begin(), polynomial[1].end());
  std::vector<long long> ans = fft.Multiply(polynomial[0], polynomial[1]);
  std::reverse(ans.begin(), ans.end());
  bool start_zero = true;
  int ans_degree = static_cast<int>(ans.size() - 1);
  for (long long value : ans) {
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
  return 0;
}
