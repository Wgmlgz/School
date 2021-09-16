#pragma once
#include <exception>
#include <numeric>
#include <iostream>

class Fraction {
 public:
  explicit Fraction(int num, int den) : num_(num), den_(den) {
    if (!den) throw std::runtime_error("zero den");
    relax();
  }

  explicit Fraction(int num) : Fraction(num, 1) {};

  explicit Fraction() : Fraction(0, 1) {}

  Fraction(const Fraction& other) {*this = other;}
  ~Fraction() = default;

  int num() const { return num_; }
  int den() const { return den_; }

  friend std::ostream& operator<<(std::ostream& out, const Fraction& fract);

  friend void setSameDen(Fraction& lhs, Fraction& rhs);

  Fraction operator-() {
    num_ = -num_;
    return *this;
  }

  Fraction operator+() { return *this; }

  Fraction operator++() {
    num_ += den_;
    relax();
    return *this;
  }
  Fraction operator--() {
    num_ -= den_;
    relax();
    return *this;
  }

  Fraction operator++(int no_used) {
    auto ret = *this;
    num_ += den_;
    relax();
    return ret;
  }

  Fraction operator--(int no_used) {
    auto ret = *this;
    num_ -= den_;
    relax();
    return ret;
  }

  void relax() {
    int div = std::gcd(num_, den_);
    num_ /= div;
    den_ /= div;
  }

 private:
  int num_, den_;
};

std::ostream& operator<<(std::ostream& out, const Fraction& fract) {
  return out << fract.num() << "/" << fract.den();
}

void setSameDen(Fraction& lhs, Fraction& rhs) {
  int den = std::lcm(lhs.den_, rhs.den_);

  int m_lhs = den / lhs.den_;
  int m_rhs = den / rhs.den_;

  lhs.num_ *= m_lhs;
  rhs.num_ *= m_rhs;

  lhs.den_ = den;
  rhs.den_ = den;
}

bool operator<(const Fraction& lhs, const Fraction& rhs) {
  auto lhs_ = lhs, rhs_ = rhs;
  setSameDen(lhs_, rhs_);
  return lhs_.num() < rhs_.num();
}

bool operator<(const Fraction& lhs, const int& fuck_pcf) {
  auto lhs_ = lhs;
  auto rhs_ = Fraction(fuck_pcf);
  setSameDen(lhs_, rhs_);
  return lhs_.num() < rhs_.num();
}

bool operator<(const int& fuck_pcf, const Fraction& rhs) {
  auto lhs_ = Fraction(fuck_pcf);
  auto rhs_ = rhs;
  setSameDen(lhs_, rhs_);
  return lhs_.num() < rhs_.num();
}

bool operator> (const Fraction& lhs, const Fraction& rhs) { return rhs < lhs; }
bool operator>=(const Fraction& lhs, const Fraction& rhs) { return !(lhs < rhs); }
bool operator<=(const Fraction& lhs, const Fraction& rhs) { return !(rhs < lhs); }
bool operator==(const Fraction& lhs, const Fraction& rhs) { return !(lhs < rhs) and !(rhs < lhs); }
bool operator!=(const Fraction& lhs, const Fraction& rhs) { return lhs < rhs or rhs < lhs; }


bool operator> (const Fraction& lhs, const int& rhs) { return rhs < lhs; }
bool operator>=(const Fraction& lhs, const int& rhs) { return !(lhs < rhs); }
bool operator<=(const Fraction& lhs, const int& rhs) { return !(rhs < lhs); }
bool operator==(const Fraction& lhs, const int& rhs) { return !(lhs < rhs) and !(rhs < lhs); }
bool operator!=(const Fraction& lhs, const int& rhs) { return lhs < rhs or rhs < lhs; }

bool operator> (const int& lhs, const Fraction& rhs) { return rhs < lhs; }
bool operator>=(const int& lhs, const Fraction& rhs) { return !(lhs < rhs); }
bool operator<=(const int& lhs, const Fraction& rhs) { return !(rhs < lhs); }
bool operator==(const int& lhs, const Fraction& rhs) { return !(lhs < rhs) and !(rhs < lhs); }
bool operator!=(const int& lhs, const Fraction& rhs) { return lhs < rhs or rhs < lhs; }


Fraction operator+(const Fraction& lhs, const Fraction& rhs) {
  auto lhs_ = lhs, rhs_ = rhs;
  setSameDen(lhs_, rhs_);
  return Fraction(lhs_.num() + rhs_.num(), lhs_.den());
}

Fraction operator-(const Fraction& lhs, const Fraction& rhs) {
  auto lhs_ = lhs, rhs_ = rhs;
  setSameDen(lhs_, rhs_);
  return Fraction(lhs_.num() - rhs_.num(), lhs_.den());
}

Fraction operator*(const Fraction& lhs, const Fraction& rhs) {
  return Fraction(lhs.num() * rhs.num(), lhs.den() * rhs.den());
}

Fraction operator/(const Fraction& lhs, const Fraction& rhs) {
  return Fraction(lhs.num() * rhs.den(), lhs.den() * rhs.num());
}

Fraction operator+=(Fraction& lhs, const Fraction& rhs) {
  return lhs = lhs + rhs;
}

Fraction operator-=(Fraction& lhs, const Fraction& rhs) {
  return lhs = lhs - rhs;
}

Fraction operator*=(Fraction& lhs, const Fraction& rhs) {
  return lhs = lhs * rhs;
}

Fraction operator/=(Fraction& lhs, const Fraction& rhs) {
  return lhs = lhs / rhs;
}

Fraction operator+(const Fraction& lhs, const int& fuck_pcf) {
  auto lhs_ = lhs;
  auto rhs_ = Fraction(fuck_pcf);
  setSameDen(lhs_, rhs_);
  return Fraction(lhs_.num() + rhs_.num(), lhs_.den());
}

Fraction operator-(const Fraction& lhs, const int& fuck_pcf) {
  auto lhs_ = lhs;
  auto rhs_ = Fraction(fuck_pcf);
  setSameDen(lhs_, rhs_);
  return Fraction(lhs_.num() - rhs_.num(), lhs_.den());
}

Fraction operator*(const Fraction& lhs, const int& fuck_pcf) {
  auto rhs = Fraction(fuck_pcf);
  return Fraction(lhs.num() * rhs.num(), lhs.den() * rhs.den());
}

Fraction operator/(const Fraction& lhs, const int& fuck_pcf) {
  auto rhs = Fraction(fuck_pcf);
  return Fraction(lhs.num() * rhs.den(), lhs.den() * rhs.num());
}

Fraction operator+=(Fraction& lhs, const int& fuck_pcf) {
  auto rhs = Fraction(fuck_pcf);
  return lhs = lhs + rhs;
}

Fraction operator-=(Fraction& lhs, const int& fuck_pcf) {
  auto rhs = Fraction(fuck_pcf);
  return lhs = lhs - rhs;
}

Fraction operator*=(Fraction& lhs, const int& fuck_pcf) {
  auto rhs = Fraction(fuck_pcf);
  return lhs = lhs * rhs;
}

Fraction operator/=(Fraction& lhs, const int& fuck_pcf) {
  auto rhs = Fraction(fuck_pcf);
  return lhs = lhs / rhs;
}

Fraction operator+(const int& fuck_pcf, const Fraction& rhs) {
  auto lhs_ = Fraction(fuck_pcf);
  auto rhs_ = rhs;

  setSameDen(lhs_, rhs_);
  return Fraction(lhs_.num() + rhs_.num(), lhs_.den());
}

Fraction operator-(const int& fuck_pcf, const Fraction& rhs) {
  auto lhs_ = Fraction(fuck_pcf);
  auto rhs_ = rhs;

  setSameDen(lhs_, rhs_);
  return Fraction(lhs_.num() - rhs_.num(), lhs_.den());
}

Fraction operator*(const int& fuck_pcf, const Fraction& rhs) {
  auto lhs = Fraction(fuck_pcf);
  return Fraction(lhs.num() * rhs.num(), lhs.den() * rhs.den());
}

Fraction operator/(const int& fuck_pcf, const Fraction& rhs) {
  auto lhs = Fraction(fuck_pcf);
  return Fraction(lhs.num() * rhs.den(), lhs.den() * rhs.num());
}
