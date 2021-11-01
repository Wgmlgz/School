#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

std::string range2arr(auto v, auto f = [](const auto& arg){ return arg; }) {
  std::stringstream ss;

  ss << "[";
  char b = 0;

  for (const auto& i : v) {
    if (b++) ss << ",";
    ss << f(i); 
  }
  ss << "]";
  return ss.str();
}

std::string range2arr(
    const std::string& name, auto v,
    auto f = [](const auto& arg) { return arg; }) {
  return '"' + name + "\":" + range2arr(v, f);
}
