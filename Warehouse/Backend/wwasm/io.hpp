#pragma once

#include <string>
#include <unordered_map>

#include "em_header.hpp"

namespace wwasm {

std::unordered_map<std::string, double> doubles;
std::unordered_map<std::string, int> ints;

extern "C" {
EMSCRIPTEN_KEEPALIVE void ioSetDouble(const char* ptr, double val) { doubles[std::string(ptr)] = val; }
EMSCRIPTEN_KEEPALIVE double ioGetDouble(const char* ptr) { return doubles[std::string(ptr)]; }
EMSCRIPTEN_KEEPALIVE void ioSetInt(const char* ptr, int val) { ints[std::string(ptr)] = val; }
EMSCRIPTEN_KEEPALIVE double ioGetInt(const char* ptr) { return ints[std::string(ptr)]; }
}
void ioSetDouble(std::string id, double val) { doubles[id] = val; }
double ioGetDouble(std::string id) { return doubles[id]; }
void ioSetInt(std::string id, int val) { ints[id] = val; }
double ioGetInt(std::string id) { return ints[id]; }
};  // namespace wasm
