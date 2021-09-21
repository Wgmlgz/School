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

double lhs = -10, rhs = 10, step = 0.1;
GraphsHandler<double> graphs({
  {parser::parse("add(pow(2.0 x) 1.0)")},
  {parser::parse("pow(x, 5.0)")},
  {parser::parse("div(sub(1.0 x) 3.0)")},
});

const std::vector<std::pair<std::string, std::string>> lexer_tokens = {
     {"fun", R"(([^|]+))"},
};

Lexer lexer(lexer_tokens);

void setGraphs(char* cstr) {
  try {
    std::string str = cstr;
    auto tokens = lexer.parse(str);
    if (tokens.size() != 3) throw std::runtime_error("lol");

    GraphsHandler<double> newGraphs({
      {parser::parse(tokens[0].val)},
      {parser::parse(tokens[1].val)},
      {parser::parse(tokens[2].val)},
    });

    graphs = newGraphs;
  } catch(...) {
    std::cout << "Error lol" << std::endl;
  }
  free(cstr);
}

void setLhs(double lhs_) { lhs = lhs_; }
void setRhs(double rhs_) { rhs = rhs_; }
void setStep(double step_) { step = step_; }

char* getJson() {
  auto hull = graphs.hull(lhs, rhs, 0.0000001);

  auto json = "{ \"hull\": " +
    hull.toJson(lhs, rhs, step) +
    ", \"graphs\": " +
    graphs.toJson(lhs, rhs, step) + "}";

  char *cstr = new char[json.length() + 1];
  strcpy(cstr, json.c_str());
  return cstr;
}

double getSurface() {
  // return graphs.hull(lhs, rhs, 0.0000001).surfaceRandAuto(1e-3);
  return graphs.hull(lhs, rhs, 0.0000001).surface(1e-5);
}

