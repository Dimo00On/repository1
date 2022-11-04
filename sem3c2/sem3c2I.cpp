#include <algorithm>
#include <iostream>
#include <vector>

const int kMaxDegreePower = 20;
const long long kBaseFFTMod = 998244353;  // = 119 * 2^23 + 1;
const long long kBaseRoot = 31;
const long long kReversedDegreeBase = 499122177;

class IntegerFFT {
 public:
  explicit IntegerFFT(long long mod = kBaseFFTMod) : kModForValues(mod) {
    reversed_degree_.resize(kMaxDegreePower, 1);
    for (int i = 1; i < kMaxDegreePower; ++i) {
      reversed_degree_[i] =
          (reversed_degree_[i - 1] * kReversedDegreeBase) % kMod;
    }
    CreateRootTable();
  }
  std::vector<long long> Multiply(const std::vector<long long>& first,
                                  const std::vector<long long>& second) {
    Prepare(static_cast<int>(first.size() + second.size()) - 1, degree_,
            degree_power_);
    std::vector<long long> first_calculated =
        Transform(first, root_in_power_[degree_power_]);
    std::vector<long long> second_calculated =
        Transform(second, root_in_power_[degree_power_]);
    std::vector<long long> ans(degree_);
    for (int i = 0; i < degree_; ++i) {
      ans[i] = (first_calculated[i] * second_calculated[i]) % kMod;
    }
    ans = Transform(ans, rev_root_in_power[degree_power_]);
    for (int i = 0; i < degree_; ++i) {
      ans[i] *= reversed_degree_[degree_power_];
      ans[i] %= kMod;
      if (ans[i] > kMod / 2) {
        ans[i] -= kMod;
        ans[i] += kMod * kModForValues;
      }
      ans[i] %= kModForValues;
    }
    return ans;
  }

 protected:
  const long long kModForValues;

  void Prepare(int value, long long& degree, int& degree_power) {
    degree_power = 0;
    degree = (value == 0 ? 0 : 1);
    while (degree < value) {
      degree *= 2;
      ++degree_power;
    }
  }
  long long Pow(long long value, long long power, long long mod) const {
    value %= mod;
    long long temp = 1;
    if (power / 2 != 0) {
      temp = Pow(value * value, power / 2, mod);
    }
    if (power % 2 == 0) {
      return temp;
    }
    return (temp * value) % mod;
  }

 private:
  const long long kMod = kBaseFFTMod;
  long long degree_ = 1;
  int degree_power_ = 0;
  std::vector<long long> reversed_degree_;
  std::vector<std::vector<long long>> root_in_power_;
  std::vector<std::vector<long long>> rev_root_in_power;
  std::vector<std::vector<int>> rev_bits_;

  void CreateRootTable() {
    root_in_power_.resize(kMaxDegreePower);
    rev_bits_.resize(kMaxDegreePower);
    long long degree = 1;
    for (int cur_degree_power = 0; cur_degree_power < kMaxDegreePower;
         ++cur_degree_power) {
      long long root_degree = (kMod - 1) / degree;
      long long root = Pow(kBaseRoot, root_degree, kMod);
      root_in_power_[cur_degree_power].resize(degree + 1, 1);
      rev_bits_[cur_degree_power].resize(degree, -1);
      for (int i = 1; i < degree; ++i) {
        root_in_power_[cur_degree_power][i] =
            (root_in_power_[cur_degree_power][i - 1] * root) % kMod;
      }
      degree *= 2;
    }
    rev_root_in_power = root_in_power_;
    for (int i = 0; i < kMaxDegreePower; ++i) {
      std::reverse(rev_root_in_power[i].begin(), rev_root_in_power[i].end());
    }
  }
  int ReversedBits(int value) {
    if (rev_bits_[degree_power_][value] == -1) {
      int ans = 0;
      for (int i = 0; i < degree_power_; ++i) {
        ans += ((value >> i) % 2) << (degree_power_ - i - 1);
      }
      rev_bits_[degree_power_][value] = ans;
      return ans;
    }
    return rev_bits_[degree_power_][value];
  }
  void Precalculate(const std::vector<long long>& polynomial,
                    std::vector<long long>& calculated) {
    for (int i = 0; i < degree_; ++i) {
      int index = ReversedBits(i);
      if (index < static_cast<int>(polynomial.size())) {
        calculated[i] = polynomial[index];
        if (calculated[i] < 0) {
          calculated[i] += kMod;
        }
      }
    }
  }
  void ButterflyTransform(std::vector<long long>& calculated, int cur_degree,
                          int size,
                          const std::vector<long long>& root_in_power) {
    long long copy = calculated[cur_degree];
    long long shift = degree_ / 2 / size;
    calculated[cur_degree] += root_in_power[(cur_degree * shift) % degree_] *
                              calculated[cur_degree + size];
    calculated[cur_degree] %= kMod;
    calculated[cur_degree + size] *=
        root_in_power[((cur_degree + size) * shift) % degree_];
    calculated[cur_degree + size] += copy;
    calculated[cur_degree + size] %= kMod;
  }
  std::vector<long long> Transform(
      const std::vector<long long>& polynomial,
      const std::vector<long long>& root_in_power) {
    std::vector<long long> calculated(degree_, 0);
    Precalculate(polynomial, calculated);
    int size = 1;
    for (int cur_degree_power = 0; cur_degree_power < degree_power_;
         ++cur_degree_power) {
      for (int cur_degree = 0; cur_degree < degree_; ++cur_degree) {
        if ((cur_degree / size) % 2 == 1) {
          continue;
        }
        ButterflyTransform(calculated, cur_degree, size, root_in_power);
      }
      size *= 2;
    }
    return calculated;
  }
};

class RecurrentFinder : public IntegerFFT {
 public:
  RecurrentFinder(long long mod, int size, const long long first_coef,
                  const long long second_coef, const int first_shift,
                  const int second_shift)
      : IntegerFFT(mod),
        kSize(size),
        kFirstCoef((first_coef % kModForValues + kModForValues) %
                   kModForValues),
        kSecondCoef((second_coef % kModForValues + kModForValues) %
                    kModForValues),
        kFirstShift(first_shift),
        kSecondShift(second_shift){};
  long long find(long long number) {
    std::vector<long long> base_values(kSize, 0);
    Prepare(base_values);
    if (number < kSize) {
      return base_values[number];
    }
    std::vector<long long> polynomial = findPolynomial(number);
    long long ans = 0;
    for (int i = 0; i < kSize; ++i) {
      ans += polynomial[i] * base_values[i];
      ans %= kModForValues;
    }
    return ans;
  }

 private:
  const long long kBaseValue = 1;
  const int kSize;
  const long long kFirstCoef;
  const long long kSecondCoef;
  const int kFirstShift;
  const int kSecondShift;

  void Prepare(std::vector<long long>& base_values) {
    base_values[0] = kBaseValue;
    for (int i = 1; i < kSize; ++i) {
      if (i - kFirstShift < 0) {
        base_values[i] += kBaseValue * kFirstCoef;
      } else {
        base_values[i] += base_values[i - kFirstShift] * kFirstCoef;
      }
      if (i - kSecondShift < 0) {
        base_values[i] += kBaseValue * kSecondCoef;
      } else {
        base_values[i] += base_values[i - kSecondShift] * kSecondCoef;
      }
      base_values[i] %= kModForValues;
    }
  }
  std::vector<long long> findPolynomial(long long step) {
    if (step < kSize) {
      std::vector<long long> ans(kSize, 0);
      ans[step] = 1;
      return ans;
    }
    std::vector<long long> ans = findPolynomial(step / 2);
    std::vector<long long> square_ans = Multiply(ans, ans);
    ans = findRemainder(square_ans);
    if (step % 2 == 0) {
      return ans;
    }
    std::vector<long long> odd_ans(kSize, 0);
    for (int i = 0; i + 1 < kSize; ++i) {
      odd_ans[i + 1] = ans[i];
    }
    UncoverToSum(kSize, odd_ans, ans[kSize - 1]);
    return odd_ans;
  }
  std::vector<long long> findRemainder(std::vector<long long>& dividend) {
    for (int i = static_cast<int>(dividend.size()) - 1; i >= kSize; --i) {
      UncoverToSum(i, dividend, dividend[i]);
    }
    std::vector<long long> temp(kSize, 0);
    for (int i = 0; i < kSize; ++i) {
      temp[i] = dividend[i];
    }
    return temp;
  }
  void UncoverToSum(int index, std::vector<long long>& values, long long coef) {
    values[index - kFirstShift] += coef * kFirstCoef;
    values[index - kFirstShift] %= kModForValues;
    values[index - kSecondShift] += coef * kSecondCoef;
    values[index - kSecondShift] %= kModForValues;
  }
};

const int kStartSize = 2;
const long long kTaskMod = 119;

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  long long number;
  std::vector<long long> coef(kStartSize);
  std::vector<int> shift(kStartSize);
  std::cin >> number >> coef[0] >> coef[1] >> shift[0] >> shift[1];
  RecurrentFinder finder(kTaskMod, shift[1], coef[0], coef[1], shift[0],
                         shift[1]);
  std::cout << finder.find(number);
  return 0;
}