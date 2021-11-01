#pragma once
#include <future>
#include <thread>

#include "Engine.hpp"
#include "wwasm/wwasm.hpp"

using namespace wwasm::literals;
using Pt = wwasm::Canvas::Pt;

typedef std::complex<wwasm::i32> Pti; 

auto pt2Frm(auto pt) {
  return 
      wwasm::frm("pos_x", wwasm::prp(pt.real())) |
      wwasm::frm("pos_y", wwasm::prp(pt.imag()));
}

/**
 * Singleton controller for wwasm stuff
 * Day timing:
 * 0 
 */
struct UiEngine {
  wwasm::Canvas& cnv;
  Engine& engine;
  TimePoint next_update;

  std::map<idt, Pt> locations;

  const Pti shelf_offset{0, -100};
  const Pti next_shelf_offset{0, -60};
  const Pti next_package_offset{60, 0};
  const Pti package_offset{15, 20};

  void createBuiling(Pt pos, Building* building, const std::string& img) {
    cnv.pushEntity(
      "building_" + std::to_string(building->id_),
      new wwasm::Img(pos, img, {400, 400})
    );
    locations[building->id_] = pos;
    pos += shelf_offset;

    if (auto warehouse = dynamic_cast<Warehouse*>(building)) {
      for (auto& [str, list] : warehouse->shelfs) {
        cnv.pushEntity("shelf_" + str,
                       new wwasm::Img(pos, "shelf_img", {2000, 200}));
        for (auto& i : list) {
        }
        pos += next_shelf_offset;
      }
    }
  }
  UiEngine(Engine& eng) : engine(eng), cnv(wwasm::Canvas::regiesterCanvas("main_canvas")) {
    // core.updateClock();

    Pt loc{-700, 0};
    auto angle = -0.6;
    // last_update = core.clock;
    auto angle_num = std::polar(1.0, angle);
    auto angle_num_r = std::polar(1.0, -angle);

    for (auto& i : engine.clients) {
      createBuiling(loc, &i, "client_img");
      loc *= angle_num;
    }

    createBuiling({0, 0}, &engine.warehouse_, "warehouse_img");
    createBuiling({2500, -500}, &engine.factory_, "factory_img");
    createBuiling({0, -1000}, &engine.trash_, "trash_img");

    engine.on_push_contract = onPushContract;
    engine.on_push_request = onPushRequest;
    engine.on_package_created = onPackageCreated;
  }  

  void update() {
    if (core.clock > next_update) {
      ++core.day;
      next_update = core.clock;
      next_update += duration_cast<seconds>(core.day_length);
      engine.update();

      /* Packages movement */
      auto package_move = [&](auto& pos, auto package) {
        auto new_pos = pos;
        auto old_pos = static_cast<Pti>(locations[package->id_]);

        locations[package->id_] = new_pos;
        pos += next_package_offset;

        auto s = "package_" + std::to_string(package->id_);
        cnv.playAnim(
          s,
          wwasm::anim({
              {0, pt2Frm(old_pos)},
              {0.5, pt2Frm(new_pos)},
            },
            [](wwasm::anim& a) {}
          )
        );
      };
      for (auto& [bid, building] : engine.buildings_list_) {
        auto building_pos = static_cast<Pti>(locations[bid]);
        if (auto warehouse = dynamic_cast<Warehouse*>(building)) {
          building_pos += shelf_offset;
          for (auto& [ptype, shelf] : warehouse->shelfs) {
            auto pos = building_pos;
            pos += package_offset;

            for (auto& package : shelf) {
              package_move(pos, package);
            }
            building_pos += next_shelf_offset;
          }
        } else {
          for (auto& package : building->storage) {
            auto t = building_pos + Pti{75, 75};
            package_move(t, package);
          }
        }
      }
      wwasm::ioSetStr("warehouse_json", engine.warehouse_json);
    }
    core.updateClock();
  }

  std::function<void(const Contract& contract)> onPushContract =
  [&](const Contract& contract) {
    wlog("New contract: " + contract.json());
  };

  std::function<void(const Request& request)> onPushRequest =
  [&](const Request& request) {
    wlog("New request: " + request.json());
    cnv.pushEntity("request_" + std::to_string(request.id_),
      new wwasm::Img(
        {0.0, 0.0},
        "request_img",
        {100, 100}
      )
    );

    auto a = locations[request.from_] + Pt{50, 50};
    auto b = locations[request.to_] + Pt{50, 50};

    auto start =
      wwasm::frm("pos_x", wwasm::prp(a.real())) |
      wwasm::frm("pos_y", wwasm::prp(a.imag()));

    auto end =
      wwasm::frm("pos_x", wwasm::prp(b.real())) |
      wwasm::frm("pos_y", wwasm::prp(b.imag()));

    cnv.playAnim(
      "request_" + std::to_string(request.id_),
      wwasm::anim({{0, start}, {1, end}}, [=](wwasm::anim& a) {
        cnv.popEntity("request_" + std::to_string(request.id_));
      })
    );
  };

  std::function<void(const std::shared_ptr<Package>&)> onPackageCreated =
  [&](const std::shared_ptr<Package>& package) {
    auto pos = locations[engine.factory_.id_];
    cnv.pushEntity("package_" + std::to_string(package->id_),
      new wwasm::Img(
        pos + Pt{75, 75},
        "package_img",
        {50, 50}
      )
    );
    locations[package->id_] = pos;
  };

  // std::function<void(const std::shared_ptr<Package>&)> onPackageMoved =
  // [&](const std::shared_ptr<Package>& package, idt to) {
  //   auto start =
  //     wwasm::frm("pos_x", wwasm::prp(locations[request.from_].real())) |
  //     wwasm::frm("pos_y", wwasm::prp(locations[request.from_].imag()));

  //   auto end =
  //     wwasm::frm("pos_x", wwasm::prp(locations[request.to_].real())) |
  //     wwasm::frm("pos_y", wwasm::prp(locations[request.to_].imag()));
    // cnv.pushEntity("package_" + std::to_string(package->id_),
    //   new wwasm::Img(
    //     {-100, -100},
    //     "package_img",
    //     {50, 50}
    //   )
    // );
  // };
// };
};

UiEngine ui_engine(engine);
