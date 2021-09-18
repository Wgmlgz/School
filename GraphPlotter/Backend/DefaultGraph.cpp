#include <math.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "Graph.hpp"
#include "GraphsHandler.hpp"
#include "Parser.hpp"

int main() {
  GraphsHandler<double> graphs({
      {parser::parse("add(pow(2.0 x) 1.0)")},
      {parser::parse("pow(x, 5.0)")},
      {parser::parse("div(sub(1.0 x) 3.0)")},
  });

  double lhs = -10, rhs = 10;
  auto hull = graphs.hull(lhs, rhs, 0.01);

  auto hull_json = hull.toJson(lhs, rhs, 0.1);
  auto json = graphs.toJson(lhs, rhs, 0.1);
  auto f = std::ofstream("..\\Frontend\\graph.js");
  f << "var graph = { \"hull\": " << hull_json << ", \"graphs\": " << json << "}";
  f.close();
}
