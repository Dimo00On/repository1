#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

const double kEps = 1e-8;
const int kPrecision = 10;

template <typename T, typename Q>
bool IsLess(const T& first, const Q& second) {
  return first + kEps < second;
}
template <typename T, typename Q>
bool IsEqual(const T& first, const Q& second) {
  return (first - kEps < second && first + kEps > second);
}
template <typename T, typename Q>
bool IsGreater(const T& first, const Q& second) {
  return first - kEps > second;
}

template <typename T = double>
class Point;

template <typename T>
T ScalarMultiply(const Point<T>& first, const Point<T>& second);
template <typename T>
T VectorMultiply(const Point<T>& first, const Point<T>& second);

const double kNoCalculated = -1;
const double kInfinity = 1e9 + 47;

template <typename T>
class Point {
 public:
  Point() : x_(0), y_(0) {};
  Point(const T& x, const T& y) : x_(x), y_(y) {};
  Point(const Point& other) : x_(other.x_), y_(other.y_) {};
  Point(Point&& other) noexcept : x_(other.x_), y_(other.y_) {};
  Point(std::initializer_list<T> list) :
      x_(*list.begin()), y_(*(list.begin() + 1)) {};
  Point(const Point& start, const Point& end) :
      x_(end.x_ - start.x_), y_(end.y_ - start.y_) {};
  explicit operator Point<double>() { return Point<double>(x_, y_); }
  Point& operator=(const Point& other) {
    x_ = other.x_;
    y_ = other.y_;
    return *this;
  }
  Point& operator=(Point&& other) noexcept {
    x_ = other.x_;
    y_ = other.y_;
    return *this;
  }
  Point& operator=(std::initializer_list<T> list) {
    x_ = *(list.begin());
    y_ = *(list.begin() + 1);
    return *this;
  }
  bool operator==(const Point& other) const {
    return (IsEqual(x_, other.x_) && IsEqual(y_, other.y_));
  }
  bool operator<(const Point& other) const {
    return IsLess(x_, other.x_) ||
           (IsEqual(x_, other.x_) && IsLess(y_, other.y_));
  }
  bool operator<=(const Point& other) const {
    return IsLess(x_, other.x_) ||
           (IsEqual(x_, other.x_) && !IsGreater(y_, other.y_));
  }
  bool operator>(const Point& other) const { return (other < *this); }
  bool operator>=(const Point& other) const { return !(*this < other); }
  bool operator!=(const Point& other) const { return !(*this == other); }
  Point& operator+=(const Point& other) {
    x_ += other.x;
    y_ += other.y;
    length_ = kNoCalculated;
    return *this;
  }
  Point& operator-=(const Point& other) {
    x_ -= other.x_;
    y_ -= other.y_;
    length_ = kNoCalculated;
    return *this;
  }
  Point operator+(const Point& other) const {
    Point new_point(*this);
    new_point += other;
    return new_point;
  }
  Point operator-(const Point& other) const {
    Point new_point(*this);
    new_point -= other;
    return new_point;
  }
  Point& operator*=(const T& value) {
    x_ *= value;
    y_ *= value;
    length_ = kNoCalculated;
    return *this;
  }
  Point operator*(const T& value) const {
    return Point(x_ * value, y_ * value);
  }
  Point& operator/=(const T& value) {
    x_ /= value;  // аккуратнее с int point
    y_ /= value;
    length_ = kNoCalculated;
    return *this;
  }
  Point operator/(const T& value) const {
    return Point(x_ / value, y_ / value);
  }
  double operator/(const Point& other) {
    if (IsEqual(other.x_, 0)) {
      if (IsEqual(x_, 0)) {
        if (IsEqual(other.y_, 0)) {
          if (IsEqual(y_, 0)) {
            return 1;
          }
          return kInfinity;
        }
        return static_cast<double>(y_) / other.y_;
      }
      return kInfinity;
    }
    return static_cast<double>(x_) / other.x_;
  }
  double Length() const {
    if (length_ == kNoCalculated) {
      length_ = std::sqrt(x_ * x_ + y_ * y_);
    }
    return length_;
  }
  Point GetRightNormal() const { return Point(y_, -x_); }
  Point GetLeftNormal() const { return Point(-y_, x_); }
  T GetX() const { return x_; }
  T GetY() const { return y_; }
  T& AccessX() { return x_; }
  T& AccessY() { return y_; }
  bool IsCollinearTo(const Point& other) const {
    return IsEqual(VectorMultiply(*this, other), 0);
  }
  bool IsPerpendicularTo(const Point& other) const {
    return IsEqual(ScalarMultiply(*this, other), 0);
  }
  void Normalize() {
    Length();
    x_ /= length_;
    y_ /= length_;
    length_ = 1;
  }

 private:
  T x_;
  T y_;
  mutable double length_ = kNoCalculated;
};

const Point<double> kInfinityPoint(kInfinity, kInfinity);

template <typename T>
Point<T> operator*(const T& value, const Point<T>& point) {
  return Point<T>(point.GetX() * value, point.GetY() * value);
}
template <typename T>
T ScalarMultiply(const Point<T>& first, const Point<T>& second) {
  return first.GetX() * second.GetX() + first.GetY() * second.GetY();
}
template <typename T>
T VectorMultiply(const Point<T>& first, const Point<T>& second) {
  return first.GetX() * second.GetY() - first.GetY() * second.GetX();
}
template <typename T>
std::ostream& operator<<(std::ostream& out, const Point<T>& point) {
  out << std::setprecision(kPrecision)
      << (IsEqual(point.GetX(), 0) ? 0 : point.GetX()) << " "
      << (IsEqual(point.GetY(), 0) ? 0 : point.GetY());
  return out;
}
template <typename T>
std::istream& operator>>(std::istream& in, Point<T>& point) {
  in >> point.AccessX() >> point.AccessY();
  return in;
}

class Line;

double Distance(const Line& first, const Line& second);
double Distance(const Point<double>& point, const Line& line);
Point<double> GetPointOnLine(double a, double b,
                             double c) {  // line a * x + b * y + c = 0
  bool is_zero = IsEqual(b, 0);
  return {(is_zero ? -c / a : 0), (is_zero ? 0 : -c / b)};
}

enum class VectorType { Direction, Normal };
class Line {
 public:
  // (normal_, direction_) - положительная
  // normal_ в сторону, где > 0
  Line(const double& a, const double& b, const double& c) :
      direction_(-b, a), normal_(a, b), point_(GetPointOnLine(a, b, c)) {};
  Line(std::initializer_list<double> list) :
      Line(*(list.begin()), *(list.begin() + 1), *(list.begin() + 2)) {};
  Line(VectorType type, const Point<double>& vector, const Point<double>& point) :
      point_(point) {
    if (type == VectorType::Direction) {
      direction_ = vector;
      normal_ = direction_.GetRightNormal();
    }
    if (type == VectorType::Normal) {
      normal_ = vector;
      direction_ = normal_.GetLeftNormal();
    }
  }
  Line(const Point<double>& first_point, const Point<double>& second_point) :
      Line(VectorType::Direction, second_point - first_point, first_point) {};
  void Normalize() {
    direction_.Normalize();
    normal_.Normalize();
  };
  Point<double> Cross(const Line& other) const {
    Point temp(-ScalarMultiply(normal_, point_),
               -ScalarMultiply(other.normal_, other.point_));
    Point temp_x(normal_.GetX(), other.normal_.GetX());
    Point temp_y(normal_.GetY(), other.normal_.GetY());
    double divider = VectorMultiply(normal_, other.normal_);
    if (IsEqual(divider, 0)) {
      if (IsEqual(VectorMultiply(point_ - other.point_, direction_), 0)) {
        return point_;
      }
      return kInfinityPoint;
    }
    double y = VectorMultiply(temp, temp_x) / divider;
    double x = -VectorMultiply(temp, temp_y) / divider;
    return {x, y};
  }
  const Point<double>& GetDirection() const { return direction_; }
  const Point<double>& GetNormal() const { return normal_; }
  const Point<double>& GetPoint() const { return point_; }

 private:
  Point<double> direction_;
  Point<double> normal_;
  Point<double> point_;
};

double Distance(const Line& first, const Line& second) {
  if (!first.GetDirection().IsCollinearTo(second.GetDirection())) {
    return 0;
  }
  return Distance(first.GetPoint(), second);
}
double Distance(const Point<double>& point, const Line& line) {
  return std::abs(ScalarMultiply(line.GetNormal(), point - line.GetPoint())) /
         line.GetNormal().Length();
}
template <typename T>
double Distance(const Point<T>& first, const Point<T>& second) {
  return Point(first, second).Length();
}

class SegmentsCrossChecker {
 public:
  bool answer = false;
  Point<double> cross_point = kInfinityPoint;

  SegmentsCrossChecker(
      const Point<double>& first_begin,
      const Point<double>& first_end,
      const Point<double>& second_begin,
      const Point<double>& second_end) :
      first_begin_(first_begin),
      first_end_(first_end),
      second_begin_(second_begin),
      second_end_(second_end),
      first_vector_(first_begin, first_end),
      second_vector_(second_begin, second_end),
      temp_vector_(first_begin, second_begin) {
    Check();
  };

 private:
  const Point<double>& first_begin_;
  const Point<double>& first_end_;
  const Point<double>& second_begin_;
  const Point<double>& second_end_;
  Point<double> first_vector_;
  Point<double> second_vector_;
  Point<double> temp_vector_;

  void Check() {
    if (first_begin_ == first_end_) {
      if (second_begin_ == second_end_) {
        if (first_begin_ == second_begin_) {
          answer = true;
          cross_point = first_begin_;
          return;
        }
        return;
      }
      IsPointOnSegment(first_begin_, second_begin_, second_end_);
      return;
    }
    if (second_begin_ == second_end_) {
      IsPointOnSegment(second_begin_, first_begin_, first_end_);
      return;
    }
    if (temp_vector_.IsCollinearTo(first_vector_)) {
      SegmentsCrossParallel();
      return;
    }
    SegmentsCrossNotParallel();
  }
  void IsPointOnSegment(const Point<double>& point, const Point<double>& begin,
                        const Point<double>& end) {
    temp_vector_ = Point<double>(begin, point);
    Point<double> vector(begin, end);
    if (temp_vector_.IsCollinearTo(vector)) {
      double coef = temp_vector_ / vector;
      if (!IsGreater(coef, 1) && !IsLess(coef, 0)) {
        answer = true;
        cross_point = point;
      }
    }
  }
  void SegmentsCrossParallel() {
    answer = true;
    double first_coef = temp_vector_ / first_vector_;
    if (!IsLess(first_coef, 0) && !IsGreater(first_coef, 1)) {
      cross_point = second_begin_;
      return;
    }
    temp_vector_ = Point<double>(first_begin_, second_end_);
    double second_coef = temp_vector_ / first_vector_;
    if (IsGreater(first_coef, 1) && !IsGreater(second_coef, 1)) {
      cross_point = first_end_;
      return;
    }
    if (IsLess(first_coef, 0) && !IsLess(second_coef, 0)) {
      cross_point = first_begin_;
      return;
    }
    answer = false;
  }
  void SegmentsCrossNotParallel() {
    Line first_line(VectorType::Direction, first_vector_, first_begin_);
    Line second_line(VectorType::Direction, second_vector_, second_begin_);
    cross_point = first_line.Cross(second_line);
    Point<double> first_cross_vector(first_begin_, cross_point);
    Point<double> second_cross_vector(second_begin_, cross_point);
    double first_coef = first_cross_vector / first_vector_;
    double second_coef = second_cross_vector / second_vector_;
    if (!IsLess(first_coef, 0) && !IsGreater(first_coef, 1) &&
        !IsLess(second_coef, 0) && !IsGreater(second_coef, 1)) {
      answer = true;
    }
  }
};

int main() {
  Point first_begin;
  Point first_end;
  Point second_begin;
  Point second_end;
  std::cin >> first_begin >> first_end;
  std::cin >> second_begin >> second_end;
  SegmentsCrossChecker checker(first_begin, first_end, second_begin,
                               second_end);
  if (checker.answer) {
    std::cout << "YES";
  } else {
    std::cout << "NO";
  }
  return 0;
}