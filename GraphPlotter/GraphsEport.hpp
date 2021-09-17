#pragma once
#include <math.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
// #include "C:\Code\CP-Lib\Code\debug.hpp"
#include "Graph.hpp"
#include "GraphsHandler.hpp"
#include "Parser.hpp"

GraphsHandler<double> graphs({
      // {[](double x) -> double {return std::sqrt(x);}},
      // {[](double x) -> double {return -0.5 * x + 4;}},
      // {[](double x) -> double {return (2.0 / 3.0) * x - 3;}},
      {parser::parse("add(pow(2.0 x) 1.0)")},
      {parser::parse("pow(x, 5.0)")},
      {parser::parse("div(sub(1.0 x) 3.0)")},

      // {[](double x) -> double { return std::pow<double>(2, x) + 1; }},
      // {[](double x) -> double { return std::pow<double>(x, 5); }},
      // {[](double x) -> double { return (1 - x) / 3; }},
});

const std::vector<std::pair<std::string, std::string>> lexer_tokens = {
     {"fun", R"(([^|]+))"},
};
// bool readyJson() {}
Lexer lexer(lexer_tokens);

void setGraphs(char* cstr) {
  try {
    std::string str = cstr;
    auto tokens = lexer.parse(str);
    if (tokens.size() != 3) throw std::runtime_error("lol");
    std::cout << 228 << std::endl;
    GraphsHandler<double> newGraphs({
      {parser::parse(tokens[0].val)},
      {parser::parse(tokens[1].val)},
      {parser::parse(tokens[2].val)},
    });

    graphs = newGraphs;
  } catch(...) {
    std::cout << "Error lol" << std::endl;
  }
  
}
char* getJson() {
  double lhs = -10, rhs = 10;
  auto intersections = graphs.findIntersections(lhs, rhs, 0.01);
  auto hull = graphs.hull(lhs, rhs, 0.01);

  auto json = "{ \"hull\": " +
    hull.toJson(lhs, rhs, 0.1) +
    ", \"graphs\": " +
    graphs.toJson(lhs, rhs, 0.1) + "}";

  char *cstr = new char[json.length() + 1];
  strcpy(cstr, json.c_str());
  return cstr;
}

