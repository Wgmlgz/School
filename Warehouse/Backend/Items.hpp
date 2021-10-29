#pragma once
#include "Core.hpp"

struct PackageInfo {
  std::string type_ = "UNDEFINED";
  dur expiration_time_ = 1h;
  Currency cost_ = 1;
  Weight weight_ = 1;
  uint32_t count_in_package_ = 1;
  static std::map<std::string, PackageInfo> items;
};

std::map<std::string, PackageInfo> PackageInfo::items{
  {"DEFAULT", {"DEFAULT", 24h, 1, 1, 1}},    /* 1 egg */

  {"Egg", {"Egg", 24h * 90, 6, 0.05, 20}},    /* 1 egg */
  {"Milk", {"Milk", 48h, 55, 1, 10}},         /* 1 boutle */
  {"Tissues", {"Tissues", 1000000000h, 75, 0.1, 5}}, /* 1 package */
  {"Fish", {"Fish", 48h, 1800, 2, 10}},       /* 1 fish */
  {"LSD", {"LSD", 1000000000h, 255000, 0.001, 2}},   /* 1 gram */
};

struct Package {
  time_point<std::chrono::system_clock> production_time_;
  PackageInfo& package_info;

  Currency cost_ = 1;
  Weight weight_ = 1;

  uint64_t id_;
  uint64_t house_id_;

  Package(std::string type,
          time_point<std::chrono::system_clock> production_time)
      : package_info(PackageInfo::items[type]), production_time_(production_time) {
    cost_ = package_info.cost_ * package_info.count_in_package_;
    weight_ = package_info.weight_ * package_info.count_in_package_;
    id_ = core.rng();
    house_id_ = 0;
  }

  void moveToBuilding(idt id) { house_id_ = id; }
};