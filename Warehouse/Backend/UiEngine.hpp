#pragma once
#include <future>
#include <thread>

#include "Engine.hpp"
#include "wwasm/wwasm.hpp"

using namespace wwasm::literals;

/** @brief Controller for wwasm::Canvas */
class UiEngine {
 private:
  static auto pt2Frm(auto pt) {
    return wwasm::frm("pos_x", wwasm::prp(pt.real())) |
           wwasm::frm("pos_y", wwasm::prp(pt.imag()));
  }

 public:
  UiEngine(Engine& eng, wwasm::Canvas& canvas) : engine(eng), cnv(canvas) {
    Pt loc{-2000, 0};
    auto angle = -0.4;
    auto angle_num = std::polar(1.0, angle);
    auto angle_num_r = std::polar(1.0, -angle);

    for (auto& i : engine.clients) {
      createBuiling(loc, &i, "client_img");
      loc *= angle_num;
    }

    createBuiling({0, 0}, &engine.warehouse_, "warehouse_img",
                  {-25, -400 * 2 - 200},
                  {-25, -400},
                  {400 * 5 + 50, 400 * 3 + 200});
    createBuiling({2500, -500}, &engine.factory_, "factory_img");
    createBuiling({(400 * 5 + 50) / 2 - 200, -2000}, &engine.trash_, "trash_img");

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
      auto package_move = [&](auto& pos, auto package, bool remove = false) {
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
            [=](wwasm::anim& a) {
              if (remove) {
                cnv.popEntity(s);
              }
            }
          )
        );
      };
      //return;

      for (auto& [bid, building] : engine.buildings_list_) {
        auto building_pos = static_cast<Pti>(locations[bid]);
        if (auto warehouse = dynamic_cast<Warehouse*>(building)) {
          building_pos = raw_locations[bid];
          building_pos += shelf_offset;
          for (auto& [ptype, shelf] : warehouse->getShelfs()) {
            auto pos = building_pos;
            pos += package_offset;

            for (auto& package : shelf) {
              package_move(pos, package);
            }
            building_pos += next_shelf_offset;
          }
        } else {
          for (auto& package : building->storage) {
            auto t = building_pos + building_offset;
            package_move(t, package, true);
          }
        }
      }
      wwasm::ioSetStr("warehouse_json", engine.warehouse_json.dump());
    }
    core.updateClock();
  }

  ~UiEngine() { cnv.clear(); }

 private:
  typedef wwasm::Canvas::Pt Pt;
  typedef std::complex<wwasm::i32> Pti;

  wwasm::Canvas& cnv;
  Engine& engine;
  TimePoint next_update;

  std::map<idt, Pt> locations;
  std::map<idt, Pt> raw_locations;

  const Pti shelf_offset{0, 200};
  const Pti next_shelf_offset{0, -60};
  const Pti next_package_offset{60, 0};
  const Pti package_offset{15, 20};
  const Pti building_offset{0, 0};

  void createBuiling(Pt pos, Building* building, const std::string& img,
                     Pt offset = {0, 0}, Pt raw_offset = {0, 0},
                     Pt size = {400, 400}) {
    cnv.pushEntity("building_" + std::to_string(building->id()),
                   new wwasm::Img(pos + offset, img, size));
    locations[building->id()] =
        raw_offset + pos + Pt{size.real() / 2 - 25, size.imag() / 2 - 25};
    raw_locations[building->id()] = pos;
    pos += shelf_offset;

    if (auto warehouse = dynamic_cast<Warehouse*>(building)) {
      for (auto& [str, list] : warehouse->getShelfs()) {
        cnv.pushEntity("shelf_" + str,
                       new wwasm::Img(pos, "shelf_img", {2000, 200}));
        for (auto& i : list) {
        }
        pos += next_shelf_offset;
      }
    }
  }

  std::function<void(const Request& request)> onPushRequest =
  [&](const Request& request) {
    cnv.pushEntity("request_" + std::to_string(request.id_),
      new wwasm::Img(
        {0.0, 0.0},
        "request_img",
        {100, 100}
      )
    );

    auto a = locations[request.from_] + Pt(building_offset.real(), building_offset.imag());
    auto b = locations[request.to_] + Pt(building_offset.real(), building_offset.imag());

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
    auto pos = locations[engine.factory_.id()];
    cnv.pushEntity("package_" + std::to_string(package->id_),
      new wwasm::Img(
        pos + Pt(building_offset.real(), building_offset.imag()),
        package->package_info.type_,
        {50, 50}
      )
    );
    locations[package->id_] = pos;
  };
};

UiEngine ui_engine(engine, wwasm::Canvas::getCanvas("default"));
