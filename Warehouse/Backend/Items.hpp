#pragma once
#include "Core.hpp"
#include "Logger.hpp"
#include "json.hpp"

using json = nlohmann::json;

struct PackageInfo {
  std::string type_ = "UNDEFINED";
  dayt expiration_time_ = 1;
  Currency cost_ = 1;
  Weight weight_ = 1;
  uint32_t count_in_package_ = 1;
  static std::map<std::string, PackageInfo> items;

  std::string json() const {
    std::string res = "{";
    res += R"("type":)"s + '"' + type_ + '"' + ',';
    res += R"("expiration time":)" + std::to_string(expiration_time_) + ',';
    res += R"("cost":)" + std::to_string(cost_) + ',';
    res += R"("weight":)" + std::to_string(weight_) + ',';
    res += R"("count in package":)" + std::to_string(count_in_package_);
    res += "}";
    return res;
  }
};

void to_json(json& j, const PackageInfo& pi) {
  j = json{
    {"type", pi.type_},
    {"expiration time", std::to_string(pi.expiration_time_)},
    {"cost",  std::to_string(pi.cost_)},
    {"weight", std::to_string(pi.weight_)},
    {"count in package", std::to_string(pi.count_in_package_)}
  };
}

std::map<std::string, PackageInfo> PackageInfo::items{
    {"DEFAULT", {"DEFAULT", 1, 1, 1, 1}},             /* 1 egg */
    {"Egg", {"Egg", 90, 6, 0.05, 20}},                /* 1 egg */
    {"Milk", {"Milk", 2, 55, 1, 10}},                 /* 1 boutle */
    {"Tissues", {"Tissues", 1000000000, 75, 0.1, 5}}, /* 1 package */
    {"Fish", {"Fish", 2, 1800, 2, 10}},               /* 1 fish */
    {"LSD", {"LSD", 1000000000, 255000, 0.001, 2}},   /* 1 gram */
};

struct Package {
  dayt production_time_;
  PackageInfo& package_info;

  Currency cost_ = 1;
  Weight weight_ = 1;

  uint64_t id_;
  uint64_t house_id_;

  Package(std::string type, dayt production_time, idt house_id)
  : package_info(PackageInfo::items[type]), production_time_(production_time) {
    cost_ = package_info.cost_ * package_info.count_in_package_;
    weight_ = package_info.weight_ * package_info.count_in_package_;
    id_ = core.rng();
    house_id_ = house_id;

    wlog(json());
  }

  std::string json() {
    std::string res = "{";

    res += R"("production time":)" + std::to_string(production_time_) + ',';
    res += R"("package info":)" + (package_info.type_) + ',';
    res += R"("cost":)" + std::to_string(cost_) + ',';
    res += R"("weight":)" + std::to_string(weight_) + ',';
    res += R"("id":)" + std::to_string(id_)+ ',';
    res += R"("house id":)" + std::to_string(house_id_);

    res += "}";
    return res;
  }
};

void to_json(json& j, const Package& pi) {
  j = json{
    {"production time", pi.production_time_},
    {"package info", pi.package_info.type_},
    {"cost", std::to_string(pi.cost_)},
    {"weight", std::to_string(pi.weight_)},
    {"house id", std::to_string(pi.house_id_)}
  };
}