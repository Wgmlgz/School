#include "Engine.hpp"
// #include "wwasm/wwasm.hpp"


// auto _update = wwasm::update([]() { ui_engine.update(); });

int main() {
  while (true) {
    ++core.day;
    engine.update();

    wlog("--da--");
  }
}