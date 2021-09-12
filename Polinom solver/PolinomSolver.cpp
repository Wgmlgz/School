#include <bits/stdc++.h>
#include <vector>

struct Polinom {
  const long double EPS = 1e-7;
  const long double smolEPS = 1e-4;
  std::vector<long double> coefs;

  explicit Polinom(int size);

  explicit Polinom(std::vector<long double> _coefs);

  long double eval(const long double time);

  long double operator()(const long double time);

  Polinom derive();

  long double binSearchRoot(long double lhs, long double rhs);

  std::vector<long double> getRoots(long double lhs, long double rhs);
};

std::istream& operator>>(std::istream& is, Polinom& polinom) {
  for (auto& i : polinom.coefs) is >> i;
  std::reverse(polinom.coefs.begin(), polinom.coefs.end()); 
  return is;
}

std::ostream& operator<<(std::ostream& os, const Polinom& polinom) {
  for (auto& i : polinom.coefs)
    os << i << " ";
  return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& polinom) {
  for (auto& i : polinom)
    os << i << " ";
  return os;
}

Polinom::Polinom(int size = 0) { coefs.resize(size); }

Polinom::Polinom(std::vector<long double> _coefs) : coefs(_coefs) {}

long double Polinom::eval(const long double time) {
  long double ans = 0, cur = 1;
  for (auto& i : coefs)
    ans += i * cur, cur *= time;
  return ans;
}

long double Polinom::operator()(const long double time) {
  return eval(time);
}

Polinom Polinom::derive() {
  auto copy = coefs;
  if (copy.size())
    copy.erase(copy.begin());

  for (int i = 0; i < copy.size(); ++i) {
    copy[i] *= i + 1;
  }

  return Polinom(copy);
}

long double Polinom::binSearchRoot(long double lhs, long double rhs) {
  bool tmp = eval(lhs) < eval(rhs);
  while (rhs - lhs > EPS) {
    auto mid = (rhs + lhs) / 2;
    (eval(mid) < 0 == tmp? lhs : rhs) = mid;
  }
  return lhs;
}

std::vector<long double> Polinom::getRoots(long double lhs, long double rhs) {
  auto putRoot = [&](auto& v, auto lhs, auto rhs) {
    if (auto root = binSearchRoot(lhs, rhs); abs(eval(root)) < smolEPS)
      v.push_back(root);
    return v;
  };

  if (coefs.size() <= 2) {
    std::vector<long double> ans;
    return putRoot(ans, lhs, rhs);
  }

  std::vector<long double> ans, real_ans;
  std::vector<long double> search = derive().getRoots(lhs, rhs);

  search.insert(search.begin(), lhs);
  search.insert(search.end(), rhs);

  for (int i = 1; i < search.size(); ++i) {
    putRoot(ans, search[i - 1], search[i]);
  }

  for (auto i = ans.begin(); i != ans.end(); ++i) {
    if (abs(*i - rhs) > EPS)
      real_ans.push_back(*i);
  }

  return real_ans;
}

int main() {
  // freopen(R"(C:\Code\CP-Lib\input.txt)", "r", stdin);

  // data
  int size;
  std::cin >> size;

  long double LHS, RHS;
  Polinom polinom(size + 1);

  // input
  std::cin >> polinom >> LHS >> RHS;

  // output
  std::cout << std::fixed << std::setprecision(9) << polinom.getRoots(LHS, RHS) << std::endl;
}
