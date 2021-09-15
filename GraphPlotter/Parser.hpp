#pragma once
#include <math.h>

#include <exception>
#include <functional>
#include <string>
#include <unordered_map>

#include "Lexer.hpp"

namespace parser {
// lexer
const std::vector<std::pair<std::string, std::string>> lexer_tokens = {
    {"x", R"((x))"},
    {"fun", R"(([_a-zA-Z][_a-zA-Z0-9]*))"},
    {"num", R"(([0-9]*\.[0-9]*))"},
};

Lexer lexer(lexer_tokens);

// parser
using func = std::function<double(double)>;
using func2 = std::function<double(double, double)>;

std::unordered_map<std::string, func2> functions{
    {"add", [](double a, double b) -> double { return a + b; }},
    {"sub", [](double a, double b) -> double { return a - b; }},
    {"mult", [](double a, double b) -> double { return a * b; }},
    {"div", [](double a, double b) -> double { return a / b; }},
    {"pow", [](double a, double b) -> double { return std::pow(a, b); }},
};

func parse(std::vector<Token>::iterator& it) {
  if (it->token == "num") {
    double y = std::stod("0" + it->val + "0");
    ++it;
    return [=](double x) -> double { return y; };
  }

  if (it->token == "x") {
    ++it;
    return [](double x) -> double { return x; };
  }

  if (it->token == "fun") {
    auto f = functions[it->val];

    ++it;

    auto lhs = parse(it);
    auto rhs = parse(it);

    return [=](double x) -> double {
      auto a = lhs(x);
      auto b = rhs(x);

      return f(a, b);
    };
  }

  throw std::runtime_error("err");
}

func parse(const std::string& str) {
  try {
    auto tokens = lexer.parse(str, false, 1);

    auto it = tokens.begin();
    return parser::parse(it);
  } catch (...) {
    return [](double x) -> double { return 1e18; };
  }
}
};  // namespace parser
