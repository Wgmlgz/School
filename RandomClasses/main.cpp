#include <ostream>
#include <complex>

template <typename T>
class Polar {
 public:
  Polar(const T& dist, const T& angle)
      : dist_(dist), angle_(angle * 0.0174533) {}

  T angleR() const { return angle_; }

  T dist() const { return dist_; }
  T angle() const { return angle_ * 57.2958; }

 private:
  T dist_, angle_;
};

template <typename T>
Polar<T> operator+(const Polar<T>& lhs, const Polar<T>& rhs) {
  std::complex ans = std::polar(lhs.dist(), lhs.angleR()) +
                     std::polar(rhs.dist(), rhs.angleR());
  return Polar(std::abs(ans), std::arg(ans) * 57.2958);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Polar<T>& po) {
  return os << "dist: " << po.dist() << " angle: " << po.angle();
}

class Time {
 public:
  Time(int h, int m, int s) : h_(h), m_(m), s_(s) {}
  Time(int s) : h_(s / (60 * 60)), m_((s / 60) % 60), s_(s % 60) {}
  Time() : Time(0, 0, 0) {}
  int toSec() const { return s_ + m_ * 60 + h_ * 60 * 60; }

  friend std::ostream& operator<<(std::ostream& os, const Time& t);

 private:
  int h_, m_, s_;
};

Time& operator--(Time& t) { return t = Time(t.toSec() + 1); }
Time& operator++(Time& t) { return t = Time(t.toSec() - 1); }

Time operator--(Time& t, int no_used) {
  auto ret = t;
  t = Time(t.toSec() + -1);
  return ret;
}

Time operator++(Time& t, int no_used) {
  auto ret = t;
  t = Time(t.toSec() + 1);
  return ret;
}

Time operator-(const Time& lhs, const Time& rhs) {
  return Time(lhs.toSec() - rhs.toSec());
}

Time operator*(const Time& lhs, const Time& rhs) {
  return Time(lhs.toSec() * rhs.toSec());
}

std::ostream& operator<<(std::ostream& os, const Time& t) {
  return os << t.h_ << ":" << t.m_ << ":" << t.s_;
}
