#pragma once
#include <future>
#include <thread>

#include "Engine.hpp"
#include "wwasm/wwasm.hpp"

using namespace wwasm::literals;
using Pt = wwasm::Canvas::Pt;

/**
 * Singleton controller for wwasm stuff
 */
struct UiEngine {
  wwasm::Canvas& cnv;
  Engine& engine;
  TimePoint last_update;

  std::map<idt, Pt> locations;

  UiEngine(Engine& eng) : engine(eng), cnv(wwasm::Canvas::regiesterCanvas("main_canvas")) {
    core.updateClock();

    Pt loc{-700, 0};
    auto angle = -0.6;
    last_update = core.clock;
    auto angle_num = std::polar(1.0, angle);
    auto angle_num_r = std::polar(1.0, -angle);

    for (auto& i : engine.clients) {
      locations[i.id_] = loc;
      cnv.pushEntity(
        "client_" + std::to_string(i.id_),
        new wwasm::Img(loc, "client_img", {200, 200})
      );
      loc *= angle_num;
    }

    cnv.pushEntity(
      "warehouse_" + std::to_string(engine.warehouse_.id_),
      new wwasm::Img({0, 0}, "warehouse_img", {200, 200})
    );
    locations[engine.warehouse_.id_] = {0, 0};

    cnv.pushEntity(
      "factory_" + std::to_string(engine.factory_.id_),
      new wwasm::Img({500, 0}, "factory_img", {200, 200})
    );
    locations[engine.factory_.id_] = {500, 0};

    cnv.pushEntity(
      "trash_" + std::to_string(engine.trash_.id_),
      new wwasm::Img({0, -500}, "trash_img", {200, 200})
    );
    locations[engine.trash_.id_] = {0, -500};

    engine.on_push_contract = onPushContract;
    engine.on_push_request = onPushRequest;
  }  

  void update() {
    core.updateClock();
    dur dt = core.clock - last_update;
    engine.update(dt);
    last_update = core.clock;
  }

  std::function<void(const Contract& contract)> onPushContract =
  [&](const Contract& contract) {
    std::cout << "New contract: " << contract.id_ << std::endl;
  };

  std::function<void(const Request& request)> onPushRequest =
  [&](const Request& request) {
    std::cout << "New request: " << request.id_ << std::endl;
    cnv.pushEntity("request_" + std::to_string(request.id_),
      new wwasm::Img(
        {0.0, 0.0},
        "request_img",
        {100, 100}
      )
    );

    auto start =
      wwasm::frm("pos_x", wwasm::prp(locations[request.from_].real())) |
      wwasm::frm("pos_y", wwasm::prp(locations[request.from_].imag()));

    auto end =
      wwasm::frm("pos_x", wwasm::prp(locations[request.to_].real())) |
      wwasm::frm("pos_y", wwasm::prp(locations[request.to_].imag()));

    cnv.playAnim(
      "request_" + std::to_string(request.id_),
      wwasm::anim({{0, start}, {1, end}}, [=](wwasm::anim& a) {
        cnv.popEntity("request_" + std::to_string(request.id_));
      })
    );
  };
};

UiEngine ui_engine(engine);
