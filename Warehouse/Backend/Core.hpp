#pragma once
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std::literals;
using namespace std::chrono;

typedef int32_t Currency;
typedef long double Weight;
typedef system_clock::time_point TimePoint;
typedef uint64_t idt;
typedef int dayt;
typedef duration<double> dur;

struct Core {
  TimePoint clock;
  dayt day = 0;
  dur day_length = 1s;
  std::mt19937_64 rng = std::mt19937_64(time(0));
  std::random_device rd;
  std::default_random_engine generator = std::default_random_engine(rd());
  std::uniform_real_distribution<double> distr = std::uniform_real_distribution<double>(0.0, 1.0);

  std::map<idt, std::string> id2str;
  
  Core() { updateClock(); }
  double rngd() { return distr(generator); }
  void updateClock() {
    clock = time_point_cast<TimePoint::duration>(
        system_clock::time_point(system_clock::now()));
  }
  void reset() {
    id2str.clear();
    day = 0;
  }
};

Core core;
