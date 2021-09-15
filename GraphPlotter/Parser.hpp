#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <math.h>

namespace parser {

template <typename T>
using func1 = std::function<T(T)>;
template <typename T>
using func2 = std::function<T(T, T)>;

std::unordered_map<std::string, func2<double>> functions{
    {"add",  [](double a, double b) -> double {return a + b; }},
    {"mult", [](double a, double b) -> double {return a * b; }},
    {"pow",  [](double a, double b) -> double {return std::pow(a, b); }},
};

// struct Exp {
//   func
// };
template <typename T>
func1<T> parse(const std::string& str) {}

};