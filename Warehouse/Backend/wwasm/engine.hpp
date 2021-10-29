#pragma once
#include "canvas.hpp"
#include "entities.hpp"
#include <functional>

namespace wwasm {

std::function<void()> on_update;
std::function<void()> on_init;

bool is_init = false;
extern "C" {
  EMSCRIPTEN_KEEPALIVE int wwasmUpdate(int dt) {
    try {
      if (!is_init) {
        is_init = true;
        on_init();
      }

      on_update();
    } catch (std::runtime_error err) {
      std::cout << err.what() << std::endl;
    }
    return 0;
  }
}

struct init {
  init(std::function<void()> f) { on_init = f; }
};
struct update {
  update(std::function<void()> f) { on_update = f; }
};
}  // namespace wwasm
