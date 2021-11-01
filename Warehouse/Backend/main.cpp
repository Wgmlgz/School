#include "wwasm/wwasm.hpp"
#include "UiEngine.hpp"

auto _init = wwasm::init([]() {
  
});

auto _update = wwasm::update([]() {
  ui_engine.update();
});
