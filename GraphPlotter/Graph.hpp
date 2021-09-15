#pragma once
#include <functional>
#include <vector>
#include <utility>
#include <assert.h>
#include <string>
#include <math.h>

template<typename T = double>
struct Graph {
  typedef std::function<T(T)> lfunc;
private:
  lfunc func;
public:
  Graph(lfunc _func) : func(_func) {}

  T eval(T x) const {
    return func(x);
  }

  T evalView(T x) const {
    T t = func(x);
    if (t > 10) return 1e18;
    if (t < -5) return 1e18;
    return t;
  }

  std::vector<std::pair<T, T>> evalRange(T lhs, T rhs, T step, bool view = false) {
    assert(lhs < rhs);
    assert(step > 0);

    std::vector<std::pair<T, T>> res;

    for (T i = lhs; i < rhs; i += step)
      res.emplace_back(i, view ? evalView(i) : eval(i));
    
    return res;
  }

  std::string toJson(T lhs, T rhs, T step) {
    assert(lhs < rhs);
    assert(step > 0);

    auto res = evalRange(lhs, rhs, step, 1);

    std::string json, x, y;

    for (auto i = begin(res); i != end(res); ++i) {
      x += std::to_string(i->first);
      y += std::abs(i->second) > 1e9 ? "\"NaN\"" : std::to_string(i->second);
      if ((i + 1) != end(res)) x += ", ", y += ", ";
    }
    
    json += "{";
    json += "\"size\": " + std::to_string(res.size()) + ", ";
    json += "\"x\": [" + x + "], ";
    json += "\"y\": [" + y + "]";
    json += "}";

    return json;
  }

  T findRoot(const T lhs, const T rhs, const T EPS) {
    auto a = lhs, b = rhs;

    while(std::abs(b - a) > EPS)
        a = b - (b - a) * eval(b) / (eval(b) - eval(a)),
        b = a - (a - b) * eval(a) / (eval(a) - eval(b));

    return b;
  }

  std::vector<T> findRoots(const T lhs, const T rhs, const T step, const T EPS) {
    assert(lhs < rhs);
    assert(step > 0);

    std::vector<T> res;

    bool last = 0;
    for (auto i = lhs; i < rhs; i += step) 
      if (std::abs(eval(i)) < EPS) {
        if (!last) {
          res.push_back(i), last = 1;
        }
      } else {
        last = 0;
      }

    return res;
  }

  T surface(const T lhs, const T rhs, const T step) {
    T sum = 0;
    for (auto i = lhs + 1e-5; i < rhs; i += step) 
      sum += eval(i);
    sum *= step;

    return sum;
  }

  T surfaceAuto(const T lhs, const T rhs, const T EPS) {
    assert(lhs < rhs);

    size_t n = 8;
  
    T cur_surface = -1e18, double_surface = -1e18; 
    do {
      cur_surface = double_surface;
      T step = (rhs - lhs) / n;
      double_surface = surface(lhs, rhs, step);
      n <<= 1;
    } while (std::abs(cur_surface - double_surface) > EPS);

    return double_surface;
  }
};
