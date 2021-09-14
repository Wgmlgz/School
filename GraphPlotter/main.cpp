#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>

#include "GraphsHandler.hpp"
#include "Graph.hpp"
#include "../CP-Lib/debug.hpp"

dbstream dd(std::cout);

int main() {
  GraphsHandler<double> graphs({
    {[](double x) -> double {return std::sqrt(x);}},
    {[](double x) -> double {return -0.5 * x + 4;}},
    {[](double x) -> double {return (2.0 / 3.0) * x - 3;}},
  });
  
  double lhs = 0.1, rhs = 10;
  auto intersections = graphs.findIntersections(lhs, rhs, 0.01);
  auto hull = graphs.hull(lhs, rhs, 0.01);

  deb(intersections);

  auto hull_json = hull.toJson(lhs, rhs, 0.1);
  auto json = graphs.toJson(lhs, rhs, 0.1);
  auto f = std::ofstream("graph.js");
  f << "var graph = { \"hull\": " << hull_json << ", \"graphs\": " << json << "}";

  std::cout << std::fixed << std::setprecision(10);
  deb(hull.surface(1e-6));
}
