#include "wwasm/wwasm.hpp"
#include "UiEngine.hpp"
#include "Json.hpp"

using json = nlohmann::json;

auto _init = wwasm::init([]() {
  wlog("init");
  wwasm::Canvas::regiesterCanvas("main_canvas");
  wwasm::setEvent("restart", [&]() {
    core.reset();
    engine.~Engine();
    ui_engine.~UiEngine();
    new(&engine) Engine(json::parse(wwasm::ioGetStr("settings")));
    new(&ui_engine) UiEngine(engine, wwasm::Canvas::getCanvas("main_canvas"));
  });
  wwasm::setEvent("next_day", [&]() {
    ui_engine.nextDay();
  });

});

auto _update = wwasm::update([]() {
  ui_engine.update();
});
