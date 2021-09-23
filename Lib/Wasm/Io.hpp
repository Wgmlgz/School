#pragma once
#include <string>
#include <unordered_map>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE 
#endif

namespace wasm {

std::unordered_map<int, double> doubles;
std::unordered_map<int, int> ints;

extern "C" {
  EMSCRIPTEN_KEEPALIVE void ioSetDouble(int id, double val){
    doubles[id] = val;
  }
  EMSCRIPTEN_KEEPALIVE double ioGetDouble(int id){
    return doubles[id];
  }
  EMSCRIPTEN_KEEPALIVE void ioSetInt(int id, int val){
    ints[id] = val;
  }
  EMSCRIPTEN_KEEPALIVE double ioGetInt(int id){
    return ints[id];
  }
}
};  // namespace wasm
