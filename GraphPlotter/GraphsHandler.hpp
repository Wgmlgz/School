#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "C:\Code\CP-Lib\Code\debug.hpp"
#include "Graph.hpp"

template<typename T>
struct Graphs {
  std::vector<Graph<T>> graphs_;

  Graphs(std::vector<Graph<T>> graphs) : graphs_(graphs) {}

  std::string toJson(T lhs, T rhs, T step) {
    std::string json;

    json += "{\"graphs\":[";

    for (auto i = std::begin(graphs_); i != std::end(graphs_); ++i) {
      json += i->toJson(lhs, rhs, step);
      if ((i + 1) != std::end(graphs_)) json += ", ";
    }
    json += "]}";

    return json;
  }
};

template<typename T>
struct Hull : public Graphs<T> {
  T lhs_, rhs_;

  Hull(std::vector<Graph<T>> graphs, T lhs, T rhs)
      : Graphs<T>(graphs), lhs_(lhs), rhs_(rhs) {}

  T surface(const T EPS) {
    assert(Graphs<T>::graphs_.size() == 2);

    return Graphs<T>::graphs_[0].surfaceAuto(lhs_, rhs_, EPS) -
           Graphs<T>::graphs_[1].surfaceAuto(lhs_, rhs_, EPS);
  }
};

template<typename T>
struct GraphsHandler : Graphs<T> {
  GraphsHandler(std::vector<Graph<T>> graphs) : Graphs<T>(graphs) {}

  std::vector<std::pair<T, std::pair<size_t, size_t>>> findIntersections(
      const T lhs, const T rhs, const T EPS) {
    std::vector<std::pair<T, std::pair<size_t, size_t>>> ans;

    for (int i = 0; i < Graphs<T>::graphs_.size(); ++i) {
      for (int j = i + 1; j < Graphs<T>::graphs_.size(); ++j) {
        auto tmp = Graph<T>([&](T x) -> T {
          return Graphs<T>::graphs_[i].eval(x) - Graphs<T>::graphs_[j].eval(x);
        })
        .findRootBin(lhs, rhs, EPS);
        // .findRoot(lhs, rhs, EPS);
        // .findRoots(lhs, rhs, 0.01, EPS);

        ans.push_back({tmp, {i, j}});
        // for (auto k : tmp) ans.push_back({k, {i, j}});
      }
    }

    return ans;
  }

  Hull<T> hull(const T lhs, const T rhs, const T EPS) {
    auto intersections = findIntersections(lhs, rhs, EPS);
    assert(intersections.size() == 3);

    std::sort(std::begin(intersections), std::end(intersections));
    auto mid = intersections[1];

    T lbound = intersections[0].first;
    T rbound = intersections[2].first;

    std::cout << rbound << std::endl;

    T mx = -1e9;
    for (auto i : Graphs<T>::graphs_) mx = std::max(mx, i.eval(mid.first));

    std::set<size_t> st{0, 1, 2};
    st.erase(mid.second.first);
    st.erase(mid.second.second);

    size_t excluded = *std::begin(st);

    if (Graphs<T>::graphs_[mid.second.first].eval(mid.first) == mx) {
      // top merged
      Graph<T> top([=](double x) -> double {
        if (x < lbound or x > rbound) return 1e18;
        return std::min(Graphs<T>::graphs_[mid.second.first].eval(x),
                        Graphs<T>::graphs_[mid.second.second].eval(x));
      });
      Graph<T> bottom([=](double x) -> double {
        if (x < lbound or x > rbound) return 1e18;
        return Graphs<T>::graphs_[excluded].eval(x);
      });

      return Hull<T>({top, bottom}, lbound, rbound);
    } else {
      // bottom merged
      Graph<T> bottom([=](double x) -> double {
        if (x < lbound or x > rbound) return 1e18;

        return std::max(Graphs<T>::graphs_[mid.second.first].eval(x),
                        Graphs<T>::graphs_[mid.second.second].eval(x));
      });
      Graph<T> top([=](double x) -> double {
        if (x < lbound or x > rbound) return 1e18;
        return Graphs<T>::graphs_[excluded].eval(x);
      });

      return Hull<T>({top, bottom}, lbound, rbound);
    }
  }
};