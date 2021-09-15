#include <math.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "C:\Code\CP-Lib\Code\debug.hpp"
#include "Graph.hpp"
#include "GraphsHandler.hpp"
#include "Parser.hpp"

dbstream dd(std::cout);

int main() {
  GraphsHandler<double> graphs({
      // {[](double x) -> double {return std::sqrt(x);}},
      // {[](double x) -> double {return -0.5 * x + 4;}},
      // {[](double x) -> double {return (2.0 / 3.0) * x - 3;}},
      {parser::parse("add(pow(2.0, x), 1 .0)")},
      {parser::parse("pow(x, 5.0)")},
      {parser::parse("div(sub(1.0, x), 3.0)")},

      // {[](double x) -> double { return std::pow<double>(2, x) + 1; }},
      // {[](double x) -> double { return std::pow<double>(x, 5); }},
      // {[](double x) -> double { return (1 - x) / 3; }},
  });

  double lhs = -10, rhs = 10;
  auto intersections = graphs.findIntersections(lhs, rhs, 0.01);
  auto hull = graphs.hull(lhs, rhs, 0.01);

  gg(intersections);

  auto hull_json = hull.toJson(lhs, rhs, 0.01);
  auto json = graphs.toJson(lhs, rhs, 0.01);
  auto f = std::ofstream(".\\Web\\graph.js");
  f << "var graph = { \"hull\": " << hull_json << ", \"graphs\": " << json << "}";
  f.close();
  std::cout << std::fixed << std::setprecision(10);
  gg(hull.surface(1e-5));
}
