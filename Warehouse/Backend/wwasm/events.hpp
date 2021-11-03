#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include "em_header.hpp"

namespace wwasm {
std::unordered_map<std::string, std::function<void()>> events;
extern "C" {
EMSCRIPTEN_KEEPALIVE void wwasmInvoke(const char* ptr) {
  events[std::string(ptr)]();
}
}
void setEvent(const std::string& str, std::function<void()> f) {
  events[str] = f;
}
}; // namespace wwasm

